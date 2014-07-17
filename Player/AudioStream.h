#ifndef MP_AUDIO_STREAM_H
#define MP_AUDIO_STREAM_H

#include <QObject>
#include "AudioStreamAsyncStarter.h"

namespace mp {

class ChannelMetadata;

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

	AudioStream(const  QString& streamName);
	virtual ~AudioStream();

	static void InitGlobal();

	ASState SyncState();
	ASState State() const;
	const QString& Name() const;

	bool IsPlaying() const;
	void Play(const QString& url);
	void Resume();
	void Pause();
	void Stop();
	long GetCurrentPos() const;
	long GetLength() const;
	void SetCurrentPos(long currentPos);

private:
	static void CALLBACK ProcessMetaCallback(HSYNC handle, DWORD channel, DWORD data, void *user); 
	static void CALLBACK ProcessStreamStalled(HSYNC handle, DWORD channel, DWORD data, void *user);
	
private slots:
	void EmitStateChanged(AudioStream::ASState newState);
	void ProcessMeta();
	void CleanupStream();
	void VolumeChanged(qreal value, const QString& streamName);
	void StreamStarted(HSTREAM stream, int errorCode);

signals:
	void StateChanged(AudioStream::ASState newState);
	void MetadataUpdated(const ChannelMetadata& meta);
	void StreamSarted(HSTREAM stream, int errorCode);

private:
	QString								m_streamName;
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