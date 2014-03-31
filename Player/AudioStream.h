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
	AudioStream();
	virtual ~AudioStream();

	Q_INVOKABLE void SetUrl(const QString& url);
	Q_INVOKABLE QString Url() const;
	Q_INVOKABLE void Play();
	Q_INVOKABLE void Pause();
	void Stop();
	void SetVolume(float volume);
	float GetVolume();

signals:
	void MetadataUpdated(const ChannelMetadata& metaData);

private slots:
	void ProcessMeadata();

private:
	// BASS library channel
	HSTREAM								m_bassChannel;
	// Current play url
	QString								m_currentUrl;
	// Metadata updates on a timer
	QTimer								m_metadataTimer;
};

typedef QSharedPointer<AudioStream> AudioStreamPtr;

}

#endif