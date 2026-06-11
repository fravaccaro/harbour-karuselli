#ifndef PARTNERSTORE_H
#define PARTNERSTORE_H

#include <QStringList>

class PartnerStore
{
public:
    static const char *dconfKey();

    QStringList loadPaths() const;
    bool savePaths(const QStringList &paths) const;
    bool restartHomescreen() const;

private:
    static QString formatDconfArray(const QStringList &paths);
    static QStringList parseDconfArray(const QString &output);
    static bool runProcess(const QString &program, const QStringList &arguments, QByteArray *output = nullptr);
};

#endif
