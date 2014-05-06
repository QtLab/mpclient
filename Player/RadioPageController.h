#ifndef MP_RADIO_PAGE_CONTROLLER_H
#define MP_RADIO_PAGE_CONTROLLER_H

#include "Prerequirements.h"
#include "ChannelSourceModel.h"
#include "CategoriesModel.h"
#include "ChannelSourceSortFilterProxyModel.h"

#include <QTimer>

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
	void PlayRadio(int id);
	void PauseRadio();
	void VolumeChanged(qreal value);
	void CategoryChanged(int id);
	void SearchFilterChanged(QString seasrch);
	void UpdateViewState();

private:
	// Current playing channel
	ChannelSourcePtr					m_currentChannel;
	// Model for all stations channnels
	ChannelSourceModel					m_stations;
	ChannelSourceSortFilterProxyModel	m_searchStationsProxyModel;
	ChannelSourceSortFilterProxyModel	m_allStationsProxyModel;
	ChannelSourceSortFilterProxyModel	m_topStationsProxyModel;
	ChannelSourceSortFilterProxyModel	m_lastStationsProxyModel;
	// All genres
	CategoriesModel						m_categories;
	// Widget view
	RadioPage*							m_view;
	// Audio stream manager
	AudioStreamPtr						m_audioStream;
	QTimer								m_updateStateTimer;
};

}

#endif