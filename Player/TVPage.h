#ifndef MP_TV_PAGE_WIDGET_H
#define MP_TV_PAGE_WIDGET_H

#include "Prerequirements.h"
#include "TabPage.h"

namespace mp {
namespace view {

class TVPage : public TabPage
{
	Q_OBJECT

public:
	TVPage(QWidget* parent, model::ChannelSourceModelPtr channels);
	virtual ~TVPage();

	// User leave the page
	void Enter();

	// User leave the page
	bool Leave();

	// Languege changed
	void RetranslateUI();

	QString Name() const;

	bool Resizable() const;
	QSize RestoreSize() const;
	void SaveSize(const QSize& size);

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
}

#endif