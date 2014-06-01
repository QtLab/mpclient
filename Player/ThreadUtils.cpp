#include "ThreadUtils.h"

#include <QThread>
#include <QCoreApplication>

namespace mp {

bool ThreadUtils::IsGuiThread()
{
    const bool isGuiThread = 
        QThread::currentThread() == QCoreApplication::instance()->thread();

	return isGuiThread;
}

}