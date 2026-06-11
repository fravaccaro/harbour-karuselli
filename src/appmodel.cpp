#include "appmodel.h"
#include "desktopparser.h"

#include <algorithm>

#include <QDir>
#include <QStandardPaths>

AppModel::AppModel(QObject *parent)
    : QAbstractListModel(parent)
{
    refresh();
}

int AppModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return m_visibleApps.size();
}

QVariant AppModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() < 0 || index.row() >= m_visibleApps.size())
        return QVariant();

    const AppInfo &info = m_visibleApps.at(index.row());

    switch (role) {
    case NameRole:
        return info.name;
    case DesktopPathRole:
        return info.desktopPath;
    case IconSourceRole:
        return info.iconSource;
    case IsAndroidRole:
        return info.isAndroid;
    case CategoryRole:
        return info.isAndroid ? QStringLiteral("1-android") : QStringLiteral("0-native");
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> AppModel::roleNames() const
{
    return {
        { NameRole, "name" },
        { DesktopPathRole, "desktopPath" },
        { IconSourceRole, "iconSource" },
        { IsAndroidRole, "isAndroid" },
        { CategoryRole, "category" },
    };
}

QString AppModel::filterText() const
{
    return m_filterText;
}

void AppModel::setFilterText(const QString &text)
{
    if (m_filterText == text)
        return;

    m_filterText = text;
    rebuildVisible();
    emit filterTextChanged();
}

void AppModel::refresh()
{
    beginResetModel();
    m_allApps.clear();
    m_byPath.clear();

    scanDirectory(QStringLiteral("/usr/share/applications"));

    const QString localApps = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation)
            + QStringLiteral("/applications");
    scanDirectory(localApps);

    std::sort(m_allApps.begin(), m_allApps.end(), [](const AppInfo &a, const AppInfo &b) {
        return a.name.compare(b.name, Qt::CaseInsensitive) < 0;
    });

    m_visibleApps.clear();
    for (const AppInfo &info : m_allApps) {
        if (passesFilter(info))
            m_visibleApps.append(info);
    }
    endResetModel();
}

void AppModel::scanDirectory(const QString &dirPath)
{
    QDir dir(dirPath);
    if (!dir.exists())
        return;

    const QStringList entries = dir.entryList({ QStringLiteral("*.desktop") }, QDir::Files);
    for (const QString &fileName : entries) {
        const QString path = dir.absoluteFilePath(fileName);
        if (m_byPath.contains(path))
            continue;

        AppInfo info;
        if (!parseDesktopFile(path, &info))
            continue;

        m_allApps.append(info);
        m_byPath.insert(path, info);
    }
}

bool AppModel::passesFilter(const AppInfo &info) const
{
    if (m_filterText.isEmpty())
        return true;
    return info.name.contains(m_filterText, Qt::CaseInsensitive);
}

void AppModel::rebuildVisible()
{
    QList<AppInfo> filtered;
    filtered.reserve(m_allApps.size());
    for (const AppInfo &info : m_allApps) {
        if (passesFilter(info))
            filtered.append(info);
    }

    if (filtered.size() == m_visibleApps.size()) {
        bool unchanged = true;
        for (int i = 0; i < filtered.size(); ++i) {
            if (filtered.at(i).desktopPath != m_visibleApps.at(i).desktopPath) {
                unchanged = false;
                break;
            }
        }
        if (unchanged)
            return;
    }

    emit layoutAboutToBeChanged();
    m_visibleApps = filtered;
    emit layoutChanged();
}

bool AppModel::containsPath(const QString &path) const
{
    return m_byPath.contains(path);
}

QVariantMap AppModel::lookup(const QString &path) const
{
    const auto it = m_byPath.constFind(path);
    if (it == m_byPath.constEnd())
        return QVariantMap();

    const AppInfo &info = it.value();
    return {
        { QStringLiteral("name"), info.name },
        { QStringLiteral("desktopPath"), info.desktopPath },
        { QStringLiteral("iconSource"), info.iconSource },
        { QStringLiteral("isAndroid"), info.isAndroid },
        { QStringLiteral("missing"), false },
    };
}
