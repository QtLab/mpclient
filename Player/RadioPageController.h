#ifndef MP_RADIO_PAGE_CONTROLLER_H
#define MP_RADIO_PAGE_CONTROLLER_H

#include "ChannelSourceModel.h"
#include "GenreModel.h"
#include "TabModel.h"

namespace mp {

// Forward declaration
class TabPage;
class TabWidget;
class RadioPage;

class RadioPageController : public QObject
{
	Q_OBJECT

public:	
	static RadioPageController& Inst();
	void ReLoadData();
	TabPage* View();

public slots:
	void PlayRadio(const QString& id);
	void PauseRadio();
	void GenreChanged(const QString& id);
	void TopStationslUpdated();
	void LastStationsUpdated(ChannelSourcePtr newchannel);

private:
	RadioPageController();
	~RadioPageController();

private:
	static RadioPageController*		m_instance;
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
};

}

#endif