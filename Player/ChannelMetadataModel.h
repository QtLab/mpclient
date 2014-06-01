#ifndef MP_CHANNEL_METDATA_MODEL_H
#define MP_CHANNEL_METDATA_MODEL_H

#include <QMetaType>
#include <QObject>
#include <QSharedPointer>

namespace mp {

class ChannelMetadata : public QObject
{
	Q_OBJECT
	Q_PROPERTY(QString title READ Title WRITE SetTitle)
	Q_PROPERTY(QString artist READ Artist WRITE SetArtist)

public:
	ChannelMetadata();
	~ChannelMetadata();

	const QString& Title() const;
	void SetTitle(const QString& title);

	const QString& Artist() const;
	void SetArtist(const QString& artist);

	bool IsEmpty() const;

	QString ToString() const;

private:
	QString					m_title;
	QString					m_artist;
	QString					m_url;

	Q_DISABLE_COPY(ChannelMetadata)
};

typedef QSharedPointer<ChannelMetadata> ChannelMetadataPtr;

}

#endif