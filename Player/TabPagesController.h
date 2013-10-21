#ifndef MP_CHANNEL_SOURCES_CONTROLLER_H
#define MP_CHANNEL_SOURCES_CONTROLLER_H

#include "ChannelSourceModel.h"
#include "GenreModel.h"
#include "TabModel.h"

namespace mp {

// Forward declaration
class TabPage;
class TabWidget;

class TabPagesController : public QObject
{
	Q_OBJECT

public:	
	static TabPagesController& Inst();

	const ChannelSourceModel& TVChannels();

	TabWidget* CreateTabsView();
	TabPage* CreateabView(const QString& tabName);
	TabPage* CreateTVTabView();
	void ReLoadData();

private:
	TabPagesController();
	~TabPagesController();

private:
	static TabPagesController*		m_instance;
	//ChannelSourceModel				m_radioChannels;
	//GenreModel						m_radioGenres;
	ChannelSourceModel				m_tvChannels;
	GenreModel						m_tvGenres;
	TabModel						m_tabs;
};

}

#endif