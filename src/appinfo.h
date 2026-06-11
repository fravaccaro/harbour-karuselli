#ifndef APPINFO_H
#define APPINFO_H

#include <QString>

struct AppInfo
{
    QString name;
    QString desktopPath;
    QString iconSource;
    bool isAndroid = false;
    bool missing = false;
};

#endif
