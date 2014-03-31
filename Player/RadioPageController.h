#ifndef MP_RADIO_PAGE_CONTROLLER_H
#define MP_RADIO_PAGE_CONTROLLER_H

#include "Prerequirements.h"
#include "ChannelSourceModel.h"
#include "GenreModel.h"

namespace mp {

class RadioPageController : public QObject
{
	Q_OBJECT

public:	
	RadioPageController();
	~RadioPageController();

	void ReLoadData();
	TabPage* View();

public slots:
	void PlayRadio(const QString& id);
	void PauseRadio();
	void GenreChanged(const QString& id);
	void TopStationslUpdated();
	void LastStationsUpdated(ChannelSourcePtr newchannel);

private:
	// All stations channnels
	ChannelSourceModel				m_stations;
	// Statinons of the cureent genre
	ChannelSourceModel				m_currentSations;
	// Last stations
	ChannelSourceModel				m_lastSations;
	// Top stations
	ChannelSourceModel				m_topSations;
	// All genres
	GenreModel						m_radioGenres;
	// Widget view
	RadioPage*						m_view;
	// Audio stream manager
	AudioStreamPtr					m_audioStream;
};

}

#endif