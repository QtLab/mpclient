#include "ChannelMetadataModel.h"

namespace mp {

ChannelMetadata::ChannelMetadata()
{
}

ChannelMetadata::~ChannelMetadata()
{
}

const QString& ChannelMetadata::Title() const
{
	return m_title;
}

void ChannelMetadata::SetTitle(const QString& title)
{
	m_title = title;
}

const QString& ChannelMetadata::Artist() const
{
	return m_artist;
}

void ChannelMetadata::SetArtist(const QString& artist)
{
	m_artist = artist;
}

const QString& ChannelMetadata::Url() const
{
	return m_url;
}

void ChannelMetadata::SetUrl(const QString& url)
{
	m_url = url;
}

QString ChannelMetadata::ToString() const
{
	QString result;

	if(!m_title.isEmpty())
	{
		result = m_title;

		if(!m_artist.isEmpty())
		{
			result = result + " - " + m_artist;
		}
	}
	else
	{
		result = m_url;
	}

	return result;
}

}