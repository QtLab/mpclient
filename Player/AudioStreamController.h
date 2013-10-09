#ifndef MP_MEDIA_CONTROLLER_H
#define MP_MEDIA_CONTROLLER_H

#include <QObject>
#include <QTimer>
#include "bass.h"

namespace mp {

class ChannelMetadata;

// Play http radio stations, or local file
// "file://c:\\1.mp3" - path for play lcoal file

class AudioStreamController : public QObject
{
	Q_OBJECT

public:
	static AudioStreamController& Inst();

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
	AudioStreamController();
	~AudioStreamController();

private:
	// SIngleton
	static AudioStreamController*		m_instance;

	// BASS library channel
	HSTREAM								m_bassChannel;
	// Current play url
	QString								m_currentUrl;
	// Metadata updates on a timer
	QTimer								m_metadataTimer;
};

}

#endif