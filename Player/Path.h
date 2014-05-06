#ifndef MP_LOG_H
#define MP_LOG_H

#include <QObject>

namespace mp {

QString QmlFilePath(const QString& fileName);
QString CssFilePath(const QString& fileName);
QString HtmlilePath(const QString& fileName);
QString ConfigFilePath(const QString& fileName);
QString HttpCachePath();

}

#endif
