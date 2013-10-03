#ifndef MP_CHANNEL_SOURCES_CONTROLLER_H
#define MP_CHANNEL_SOURCES_CONTROLLER_H

#include "ChannelSourceModel.h"

namespace mp {

// Forward declaration
class TabPage;
class TabWidget;

class TabPagesController
{
public:	
	static TabPagesController& Inst();

	const ChannelSourceModel& TVChannels();
	const ChannelSourceModel& RadioChannels();

	TabWidget* CreateTabsView();
	TabPage* CreateabView(const QString& tabName);
	TabPage* CreateTVTabView();
	TabPage* CreateRadioTabView();

	void LoadData();

private:
	TabPagesController();
	~TabPagesController();

private:
	static TabPagesController*		m_instance;
	ChannelSourceModel				m_radioChannels;
	ChannelSourceModel				m_tvChannels;
};

}

#endif