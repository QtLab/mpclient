#ifndef MP_THREAD_UTILS_H
#define MP_THREAD_UTILS_H

#include <QThread>
#include <QThreadPool>
namespace mp {

class ThreadUtils
{
public:
	static bool IsGuiThread();
};

}

#endif