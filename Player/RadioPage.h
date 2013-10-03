#ifndef MP_RADIO_PAGE_WIDGET_H
#define MP_RADIO_PAGE_WIDGET_H

#include "TabPage.h"

class QDeclarativeView;

namespace mp {

class RadioPage : public TabPage
{
	Q_OBJECT

public:
	RadioPage(QWidget* parent, ChannelSourceModel* channels);
	virtual ~RadioPage();

	// User leave the page
	virtual void Enter();

	// User leave the page
	virtual bool Leave();

	// Languege changed
	virtual void RetranslateUI();

	// Channels model was updated
	virtual void ModelUpdated(ChannelSourceModel* channels);

private:
	QDeclarativeView *		m_view;
};


}

#endif