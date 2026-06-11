#ifndef SELECTEDAPPSMODEL_H
#define SELECTEDAPPSMODEL_H

#include "appinfo.h"

#include <QAbstractListModel>
#include <QStringList>

class AppModel;

class SelectedAppsModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ count NOTIFY countChanged)
    Q_PROPERTY(int maxCount READ maxCount CONSTANT)

public:
    enum Roles {
        NameRole = Qt::UserRole + 1,
        DesktopPathRole,
        IconSourceRole,
        IsAndroidRole,
        MissingRole,
    };

    static const int kMaxCount = 5;

    explicit SelectedAppsModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    int count() const;
    int maxCount() const;

    QStringList desktopPaths() const;

    Q_INVOKABLE void loadFromPaths(const QStringList &paths, AppModel *appModel);
    Q_INVOKABLE bool appendFromMap(const QVariantMap &appInfo);
    Q_INVOKABLE void removeAt(int index);
    Q_INVOKABLE void removeByPath(const QString &path);
    Q_INVOKABLE void move(int from, int to, int count);
    Q_INVOKABLE bool containsPath(const QString &path) const;
    Q_INVOKABLE void clear();

signals:
    void countChanged();
    void changed();

private:
    AppInfo appInfoFromMap(const QVariantMap &map) const;
    AppInfo placeholderForPath(const QString &path) const;

    QList<AppInfo> m_apps;
};

#endif
