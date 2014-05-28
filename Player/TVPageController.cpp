#include "TVPageController.h"
#include "ChannelSourceModel.h"
#include "TVPage.h"
#include "Path.h"

namespace mp {

TVPageController::TVPageController()
	:m_view(new TVPage(NULL, ChannelSourceModelPtr()))
{
	ReLoadData();

	connect(m_view, SIGNAL(FlashInstalled()), SIGNAL(FlashInstalled()));
}

TVPageController::~TVPageController()
{
	
}

void TVPageController::ReLoadData()
{
}

TabPage* TVPageController::View()
{
	return m_view;
}

}