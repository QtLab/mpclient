#ifndef MP_MEDIA_CONTROLLER_H
#define MP_MEDIA_CONTROLLER_H

#include <QObject>
#include <QTimer>
#include <QSharedPointer>
#include "bass.h"

namespace mp {

class ChannelMetadata;

// Play http radio stations, or local file
// "file://c:\\1.mp3" - path for play lcoal file

class AudioStream : public QObject
{
	Q_OBJECT

public:
	enum ASState
	{
		ASPlaying,
		ASPaused,
		ASStopped
	};

	AudioStream();
	virtual ~AudioStream();

	ASState State() const;

	void SetUrl(const QString& url);
	QString Url() const;
	bool IsPlaying() const;
	void Play(bool resume = false);
	void Pause();
	void Stop();
	void SetVolume(float volume);
	float GetVolume() const;
	void GetMetaData(ChannelMetadata& meta);

private:
	ASState								m_state;
	// BASS library channel
	HSTREAM								m_hStream;
	// Current play url
	QString								m_currentUrl;
	//
	QString								m_;
	// 
	QTimer								m_metadataTimer;
};

typedef QSharedPointer<AudioStream> AudioStreamPtr;

}

#endif