#include "AudioStreamAsyncStarter.h"

namespace mp {

AudioStreamAsyncStarter::AudioStreamAsyncStarter()
	:m_running(false)
{
	setAutoDelete(false);
}

void AudioStreamAsyncStarter::SetUrl(const QString& url)
{
	QMutexLocker locker(&m_mutex);
	m_url = url;
}

void AudioStreamAsyncStarter::run()
{
	HSTREAM	stream = 0;

	m_running = true;

	forever
	{
		BASS_StreamFree(stream);

		std::string url;
		{
			QMutexLocker locker(&m_mutex);
			url = m_url.toStdString();
			m_url.clear();
		}


		stream = BASS_StreamCreateURL(url.c_str(), 0, BASS_STREAM_BLOCK|BASS_STREAM_STATUS|BASS_STREAM_AUTOFREE, NULL, NULL); // open URL

		if(stream)
		{
			BASS_ChannelPlay(stream, TRUE);
		}

		{
			QMutexLocker locker(&m_mutex);
			if(m_url.isEmpty())
			{
				break;
			}
		}
	}

	m_running = false;

	emit Ready(stream, BASS_ErrorGetCode());
}

}