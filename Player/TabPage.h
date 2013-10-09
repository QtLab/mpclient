#ifndef MP_TAB_PANE_H
#define MP_TAB_PANE_H

#include <QWidget>

class QDeclarativeView;

namespace mp {

// Forward declaration
class ChannelSourceModel;

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

	// Channels model was updated
	virtual void ModelUpdated(ChannelSourceModel* channels);

	QMargins Margins();

protected:
	void changeEvent(QEvent *event);

	QDeclarativeView* CreateDeclarativeView(const QString& qmlFileName);
};

}

#endif