#include "partnerstore.h"
#include "selectedappsmodel.h"

#include <QProcess>
#include <QRegularExpression>

const char *PartnerStore::dconfKey()
{
    return "/desktop/lipstick-jolla-home/partnerspace/applications";
}

QStringList PartnerStore::loadPaths() const
{
    QByteArray output;
    if (!runProcess(QStringLiteral("dconf"),
                    { QStringLiteral("read"), QString::fromUtf8(dconfKey()) },
                    &output)) {
        return QStringList();
    }

    return parseDconfArray(QString::fromUtf8(output));
}

bool PartnerStore::savePaths(const QStringList &paths) const
{
    QStringList limited = paths;
    while (limited.size() > SelectedAppsModel::kMaxCount)
        limited.removeLast();

    const QString value = formatDconfArray(limited);
    return runProcess(QStringLiteral("dconf"),
                      { QStringLiteral("write"), QString::fromUtf8(dconfKey()), value });
}

bool PartnerStore::restartHomescreen() const
{
    return runProcess(QStringLiteral("systemctl"),
                      { QStringLiteral("--user"), QStringLiteral("restart"), QStringLiteral("lipstick") });
}

QString PartnerStore::formatDconfArray(const QStringList &paths)
{
    if (paths.isEmpty())
        return QStringLiteral("@as []");

    QStringList quoted;
    quoted.reserve(paths.size());
    for (const QString &path : paths)
        quoted.append(QStringLiteral("'") + path + QStringLiteral("'"));

    return QStringLiteral("[") + quoted.join(QStringLiteral(", ")) + QStringLiteral("]");
}

QStringList PartnerStore::parseDconfArray(const QString &output)
{
    const QString trimmed = output.trimmed();
    if (trimmed.isEmpty() || trimmed == QStringLiteral("@as []"))
        return QStringList();

    QStringList paths;
    QRegularExpression re(QStringLiteral("'([^']*)'"));
    auto it = re.globalMatch(trimmed);
    while (it.hasNext()) {
        const QString path = it.next().captured(1);
        if (!path.isEmpty())
            paths.append(path);
    }
    return paths;
}

bool PartnerStore::runProcess(const QString &program, const QStringList &arguments, QByteArray *output)
{
    QProcess process;
    process.start(program, arguments);
    if (!process.waitForFinished(30000))
        return false;

    if (output)
        *output = process.readAllStandardOutput();

    return process.exitCode() == 0;
}
