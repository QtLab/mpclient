#ifndef ZAXAR_CHANNEL_METDATA_MODEL_H
#define ZAXAR_CHANNEL_METDATA_MODEL_H

#include <QMetaType>
#include <QObject>
#include <QSharedPointer>

namespace mp {

class ChannelMetadata : public QObject
{
	Q_OBJECT
	Q_PROPERTY(QString title READ Title WRITE SetTitle)
	Q_PROPERTY(QString artist READ Artist WRITE SetArtist)
	Q_PROPERTY(QString url READ Url WRITE SetUrl)

public:
	ChannelMetadata();
	~ChannelMetadata();

	const QString& Title() const;
	void SetTitle(const QString& title);

	const QString& Artist() const;
	void SetArtist(const QString& artist);

	const QString& Url() const;
	void SetUrl(const QString& url);

	QString ToString() const;

private:
	QString					m_title;
	QString					m_artist;
	QString					m_url;

	Q_DISABLE_COPY(ChannelMetadata)
};

typedef QSharedPointer<ChannelMetadata> ChannelMetadataPtr;
Q_DECLARE_METATYPE(ChannelMetadataPtr);

}

#endif