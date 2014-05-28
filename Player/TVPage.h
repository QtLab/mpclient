#ifndef MP_TV_PAGE_WIDGET_H
#define MP_TV_PAGE_WIDGET_H

#include "Prerequirements.h"
#include "TabPage.h"

namespace mp {

class TVPage : public TabPage
{
	Q_OBJECT

public:
	TVPage(QWidget* parent, ChannelSourceModelPtr channels);
	virtual ~TVPage();

	// User leave the page
	virtual void Enter();

	// User leave the page
	virtual bool Leave();

	// Languege changed
	virtual void RetranslateUI();

	QString Name() const;

private slots:
	void ContentLoaded(bool ok);
	void FlashInstallStarted();
	void FlashInstallProgressChanged(int percent);
	void FlashInstalled(bool);
	
signals:
	void FlashInstalled();

private:
	QHBoxLayout	*		m_layout;
	WebView *			m_view;
};


}

#endif