#ifndef MP_RADIO_PAGE_CONTROLLER_H
#define MP_RADIO_PAGE_CONTROLLER_H

#include "Prerequirements.h"
#include "AudioStream.h"
#include "CategoriesModel.h"
#include "ChannelSourceModel.h"
#include "ChannelSourceSortFilterProxyModel.h"

#include <QTimer>

namespace mp {

class RadioPageController : public QObject
{
	Q_OBJECT

public:	
	RadioPageController();
	~RadioPageController();

	bool IsRadioPlaying();

	void ReLoadData();
	TabPage* View();

private slots:
	void PlayRadio(int id);
	void ResumeRadio();
	void PauseRadio();
	void VolumeChanged(qreal value);
	void CategoryChanged(int id);
	void SearchFilterChanged(QString seasrch);
	void MetadataUpdated(const ChannelMetadata& meta);
	void AudioStreamStateChanged(AudioStream::ASState newState);

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
	AudioStream							m_audioStream;
	QTimer								m_updateStateTimer;
};

}

#endif