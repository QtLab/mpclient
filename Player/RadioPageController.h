#ifndef MP_RADIO_PAGE_CONTROLLER_H
#define MP_RADIO_PAGE_CONTROLLER_H

#include "IPageController.h"
#include "AudioStream.h"
#include "CategoriesModel.h"
#include "ChannelSourceModel.h"
#include "ChannelSourceSortFilterProxyModel.h"

namespace mp {
namespace controller {

class RadioPageController : public IPageController
{
	Q_OBJECT

public:	
	RadioPageController();

	bool IsActive() const;
	view::TabPage* View() const;
	void ReLoadData();
	void Search(const QString& filter);
	void Stop();

private:
	void SetCategoryFilter(int categoryId);

private slots:
	void PlayRadio(int id);
	void ResumeRadio();
	void PauseRadio();
	void VolumeChanged(qreal value);
	void CategoryChanged(int id);
	void SearchFilterChanged(QString seasrch);
	void MetadataUpdated(const ChannelMetadata& meta);
	void AudioStreamStateChanged(AudioStream::ASState newState);
	
signals:
	void SearchTracks(QString filter);

private:
	// Current playing channel
	model::ChannelSourcePtr						m_currentChannel;
	model::CategoryPtr							m_currentCategory;
	// Model for all stations channnels
	model::ChannelSourceModel					m_stations;
	model::ChannelSourceSortFilterProxyModel	m_searchStationsProxyModel;
	model::ChannelSourceSortFilterProxyModel	m_allStationsProxyModel;
	model::ChannelSourceSortFilterProxyModel	m_topStationsProxyModel;
	model::ChannelSourceSortFilterProxyModel	m_lastStationsProxyModel;
	// All genres
	model::CategoriesModel						m_categories;
	// Widget view
	view::RadioPage*							m_view;
	// Audio stream manager
	AudioStream									m_audioStream;
};

}
}

#endif