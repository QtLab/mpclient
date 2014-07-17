#ifndef MP_TAB_PAGE_H
#define MP_TAB_PAGE_H

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
	
	virtual QString Name() const = 0;
	virtual QString ToolTip() const;
	int TabIndex() const;

	virtual bool Resizable() const = 0;
	virtual QSize RestoreSize() const = 0;
	virtual void SaveSize(const QSize& size) = 0;

private:
	void SetTabIndex(int index);
	void changeEvent(QEvent *event);

signals:
	void SizeChangeRequest(const QSize& newSize);

private:
	int		m_tabIndex;

	friend class TabWidget;
};

} // end namespace view
} // end namespace mp

#endif