#include "TVPageController.h"
#include "TVCategoriesModel.h"
#include "TVGenresModel.h"
#include "TVGenresSortFilterProxyModel.h"
#include "TVSourcesModel.h"
#include "TVSourcesSortFilterProxyModel.h"
#include "BannersModel.h"
#include "TVPage.h"
#include "TVGenresPopup.h"
#include "Path.h"

namespace mp {
namespace controller {

const int GenresPopupTopMagrin		= 70;
const int GenresPopupLeftMagrin		= 11;

TVPageController::TVPageController()
	:m_genresPopup(NULL)
{
	m_categoriesModel = new model::TVCategoriesModel(this);
	m_genresModel = new model::TVGenresModel(this);

	m_genresProxyModel = new model::TVGenresSortFilterProxyModel(this);
	m_genresProxyModel->setSourceModel(m_genresModel);	

	m_sourcesModel = new model::TVSourcesModel(this);

	m_currentGenreSourcesModel = new model::TVSourcesSortFilterProxyModel(this);
	m_currentGenreSourcesModel->setSourceModel(m_sourcesModel);

	m_searchSourcesModel = new model::TVSourcesSortFilterProxyModel(this);
	m_searchSourcesModel->setSourceModel(m_sourcesModel);

	m_topSourcesModel = new model::TVSourcesSortFilterProxyModel(this);
	m_topSourcesModel->setSourceModel(m_sourcesModel);

	m_bannersModel = new model::BannersModel(this);

	m_view = new view::TVPage(NULL, m_categoriesModel, m_currentGenreSourcesModel, m_searchSourcesModel, m_topSourcesModel);
	m_genresPopup = new view::TVGenresPopup(m_genresProxyModel);

	connect(m_view, SIGNAL(TVCurrentCategoryChanged(int)), SLOT(TVCurrentCategoryChanged(int)));
	connect(m_view, SIGNAL(ShowGenres(int)), SLOT(ShowGenres(int)));
	connect(m_view, SIGNAL(SearchFilterChanged(QString)), SLOT(SearchFilterChanged(QString)));
	connect(m_view, SIGNAL(GotoTVSource(int)), SLOT(GotoTVSource(int)));

	connect(m_genresPopup, SIGNAL(TVGenreChanged(int)), SLOT(TVCurrentGenreChanged(int)));
	
	ReLoadData();
}

TVPageController::~TVPageController()
{
	
}

bool TVPageController::IsActive() const
{
	return false;
}

view::TabPage* TVPageController::View() const
{
	return m_view;
}

void TVPageController::ReLoadData()
{
	m_categoriesModel->Cleanup();
	m_genresModel->Cleanup();
	m_sourcesModel->Cleanup();

	m_categoriesModel->Load(Path::ConfigFile("tvcategories.j"));
	m_genresModel->Load(Path::ConfigFile("tvgenres.j"));
	m_sourcesModel->Load(Path::ConfigFile("tvsoures.j"));

	m_bannersModel->Load(Path::ConfigFile("tvbanners.j"));

	if(m_currentCategory.isNull())
	{
		m_currentCategory = m_categoriesModel->First();
		m_currentCategory->SetTopVisible(true);
	}

	SetCurentCategory(m_currentCategory);
}

void TVPageController::Search(const QString& filter)
{
}

void TVPageController::Pause()
{
}

void TVPageController::SetCurentCategory(model::TVCategoryPtr category)
{
	m_genresProxyModel->SetCategoryIdFilter(category->Id());

	if(!m_currentCategory.isNull())
	{
		model::BannerInfoPtr banner = m_bannersModel->FindById(category->NextBannerId());
		
		int bannerId = 0;
		QString bannerLogo;

		if(!banner.isNull())
		{
			bannerId = banner->Id();
			bannerLogo = banner->Logo();
		}

		m_topSourcesModel->SetTvSourceIdsFilter(m_currentCategory->TVTopSourceIds());

		m_view->SetCurrentCategory(category->Id(), category->TopVisible(), bannerId, bannerLogo);
	}
}

void TVPageController::TVCurrentCategoryChanged(int categoryId)
{
	m_currentCategory = m_categoriesModel->FindById(categoryId);

	if(!m_currentCategory.isNull())
	{
		SetCurentCategory(m_currentCategory);
	}
}

void TVPageController::ShowGenres(int categoryId)
{
	QPoint pos = m_view->mapToGlobal(m_view->pos());
	pos.setY(pos.y() + GenresPopupTopMagrin);
	pos.setX(pos.x() + GenresPopupLeftMagrin);

	m_genresPopup->Show(pos);
}

void TVPageController::TVCurrentGenreChanged(int genreId)
{
	m_currentTVGenre = m_genresModel->FindById(genreId);

	if(!m_currentTVGenre.isNull())
	{
		m_view->SetCurrentGenre(m_currentTVGenre->Id(), m_currentTVGenre->Name());
	}

	m_currentGenreSourcesModel->SetGenreIdFilter(genreId);
}

void TVPageController::SearchFilterChanged(QString filter)
{
	m_searchSourcesModel->SetNameFilter(filter);
}

void TVPageController::GotoTVSource(int id)
{
	m_currentTVSource = m_sourcesModel->Find(id);

	if(!m_currentTVSource.isNull())
	{
		m_view->ShowTVSource(m_currentTVSource->Url());
	}
}

} //namespace controller
} //namespace mp
