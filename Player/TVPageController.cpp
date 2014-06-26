#include "TVPageController.h"
#include "ChannelSourceModel.h"
#include "TVPage.h"
#include "Path.h"

namespace mp {
namespace controller {

TVPageController::TVPageController()
	:m_view(new view::TVPage(NULL, model::ChannelSourceModelPtr()))
{
	ReLoadData();

	connect(m_view, SIGNAL(FlashInstalled()), SIGNAL(FlashInstalled()));
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
}

void TVPageController::Search(const QString& filter)
{
}

void TVPageController::Stop()
{
}


}
}