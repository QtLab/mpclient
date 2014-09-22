#ifndef MP_STUB_PAGE_H
#define MP_STUB_PAGE_H

#include "TabPage.h"
#include "Prerequirements.h"

namespace mp {
namespace view {

// Base class for all QML tabs
class StubPage : public TabPage
{
	Q_OBJECT

public:
	StubPage(QWidget* parent);
	virtual ~StubPage();

	QString Name() const;
	QString ToolTip() const;
	bool Resizable() const;
	QSize RestoreSize() const;
	void SaveSize(const QSize& size);

private:
	QVBoxLayout *				m_layout;
};

} // end namespace view
} // end namespace mp

#endif