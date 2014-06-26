#ifndef MP_AUDIO_STREAM_ASYNC_STARTER_H
#define MP_AUDIO_STREAM_ASYNC_STARTER_H

#include <QObject>
#include <QRunnable>
#include "bass.h"

namespace mp {

class AudioStreamAsyncStarter: public QObject, public QRunnable
{
     Q_OBJECT
public:
	AudioStreamAsyncStarter();
	void SetUrl(const QString& url);
	volatile bool IsRunning() const { return m_running; }

private:
	void run();
	
signals:
    void Ready(HSTREAM stream, int errorCode);

private:
	QMutex					m_mutex;
	volatile bool			m_running;
	QString					m_url;
};

}

#endif