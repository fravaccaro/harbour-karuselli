#include "desktopparser.h"

#include <QFile>
#include <QFileInfo>

bool parseDesktopFile(const QString &path, AppInfo *info)
{
    if (!info)
        return false;

    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    QString type;
    QString name;
    QString icon;
    QString noDisplay;
    QString hidden;
    bool hasApkd = false;

    while (!file.atEnd()) {
        const QString line = QString::fromUtf8(file.readLine()).trimmed();
        if (line.isEmpty() || line.startsWith('#'))
            continue;

        const int eq = line.indexOf('=');
        if (eq < 0)
            continue;

        const QString key = line.left(eq).trimmed();
        const QString value = line.mid(eq + 1).trimmed();

        if (key == QLatin1String("Type"))
            type = value;
        else if (key == QLatin1String("Name"))
            name = value;
        else if (key == QLatin1String("Icon"))
            icon = value;
        else if (key == QLatin1String("NoDisplay"))
            noDisplay = value;
        else if (key == QLatin1String("Hidden"))
            hidden = value;
        else if (key == QLatin1String("X-apkd-packageName"))
            hasApkd = true;
    }

    if (type != QLatin1String("Application"))
        return false;
    if (noDisplay.compare(QLatin1String("true"), Qt::CaseInsensitive) == 0)
        return false;
    if (hidden.compare(QLatin1String("true"), Qt::CaseInsensitive) == 0)
        return false;

    if (name.isEmpty())
        name = QFileInfo(path).completeBaseName();

    info->desktopPath = path;
    info->name = name;
    info->isAndroid = hasApkd
            || QFileInfo(path).fileName().startsWith(QLatin1String("apkd_launcher_"));
    info->missing = false;

    if (icon.startsWith('/'))
        info->iconSource = QStringLiteral("file://") + icon;
    else if (icon.isEmpty())
        info->iconSource = QStringLiteral("image://theme/icon-launcher-default");
    else
        info->iconSource = QStringLiteral("image://theme/") + icon;

    return true;
}
