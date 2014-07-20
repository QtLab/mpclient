#ifndef MP_RADIO_PAGE_CONTROLLER_H
#define MP_RADIO_PAGE_CONTROLLER_H

#include "IPageController.h"
#include "AudioStream.h"

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
	void Pause();

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
	model::RadioSourcePtr							m_currentChannel;
	model::RadioCategoryPtr							m_currentCategory;
	// Model for all stations channnels
	model::RadioSourcesModelPtr						m_stations;
	model::RadioSourcesSortFilterProxyModelPtr		m_searchStationsProxyModel;
	model::RadioSourcesSortFilterProxyModelPtr		m_allStationsProxyModel;
	model::RadioSourcesSortFilterProxyModelPtr		m_topStationsProxyModel;
	model::RadioSourcesSortFilterProxyModelPtr		m_lastStationsProxyModel;
	// All genres
	model::RadioCategoriesModelPtr					m_categories;
	// Widget view
	view::RadioPage*								m_view;
	// Audio stream manager
	AudioStream										m_audioStream;
};

} //namespace controller
} //namespace mp

#endif