#include "TVPageController.h"
#include "TVCategoriesModel.h"
#include "TVGenresModel.h"
#include "TVGenresSortFilterProxyModel.h"
#include "TVSourcesModel.h"
#include "TVSourcesSortFilterProxyModel.h"
#include "BannersModel.h"
#include "TVPage.h"
#include "TVGenresPopupView.h"
#include "Path.h"

#include <QUrl>
#include <QDesktopServices>

namespace mp {
namespace controller {

TVPageController::TVPageController()
	:m_genresPopupView(NULL)
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

	m_tvPageView = new view::TVPage(NULL, m_categoriesModel, m_currentGenreSourcesModel, m_searchSourcesModel, m_topSourcesModel);
	m_genresPopupView = new view::TVGenresPopupView(m_genresProxyModel);

	connect(m_tvPageView, SIGNAL(TVCurrentCategoryChanged(int)), SLOT(TVCurrentCategoryChanged(int)));
	connect(m_tvPageView, SIGNAL(ShowGenres(int)), SLOT(ShowGenres(int)));
	connect(m_tvPageView, SIGNAL(SearchFilterChanged(QString)), SLOT(SearchFilterChanged(QString)));
	connect(m_tvPageView, SIGNAL(GotoTVSource(int)), SLOT(GotoTVSource(int)));
	connect(m_tvPageView, SIGNAL(ProcessBanner(int)), this, SLOT(ProcessBanner(int)));

	connect(m_genresPopupView, SIGNAL(TVGenreChanged(int)), SLOT(TVCurrentGenreChanged(int)));
	
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
	return m_tvPageView;
}

void TVPageController::ReLoadData()
{
	m_categoriesModel->Cleanup();
	m_genresModel->Cleanup();
	m_sourcesModel->Cleanup();

	m_categoriesModel->Load(Path::ConfigFile("tvcategories.j"));
	m_genresModel->Load(Path::ConfigFile("tvgenres.j"));
	m_bannersModel->Load(Path::ConfigFile("tvbanners.j"));
	m_sourcesModel->Load(Path::ConfigFile("tvsoures.j"), m_genresModel->GenreIdsToCategoryIdBindingMap());

	if(m_currentCategory.isNull())
	{
		m_currentCategory = m_categoriesModel->First();

		if(!m_currentCategory.isNull())
		{
			m_currentCategory->SetTopVisible(true);
		}
	}

	GotoCategory(m_currentCategory);
}

void TVPageController::Search(const QString& filter)
{
}

void TVPageController::Pause()
{
}

void TVPageController::GotoCategory(model::TVCategoryPtr category)
{
	if(!category.isNull())
	{
		m_genresProxyModel->SetCategoryIdFilter(category->Id());

		model::BannerInfoPtr banner = m_bannersModel->FindById(category->RandomBannerId());
		
		int bannerId = 0;
		QString bannerLogo;

		if(!banner.isNull())
		{
			bannerId = banner->Id();
			bannerLogo = banner->Logo();
		}

		m_topSourcesModel->SetTvSourceIdsFilter(category->TVTopSourceIds());

		m_tvPageView->SetCurrentCategory(category->Id(), category->TopVisible(), bannerId, bannerLogo);
	}
}

void TVPageController::TVCurrentCategoryChanged(int categoryId)
{
	m_currentCategory = m_categoriesModel->FindById(categoryId);

	if(!m_currentCategory.isNull())
	{
		GotoCategory(m_currentCategory);
	}
}

void TVPageController::ShowGenres(int categoryId)
{
	QPoint pos = m_tvPageView->mapToGlobal(m_tvPageView->pos());
	m_genresPopupView->Show(pos);
}

void TVPageController::TVCurrentGenreChanged(int genreId)
{
	m_currentTVGenre = m_genresModel->FindById(genreId);

	if(!m_currentTVGenre.isNull())
	{
		m_tvPageView->SetCurrentGenre(m_currentTVGenre->Id(), m_currentTVGenre->Name());
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
		m_tvPageView->ShowTVSource(m_currentTVSource->Url(), m_currentTVSource->Selector());
	}
}

void TVPageController::ProcessBanner(int id)
{
	model::BannerInfoPtr banner = m_bannersModel->FindById(id);
		
	if(!banner.isNull())
	{
		int tvSourceId = banner->TVSourceId();

		if(tvSourceId >= 0)
		{
			GotoTVSource(tvSourceId);
		}

		QString url = banner->ExternalUrl();

		if(!url.isEmpty())
		{
			QDesktopServices::openUrl(QUrl(url));
		}
	}
}

} //namespace controller
} //namespace mp
