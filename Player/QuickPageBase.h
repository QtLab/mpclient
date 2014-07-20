#ifndef MP_QUICK_PAGE_BASE_H
#define MP_QUICK_PAGE_BASE_H

#include "TabPage.h"
#include "Prerequirements.h"

namespace mp {
namespace view {

// Base class for all QML tabs
class QuickPageBase : public TabPage
{
	Q_OBJECT

public:
	QuickPageBase(QWidget* parent, const QString& qmlFile);
	virtual ~QuickPageBase();

protected:
	QQuickView * QuickView() const;
	QWidget * QuickViewWidget() const;
	QQuickItem * RootQuickItem() const;
	QQmlContext* RootContext() const;
	QVBoxLayout * Layout() const;

private slots:
	void ShowSearchContextMenu(bool hasSelectedText);
	void CopyFromSearchEdit();
	void CutFromSearchEdit();
	void PasteToSearchEdit();
	void ShowTooltip(QString text);

private:
	QWidget *					m_quickViewWidget;
	// QML view
	QQuickView *				m_quickView;
	QVBoxLayout *				m_layout;
};

} // end namespace view
} // end namespace mp

#endif