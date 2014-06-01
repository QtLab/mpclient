#include "AudioStream.h"
#include "ChannelMetadataModel.h"
#include "ThreadUtils.h"
#include "Config.h"

#include <QDebug>
#include <QTextCodec>
#include <QCoreApplication>

#ifdef Q_OS_WIN32
	#include <windows.h>
	#include <process.h>
#endif

namespace mp {

AudioStream::AudioStream()
	:m_hStream(0)
	,m_state(ASStopped)
{
	qRegisterMetaType<AudioStream::ASState>("AudioStream::ASState");
	qRegisterMetaType<HSTREAM>("HSTREAM");
	
	if (!BASS_Init(-1,44100, 0, 0, NULL)) 
	{
		qDebug() << "BASS_Init: can't initialize device, error code: " << BASS_ErrorGetCode();
	}
	else
	{
		BASS_SetConfig(BASS_CONFIG_NET_PLAYLIST, 1); // enable playlist processing
		BASS_SetConfig(BASS_CONFIG_NET_PREBUF, 0); // minimize automatic pre-buffering, so we can do it (and display it) instead
		BASS_SetConfig(BASS_CONFIG_UNICODE, TRUE);

		qDebug() << "BASS_Init: success";
	}

	connect(&Config::Inst(), SIGNAL(VolumeChanged(qreal)), SLOT(VolumeChanged(qreal)));
	connect(&m_streamStarter, SIGNAL(Ready(HSTREAM, int)), SLOT(StreadStarted(HSTREAM, int)), Qt::DirectConnection);
}

AudioStream::~AudioStream()
{
	BASS_Free();
}

AudioStream::ASState AudioStream::State() const
{
	return m_state;
}

bool AudioStream::IsPlaying() const
{
	return m_state == ASPlaying;
}

void AudioStream::Play(const QString& url)
{
	m_streamStarter.SetUrl(url);

	if(!m_streamStarter.IsRunning())
	{
		EmitStateChanged(AudioStream::ASStarting);
		CleanupStream();
		QThreadPool::globalInstance()->start(&m_streamStarter, 0);
	}
}

void AudioStream::StreadStarted(HSTREAM stream, int errorCode)
{
	if(ThreadUtils::IsGuiThread())
	{
		if(stream != 0 && errorCode == 0)
		{
			m_hStream = stream;
			BASS_ChannelSetSync(m_hStream, BASS_SYNC_META, 0, &AudioStream::ProcessMetaCallback, this); // Shoutcast
			BASS_ChannelSetSync(m_hStream, BASS_SYNC_OGG_CHANGE, 0, &AudioStream::ProcessMetaCallback, this); // Icecast/OGG
			BASS_ChannelSetSync(m_hStream, BASS_SYNC_STALL | BASS_SYNC_MIXTIME, 0, &AudioStream::ProcessStreamStalled, this);

			VolumeChanged(Config::Inst().Volume());
			EmitStateChanged(AudioStream::ASPlaying);
		}
		else
		{
			qDebug() << "AudioStreamController::Play error code: " << errorCode;

			EmitStateChanged(AudioStream::ASStopped);
		}
	}
	else
	{
		QMetaObject::invokeMethod(this, "StreadStarted", Qt::QueuedConnection, 
									Q_ARG(HSTREAM, stream), Q_ARG(int, errorCode));
	}
}

void AudioStream::Resume()
{
	VolumeChanged(Config::Inst().Volume());
	BASS_ChannelPlay(m_hStream, TRUE);

	int errCode = BASS_ErrorGetCode();

	ASState newState = ASPlaying;

	if(errCode != 0)
	{
		newState = ASStopped;
		qDebug() << "AudioStreamController::Resume error code: " << errCode;
	}

	EmitStateChanged(newState);
}

void AudioStream::Pause()
{
	if(m_hStream)
	{
		BASS_ChannelPause(m_hStream);

		int errCode = BASS_ErrorGetCode();

		ASState newState = ASPaused;

		if(errCode != 0)
		{
			newState = ASStopped;
			qDebug() << "AudioStreamController::Pause error code: " << errCode;
		}

		EmitStateChanged(newState);
	}
}

void AudioStream::Stop()
{
	CleanupStream();

	int errCode = BASS_ErrorGetCode();

	if(errCode != 0)
	{
		qDebug() << "AudioStreamController::Stop error code: " << errCode;
	}

	EmitStateChanged(ASStopped);
}

void AudioStream::VolumeChanged(qreal value)
{
	if(m_hStream)
	{
		if(!BASS_ChannelSetAttribute(m_hStream, BASS_ATTRIB_VOL, value))
		{
			int errCode = BASS_ErrorGetCode();

			if(errCode != 0)
			{
				qDebug() << "AudioStreamController::GetVolume error code: " << errCode;
			}
		}
	}
}

void AudioStream::EmitStateChanged(ASState newState)
{
	m_state = newState;
	emit StateChanged(m_state);
}

QString ToUnicode(const char * input)
{
	QTextCodec::ConverterState state;
	QTextCodec *codec = QTextCodec::codecForName("UTF-8");
	QString meta = codec->toUnicode(input, strlen(input), &state);

	if (state.invalidChars > 0)
	{
		meta = QString::fromLocal8Bit(input);
	}

	return meta;
}

void AudioStream::ProcessMeta()
{
	if(m_hStream)
	{
		ChannelMetadata metadata;

		QString meta = ToUnicode(BASS_ChannelGetTags(m_hStream, BASS_TAG_META));

		if (!meta.isEmpty()) 
		{
			if(meta.indexOf("StreamTitle='") > -1)
			{
				meta = meta.remove(0, 13);
				int index = meta.indexOf("';", 0);
				meta = meta.mid(0, index);
				metadata.SetTitle(meta);
			}
		} 
		else 
		{ 
			meta = ToUnicode(BASS_ChannelGetTags(m_hStream, BASS_TAG_OGG));
			if (!meta.isEmpty()) 
			{
				//TODO: parse artist, title
				metadata.SetTitle(meta);
			}
		}

		emit MetadataUpdated(metadata);
	}
}

void AudioStream::CleanupStream()
{
	BASS_StreamFree(m_hStream);
	m_hStream = 0;
}

void AudioStream::ProcessMetaCallback(HSYNC handle, DWORD channel, DWORD data, void *user)
{
	AudioStream * stream =  reinterpret_cast<AudioStream*>(user);
	QMetaObject::invokeMethod( stream, "ProcessMeta", Qt::QueuedConnection);
}

void AudioStream::ProcessStreamStalled(HSYNC handle, DWORD channel, DWORD data, void *user)
{
	AudioStream * stream =  reinterpret_cast<AudioStream*>(user);
	QMetaObject::invokeMethod( stream, "EmitStateChanged", Qt::QueuedConnection,
							   Q_ARG( AudioStream::ASState, ASStopped ) );
}

}
