#ifndef MP_RADIO_COMPOSITE_MODEL_H
#define MP_RADIO_COMPOSITE_MODEL_H

#include <QMetaType>
#include <QSharedPointer>
#include <QAbstractListModel>
#include <QReadWriteLock>
#include <QObject>

#include "GenreModel.h"
#include "ChannelSourceModel.h"

namespace mp {

class RadioCompositeModel : public QAbstractListModel
{
	Q_OBJECT

public:
	RadioCompositeModel();
	~RadioCompositeModel();

	//ChannelSourceModelPtr Channels();
	//GenreModelPtr Genres();

private:
	friend class TabPagesController;

	//ChannelSourceModelPtr				m_channels;
	//GenreModelPtr						m_genres;

	Q_DISABLE_COPY(RadioCompositeModel)
};

}

#endif