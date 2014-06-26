#include "ThreadUtils.h"

#include <QThread>
#include <QCoreApplication>

namespace mp {

bool ThreadUtils::IsCurrentThreadGui()
{
    const bool isGuiThread = 
        QThread::currentThread() == QCoreApplication::instance()->thread();

	return isGuiThread;
}

bool ThreadUtils::IsCurrentThreadOwn(QObject* obj)
{
    const bool isGuiThread = 
        QThread::currentThread() == obj->thread();

	return isGuiThread;
}

}