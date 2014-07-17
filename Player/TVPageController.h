#ifndef MP_TV_PAGE_CONTROLLER_H
#define MP_TV_PAGE_CONTROLLER_H

#include "IPageController.h"

namespace mp {
namespace controller {

class TVPageController : public IPageController
{
	Q_OBJECT

public:	
	TVPageController();
	~TVPageController();

	bool IsActive() const;
	view::TabPage* View() const;
	void ReLoadData();
	void Search(const QString& filter);
	void Pause();

private:
	void SetCurentCategory(model::TVCategoryPtr category);

private slots:
	void TVCurrentCategoryChanged(int categoryId);
	void ShowGenres(int id);
	void TVCurrentGenreChanged(int genreId);
	void SearchFilterChanged(QString filter);
	void GotoTVSource(int id);

signals:
	void FlashInstalled();

private:
	// Widget view
	view::TVPage *								m_view;
	view::TVGenresPopupPtr						m_genresPopup;
	// TV Cateories model
	model::TVCategoryPtr						m_currentCategory;
	model::TVCategoriesModelPtr					m_categoriesModel;
	// TV Genres model
	model::TVGenrePtr							m_currentTVGenre;
	model::TVGenresModelPtr						m_genresModel;
	model::TVGenresSortFilterProxyModelPtr		m_genresProxyModel;
	// TV sources model
	model::TVSourcePtr							m_currentTVSource;
	model::TVSourcesModelPtr					m_sourcesModel;
	model::TVSourcesSortFilterProxyModelPtr		m_currentGenreSourcesModel;
	model::TVSourcesSortFilterProxyModelPtr		m_searchSourcesModel;
	model::TVSourcesSortFilterProxyModelPtr		m_topSourcesModel;
	//Banners model
	model::BannersModelPtr						m_bannersModel;
};

} //namespace controller
} //namespace mp

#endif