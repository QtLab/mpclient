#ifndef MP_TAB_PANE_H
#define MP_TAB_PANE_H

#include <QWidget>

namespace mp {

// Base class for all tabs
class TabPage : public QWidget
{
	Q_OBJECT

public:
	TabPage(QWidget* parent);
	virtual ~TabPage();

	// User leave the page
	virtual void Enter();

	// User leave the page
	virtual bool Leave();

	// Languege changed
	virtual void RetranslateUI();

	virtual QMargins Margins();

	virtual QString Name() const = 0;

protected:
	void changeEvent(QEvent *event);
};

}

#endif