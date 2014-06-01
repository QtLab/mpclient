#ifndef MP_AUDIO_STREAM_H
#define MP_AUDIO_STREAM_H

#include <QObject>
#include "AudioStreamAsyncStarter.h"

namespace mp {

class ChannelMetadata;

// Play http radio stations, or local file
// "file://c:\\1.mp3" - path for play lcoal file

class AudioStream : public QObject
{
	Q_OBJECT

public:
	Q_ENUMS(ASState)
	enum ASState
	{
		ASStarting,
		ASPlaying,
		ASPaused,
		ASStopped
	};

	AudioStream();
	virtual ~AudioStream();

	ASState State() const;

	bool IsPlaying() const;
	void Play(const QString& url);
	void Resume();
	void Pause();
	void Stop();

private:
	static void __stdcall ProcessMetaCallback(HSYNC handle, DWORD channel, DWORD data, void *user); 
	static void __stdcall ProcessStreamStalled(HSYNC handle, DWORD channel, DWORD data, void *user); 

private slots:
	void EmitStateChanged(AudioStream::ASState newState);
	void ProcessMeta();
	void CleanupStream();
	void VolumeChanged(qreal value);
	void StreadStarted(HSTREAM stream, int errorCode);

signals:
	void StateChanged(AudioStream::ASState newState);
	void MetadataUpdated(const ChannelMetadata& meta);
	void StreamSarted(HSTREAM stream, int errorCode);

private:
	// Async stream starter
	AudioStreamAsyncStarter				m_streamStarter;
	// Current state
	ASState								m_state;
	// BASS library channel
	HSTREAM								m_hStream;
	// Url that will be playing
	QString								m_urlToPlay;
};

Q_DECLARE_METATYPE(AudioStream::ASState)
Q_DECLARE_METATYPE(HSTREAM)

}

#endif