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

		std::wstring url;
		{
			QMutexLocker locker(&m_mutex);
			url = (const wchar_t *)m_url.utf16();// windows wide encoding is utf16
			m_url.clear();
		}

		// try streaming the file/url
		stream = BASS_StreamCreateFile(FALSE, url.c_str(), 0, 0, BASS_UNICODE);

		if(!stream)
		{
			stream = BASS_StreamCreateURL(url.c_str(), 0, BASS_STREAM_STATUS|BASS_STREAM_AUTOFREE|BASS_UNICODE, NULL, NULL); // open URL
		}

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