#include "partnercontroller.h"

PartnerController::PartnerController(QObject *parent)
    : QObject(parent)
{
    connectModelSignals();
    reload();
}

AppModel *PartnerController::appModel()
{
    return &m_appModel;
}

SelectedAppsModel *PartnerController::selectedAppsModel()
{
    return &m_selectedAppsModel;
}

bool PartnerController::partnerSpaceEnabled() const
{
    return m_enabled;
}

void PartnerController::setPartnerSpaceEnabled(bool enabled)
{
    if (m_enabled == enabled)
        return;

    m_enabled = enabled;
    emit partnerSpaceEnabledChanged();
    updateDirty();
}

bool PartnerController::dirty() const
{
    return m_dirty;
}

int PartnerController::selectedCount() const
{
    return m_selectedAppsModel.count();
}

bool PartnerController::partnerSpaceActive() const
{
    return m_savedEnabled && !m_savedPaths.isEmpty();
}

bool PartnerController::showRestartHint() const
{
    return m_showRestartHint;
}

int PartnerController::maxCount() const
{
    return SelectedAppsModel::kMaxCount;
}

int PartnerController::savedAppCount() const
{
    return m_savedPaths.size();
}

void PartnerController::reload()
{
    m_savedPaths = m_store.loadPaths();
    m_savedEnabled = !m_savedPaths.isEmpty();
    m_enabled = m_savedEnabled;

    m_selectedAppsModel.loadFromPaths(m_savedPaths, &m_appModel);

    m_showRestartHint = false;
    emit partnerSpaceEnabledChanged();
    emit showRestartHintChanged();
    emit selectedCountChanged();
    emit partnerSpaceActiveChanged();
    updateDirty();
}

bool PartnerController::apply()
{
    const QStringList paths = currentPathsForSave();
    if (!m_store.savePaths(paths)) {
        emit applyFailed();
        return false;
    }

    m_savedPaths = paths;
    m_savedEnabled = m_enabled && !paths.isEmpty();

    if (!m_enabled)
        m_selectedAppsModel.clear();

    m_showRestartHint = true;
    emit showRestartHintChanged();
    emit partnerSpaceActiveChanged();
    updateDirty();
    return true;
}

void PartnerController::removeApp(const QString &path)
{
    if (path.isEmpty())
        return;

    QMetaObject::invokeMethod(this, "removeAppDeferred", Qt::QueuedConnection,
                              Q_ARG(QString, path));
}

void PartnerController::removeAppDeferred(const QString &path)
{
    m_selectedAppsModel.removeByPath(path);
}

bool PartnerController::restartHomescreen()
{
    return m_store.restartHomescreen();
}

void PartnerController::dismissRestartHint()
{
    if (!m_showRestartHint)
        return;

    m_showRestartHint = false;
    emit showRestartHintChanged();
}

QStringList PartnerController::currentPathsForSave() const
{
    if (!m_enabled)
        return QStringList();

    return m_selectedAppsModel.desktopPaths();
}

void PartnerController::updateDirty()
{
    const QStringList current = currentPathsForSave();
    const bool savedActive = m_savedEnabled && !m_savedPaths.isEmpty();
    const bool currentActive = m_enabled && !current.isEmpty();

    const bool newDirty = (currentActive != savedActive) || (current != m_savedPaths);

    if (m_dirty == newDirty)
        return;

    m_dirty = newDirty;
    emit dirtyChanged();
}

void PartnerController::connectModelSignals()
{
    connect(&m_selectedAppsModel, &SelectedAppsModel::changed, this, [this]() {
        emit selectedCountChanged();
        updateDirty();
    });
}
