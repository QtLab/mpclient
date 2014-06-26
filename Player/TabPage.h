#ifndef MP_TAB_PANE_H
#define MP_TAB_PANE_H

#include <QWidget>

namespace mp {
namespace view {

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
	virtual bool Resizable() const = 0;
	virtual QSize RestoreSize() const = 0;
	virtual void SaveSize(const QSize& size) = 0;
	int TabIndex() const;

private:
	void SetTabIndex(int index);
	void changeEvent(QEvent *event);

private:
	int		m_tabIndex;

	friend class TabWidget;
};

}
}

#endif