#ifndef MP_LOG_H
#define MP_LOG_H

#include <QObject>

namespace mp {

void Log(QtMsgType type, const QMessageLogContext &, const QString & str);

}

#endif
