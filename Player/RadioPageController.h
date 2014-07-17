#ifndef MP_RADIO_PAGE_CONTROLLER_H
#define MP_RADIO_PAGE_CONTROLLER_H

#include "IPageController.h"
#include "AudioStream.h"
#include "RadioCategoriesModel.h"
#include "RadioSourcesModel.h"
#include "RadioSourcesSortFilterProxyModel.h"

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
	model::RadioSourcePtr						m_currentChannel;
	model::RadioCategoryPtr						m_currentCategory;
	// Model for all stations channnels
	model::RadioSourcesModel					m_stations;
	model::RadioSourcesSortFilterProxyModel		m_searchStationsProxyModel;
	model::RadioSourcesSortFilterProxyModel		m_allStationsProxyModel;
	model::RadioSourcesSortFilterProxyModel		m_topStationsProxyModel;
	model::RadioSourcesSortFilterProxyModel		m_lastStationsProxyModel;
	// All genres
	model::RadioCategoriesModel					m_categories;
	// Widget view
	view::RadioPage*							m_view;
	// Audio stream manager
	AudioStream									m_audioStream;
};

} //namespace controller
} //namespace mp

#endif