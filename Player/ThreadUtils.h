#ifndef MP_THREAD_UTILS_H
#define MP_THREAD_UTILS_H

#include <QObject>

namespace mp {

class ThreadUtils
{
public:
	static bool IsCurrentThreadGui();
	static bool IsCurrentThreadOwn(QObject* obj);
};

}

#endif