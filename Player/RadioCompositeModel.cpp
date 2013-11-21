#include "RadioCompositeModel.h"

#include <QFile>
#include <QDir>
#include <QHash>
#include <QDebug>
#include <QJson/Parser>
#include <QJson/Serializer>
#include <QJson/QObjectHelper>
#include <QCryptographicHash>

namespace mp {

RadioCompositeModel::RadioCompositeModel()
	//:m_channels(new ChannelSourceModel())
	//,m_genres(new GenreModel())
{
	//m_genres->Load("..//radiogenres.json");
	//m_channels->Load("..//radio.json");
}

RadioCompositeModel::~RadioCompositeModel()
{
}

/*
ChannelSourceModelPtr RadioCompositeModel::Channels()
{
	return m_channels;
}

GenreModelPtr RadioCompositeModel::Genres()
{
	return m_genres;
}
*/
}