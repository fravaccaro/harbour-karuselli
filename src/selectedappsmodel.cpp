#include "selectedappsmodel.h"
#include "appmodel.h"

#include <QFileInfo>

SelectedAppsModel::SelectedAppsModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

int SelectedAppsModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return m_apps.size();
}

QVariant SelectedAppsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() < 0 || index.row() >= m_apps.size())
        return QVariant();

    const AppInfo &info = m_apps.at(index.row());

    switch (role) {
    case NameRole:
        return info.name;
    case DesktopPathRole:
        return info.desktopPath;
    case IconSourceRole:
        return info.iconSource;
    case IsAndroidRole:
        return info.isAndroid;
    case MissingRole:
        return info.missing;
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> SelectedAppsModel::roleNames() const
{
    return {
        { NameRole, "name" },
        { DesktopPathRole, "desktopPath" },
        { IconSourceRole, "iconSource" },
        { IsAndroidRole, "isAndroid" },
        { MissingRole, "missing" },
    };
}

int SelectedAppsModel::count() const
{
    return m_apps.size();
}

int SelectedAppsModel::maxCount() const
{
    return kMaxCount;
}

QStringList SelectedAppsModel::desktopPaths() const
{
    QStringList paths;
    paths.reserve(m_apps.size());
    for (const AppInfo &info : m_apps)
        paths.append(info.desktopPath);
    return paths;
}

void SelectedAppsModel::loadFromPaths(const QStringList &paths, AppModel *appModel)
{
    beginResetModel();
    m_apps.clear();

    for (const QString &path : paths) {
        if (path.isEmpty())
            continue;

        const QVariantMap found = appModel ? appModel->lookup(path) : QVariantMap();
        if (found.isEmpty())
            m_apps.append(placeholderForPath(path));
        else
            m_apps.append(appInfoFromMap(found));
    }

    endResetModel();
    emit countChanged();
    emit changed();
}

bool SelectedAppsModel::appendFromMap(const QVariantMap &appInfo)
{
    if (m_apps.size() >= kMaxCount || appInfo.isEmpty())
        return false;

    const QString path = appInfo.value(QStringLiteral("desktopPath")).toString();
    if (path.isEmpty() || containsPath(path))
        return false;

    beginInsertRows(QModelIndex(), m_apps.size(), m_apps.size());
    m_apps.append(appInfoFromMap(appInfo));
    endInsertRows();

    emit countChanged();
    emit changed();
    return true;
}

void SelectedAppsModel::removeAt(int index)
{
    if (index < 0 || index >= m_apps.size())
        return;

    beginRemoveRows(QModelIndex(), index, index);
    m_apps.removeAt(index);
    endRemoveRows();

    emit countChanged();
    emit changed();
}

void SelectedAppsModel::removeByPath(const QString &path)
{
    if (path.isEmpty())
        return;

    for (int i = 0; i < m_apps.size(); ++i) {
        if (m_apps.at(i).desktopPath == path) {
            removeAt(i);
            return;
        }
    }
}

void SelectedAppsModel::move(int from, int to, int count)
{
    if (count != 1 || from < 0 || from >= m_apps.size())
        return;
    if (to < 0 || to >= m_apps.size())
        return;
    if (from == to)
        return;

    const int destChild = from < to ? to + 1 : to;
    if (!beginMoveRows(QModelIndex(), from, from, QModelIndex(), destChild))
        return;

    const AppInfo item = m_apps.takeAt(from);
    m_apps.insert(to, item);
    endMoveRows();

    emit changed();
}

bool SelectedAppsModel::containsPath(const QString &path) const
{
    for (const AppInfo &info : m_apps) {
        if (info.desktopPath == path)
            return true;
    }
    return false;
}

void SelectedAppsModel::clear()
{
    if (m_apps.isEmpty())
        return;

    beginResetModel();
    m_apps.clear();
    endResetModel();

    emit countChanged();
    emit changed();
}

AppInfo SelectedAppsModel::appInfoFromMap(const QVariantMap &map) const
{
    AppInfo info;
    info.name = map.value(QStringLiteral("name")).toString();
    info.desktopPath = map.value(QStringLiteral("desktopPath")).toString();
    info.iconSource = map.value(QStringLiteral("iconSource")).toString();
    info.isAndroid = map.value(QStringLiteral("isAndroid")).toBool();
    info.missing = map.value(QStringLiteral("missing")).toBool();
    return info;
}

AppInfo SelectedAppsModel::placeholderForPath(const QString &path) const
{
    AppInfo info;
    info.desktopPath = path;
    info.name = QFileInfo(path).completeBaseName();
    info.iconSource = QStringLiteral("image://theme/icon-m-warning");
    info.isAndroid = path.contains(QStringLiteral("apkd_launcher"));
    info.missing = true;
    return info;
}
