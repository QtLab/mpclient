#include "AudioStream.h"
#include "ChannelMetadataModel.h"

#include <QDebug>

#ifdef Q_OS_WIN32
	#include <windows.h>
	#include <process.h>
#endif

namespace mp {

AudioStream::AudioStream()
	:m_hStream(0)
	,m_state(ASStopped)
{
	if (!BASS_Init(-1,44100, 0, 0, NULL)) 
	{
		qDebug() << "BASS_Init: can't initialize device, error code: " << BASS_ErrorGetCode();
	}
	else
	{
		BASS_SetConfig(BASS_CONFIG_NET_PLAYLIST, 1); // enable playlist processing
		BASS_SetConfig(BASS_CONFIG_NET_PREBUF, 0); // minimize automatic pre-buffering, so we can do it (and display it) instead

		qDebug() << "BASS_Init: success";
	}
}

AudioStream::~AudioStream()
{
	BASS_Free();
}

AudioStream::ASState AudioStream::State() const
{
	return m_state;
}

void CALLBACK StatusProc(const void *buffer, DWORD length, void *user)
{
}

void AudioStream::SetUrl(const QString& url)
{
	Stop();
	m_currentUrl = url;
}

QString AudioStream::Url() const
{
	return m_currentUrl;
}

bool AudioStream::IsPlaying() const
{
	return m_state == ASPlaying;
}

void AudioStream::Play(bool resume)
{
	if(m_currentUrl.isEmpty())
	{
		qDebug() << "AudioStreamController::Play cuurrent channel is null!";
		m_state = ASStopped;
	}
	else
	{
		if(!resume)
		{
			std::string url = m_currentUrl.toStdString();
			BASS_StreamFree(m_hStream);
			m_hStream = BASS_StreamCreateURL(url.c_str(), 0, BASS_STREAM_BLOCK|BASS_STREAM_STATUS|BASS_STREAM_AUTOFREE, StatusProc, NULL); // open URL
		}

		if(m_hStream)
		{
			BASS_ChannelPlay(m_hStream, FALSE);

			int errCode = BASS_ErrorGetCode();

			if(errCode != 0)
			{
				m_state = ASStopped;
				qDebug() << "AudioStreamController::GetVolume error code: " << errCode;
			}
			else
			{
				m_state = ASPlaying;
			}
		}
		else
		{
			m_state = ASStopped;
			qDebug() << "BASS_Init: can't initialize device, error code: " << BASS_ErrorGetCode();
		}
	}
}

void AudioStream::Pause()
{
	if(m_hStream)
	{
		BASS_ChannelPause(m_hStream);

		int errCode = BASS_ErrorGetCode();

		if(errCode != 0)
		{
			m_state = ASStopped;
			qDebug() << "AudioStreamController::Pause error code: " << errCode;
		}
		else
		{
			m_state = ASPaused;
		}
	}
}

void AudioStream::Stop()
{
	BASS_StreamFree(m_hStream);

	int errCode = BASS_ErrorGetCode();

	if(errCode != 0)
	{
		qDebug() << "AudioStreamController::Stop error code: " << errCode;
	}

	m_hStream = 0;
	m_currentUrl.clear();

	m_state = ASStopped;
}

void AudioStream::SetVolume(float volume)
{
	if(!BASS_SetVolume(volume))
	{
		qDebug() << "AudioStreamController::SetVolume error code: " << BASS_ErrorGetCode();
	}
}

float AudioStream::GetVolume() const
{
	float volume = BASS_GetVolume();

	int errCode = BASS_ErrorGetCode();

	if(errCode != 0)
	{
		qDebug() << "AudioStreamController::GetVolume error code: " << errCode;
	}

	return volume;
}

void AudioStream::GetMetaData(ChannelMetadata& metadata)
{
	if(m_hStream)
	{
		const char *meta=BASS_ChannelGetTags(m_hStream, BASS_TAG_META);
		if (meta) 
		{ 
			// got Shoutcast metadata
			const char *p=strstr(meta,"StreamTitle='"); // locate the title
			if (p) 
			{
				const char *p2=strstr(p,"';"); // locate the end of it
				if (p2) 
				{
					char *t=strdup(p+13);
					t[p2-(p+13)]=0;
					metadata.SetTitle(t);
					free(t);
				}
			}
		} 
		else 
		{ 
			meta=BASS_ChannelGetTags(m_hStream, BASS_TAG_OGG);
			if (meta) 
			{ 
				// got Icecast/OGG tags
				const char *artist=NULL,*title=NULL,*p=meta;
				for (;*p;p+=strlen(p)+1) 
				{
					if (!strnicmp(p,"artist=",7)) // found the artist
						artist=p+7;
					if (!strnicmp(p,"title=",6)) // found the title
						title=p+6;
				}
				if (title) 
				{
					if (artist) 
					{
						metadata.SetTitle(title);
						metadata.SetArtist(artist);
					}
					else
					{
						metadata.SetTitle(title);
					}	
				}
			}
			else
			{
				metadata.SetUrl(m_currentUrl);
			}
		}
	}
}

}
