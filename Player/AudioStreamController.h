#ifndef MP_MEDIA_CONTROLLER_H
#define MP_MEDIA_CONTROLLER_H

#include <QObject>
#include <QTimer>
#include "bass.h"

namespace mp {

class ChannelMetadata;
class AudioStreamController : public QObject
{
	Q_OBJECT

public:
	static AudioStreamController& Inst();

	Q_INVOKABLE void SetUrl(const QString& url);
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
	static AudioStreamController*		m_instance;
	HSTREAM						m_bassChannel;
	QString						m_currentUrl;
	QTimer						m_metadataTimer;
};

}

#endif