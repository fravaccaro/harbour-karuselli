#ifndef APPMODEL_H
#define APPMODEL_H

#include "appinfo.h"

#include <QAbstractListModel>
#include <QHash>
#include <QStringList>

class AppModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QString filterText READ filterText WRITE setFilterText NOTIFY filterTextChanged)

public:
    enum Roles {
        NameRole = Qt::UserRole + 1,
        DesktopPathRole,
        IconSourceRole,
        IsAndroidRole,
        CategoryRole,
    };

    explicit AppModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    QString filterText() const;
    void setFilterText(const QString &text);

    Q_INVOKABLE void refresh();
    Q_INVOKABLE bool containsPath(const QString &path) const;
    Q_INVOKABLE QVariantMap lookup(const QString &path) const;

signals:
    void filterTextChanged();

private:
    void scanDirectory(const QString &dirPath);
    bool passesFilter(const AppInfo &info) const;
    void rebuildVisible();

    QList<AppInfo> m_allApps;
    QList<AppInfo> m_visibleApps;
    QHash<QString, AppInfo> m_byPath;
    QString m_filterText;
};

#endif
