#ifndef DESKTOPPARSER_H
#define DESKTOPPARSER_H

#include "appinfo.h"

#include <QString>

bool parseDesktopFile(const QString &path, AppInfo *info);

#endif
