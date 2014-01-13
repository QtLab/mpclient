#include "AudioStreamController.h"
#include "ChannelMetadataModel.h"

#include <QDebug>

#ifdef Q_OS_WIN32
	#include <windows.h>
	#include <process.h>
#endif

namespace mp {

AudioStreamController* AudioStreamController::m_instance = 0;

AudioStreamController& AudioStreamController::Inst()
{
	if(!m_instance)
		m_instance = new AudioStreamController();

	return *m_instance;
}

AudioStreamController::AudioStreamController()
	:m_bassChannel(0)
{
	m_metadataTimer.setSingleShot(false);
	m_metadataTimer.setInterval(2000); // 2 sec
	connect(&m_metadataTimer, SIGNAL(timeout()), SLOT(ProcessMeadata()));

	if (!BASS_Init(-1,44100,0, 0, NULL)) 
	{
		qDebug() << "BASS_Init: can't initialize device, error code: " << BASS_ErrorGetCode();
	}
	else
	{
		BASS_SetConfig(BASS_CONFIG_NET_PLAYLIST,1); // enable playlist processing
		BASS_SetConfig(BASS_CONFIG_NET_PREBUF,0); // minimize automatic pre-buffering, so we can do it (and display it) instead

		qDebug() << "BASS_Init: success";
	}
}

AudioStreamController::~AudioStreamController()
{
	BASS_Free();
}

void AudioStreamController::ProcessMeadata()
{
	if(m_bassChannel)
	{
		ChannelMetadata metadata;

		const char *meta=BASS_ChannelGetTags(m_bassChannel, BASS_TAG_META);
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
			meta=BASS_ChannelGetTags(m_bassChannel, BASS_TAG_OGG);
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

		emit MetadataUpdated(metadata);
	}
}

void CALLBACK StatusProc(const void *buffer, DWORD length, void *user)
{
}

void AudioStreamController::SetUrl(const QString& url)
{
	m_currentUrl = url;
}

QString AudioStreamController::Url() const
{
	return m_currentUrl;
}

void AudioStreamController::Play()
{
	if(m_currentUrl.isEmpty())
	{
		qDebug() << "AudioStreamController::Play cuurrent channel is null!";
	}
	else
	{
		std::string url = m_currentUrl.toStdString();
		BASS_StreamFree(m_bassChannel);
		m_bassChannel = BASS_StreamCreateURL(url.c_str(), 0, BASS_STREAM_BLOCK|BASS_STREAM_STATUS|BASS_STREAM_AUTOFREE, StatusProc, NULL); // open URL

		if(m_bassChannel)
		{
			BASS_ChannelPlay(m_bassChannel,FALSE);
			ProcessMeadata();
			m_metadataTimer.start();
		}
		else
		{
			qDebug() << "BASS_Init: can't initialize device, error code: " << BASS_ErrorGetCode();
		}
	}
}

void AudioStreamController::Pause()
{
	if(m_bassChannel)
	{
		m_metadataTimer.stop();
		BASS_ChannelPause(m_bassChannel);
	}
}

void AudioStreamController::Stop()
{
	m_metadataTimer.stop();
	BASS_StreamFree(m_bassChannel);
	m_bassChannel = 0;
	m_currentUrl.clear();
}

void AudioStreamController::SetVolume(float volume)
{
	if(!BASS_SetVolume(volume))
	{
		qDebug() << "AudioStreamController::SetVolume error code: " << BASS_ErrorGetCode();
	}
}

float AudioStreamController::GetVolume()
{
	float volume = BASS_GetVolume();
	return volume;
}

}
