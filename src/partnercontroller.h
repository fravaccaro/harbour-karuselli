#ifndef PARTNERCONTROLLER_H
#define PARTNERCONTROLLER_H

#include "appmodel.h"
#include "partnerstore.h"
#include "selectedappsmodel.h"

#include <QObject>
#include <QStringList>

class PartnerController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(AppModel* appModel READ appModel CONSTANT)
    Q_PROPERTY(SelectedAppsModel* selectedAppsModel READ selectedAppsModel CONSTANT)
    Q_PROPERTY(bool partnerSpaceEnabled READ partnerSpaceEnabled WRITE setPartnerSpaceEnabled NOTIFY partnerSpaceEnabledChanged)
    Q_PROPERTY(bool dirty READ dirty NOTIFY dirtyChanged)
    Q_PROPERTY(int selectedCount READ selectedCount NOTIFY selectedCountChanged)
    Q_PROPERTY(bool partnerSpaceActive READ partnerSpaceActive NOTIFY partnerSpaceActiveChanged)
    Q_PROPERTY(bool showRestartHint READ showRestartHint NOTIFY showRestartHintChanged)
    Q_PROPERTY(int maxCount READ maxCount CONSTANT)
    Q_PROPERTY(int savedAppCount READ savedAppCount NOTIFY partnerSpaceActiveChanged)

public:
    explicit PartnerController(QObject *parent = nullptr);

    AppModel *appModel();
    SelectedAppsModel *selectedAppsModel();

    bool partnerSpaceEnabled() const;
    void setPartnerSpaceEnabled(bool enabled);

    bool dirty() const;
    int selectedCount() const;
    bool partnerSpaceActive() const;
    bool showRestartHint() const;
    int maxCount() const;
    int savedAppCount() const;

    Q_INVOKABLE void reload();
    Q_INVOKABLE bool apply();
    Q_INVOKABLE void removeApp(const QString &path);
    Q_INVOKABLE bool restartHomescreen();
    Q_INVOKABLE void dismissRestartHint();

signals:
    void partnerSpaceEnabledChanged();
    void dirtyChanged();
    void selectedCountChanged();
    void partnerSpaceActiveChanged();
    void showRestartHintChanged();
    void applyFailed();

private slots:
    void removeAppDeferred(const QString &path);

private:
    QStringList currentPathsForSave() const;
    void updateDirty();
    void connectModelSignals();

    AppModel m_appModel;
    SelectedAppsModel m_selectedAppsModel;
    PartnerStore m_store;

    bool m_enabled = false;
    bool m_savedEnabled = false;
    QStringList m_savedPaths;
    bool m_dirty = false;
    bool m_showRestartHint = false;
};

#endif
