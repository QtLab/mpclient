#ifndef MP_TV_PAGE_WIDGET_H
#define MP_TV_PAGE_WIDGET_H

#include "TabPage.h"



class QDeclarativeView;

namespace mp {

class GenreModel;
class ChannelSourceModel;
class WebView;

class TVPage : public TabPage
{
	Q_OBJECT

public:
	TVPage(QWidget* parent, ChannelSourceModel* channels);
	virtual ~TVPage();

	// User leave the page
	virtual void Enter();

	// User leave the page
	virtual bool Leave();

	// Languege changed
	virtual void RetranslateUI();

	// Channels model was updated
	virtual void ChannelSourceModelUpdated(ChannelSourceModel* channels);
	virtual void GenreModelUpdated(GenreModel* channels);

private:
	WebView *		m_view;
};


}

#endif