#include "StubPage.h"
#include "Path.h"

#include <QVBoxLayout>

namespace mp {
namespace view {

StubPage::StubPage(QWidget* parent)
	:TabPage(parent)
{
	m_layout = new QVBoxLayout(this);
	m_layout->setContentsMargins(0, 0, 0, 0);
}

StubPage::~StubPage()
{
}

QString StubPage::Name() const
{
	return "Test";
}

QString StubPage::ToolTip() const
{
	return "";
}

bool StubPage::Resizable() const
{
	return false;
}

QSize StubPage::RestoreSize() const
{
	return QSize(500, 600);
}

void StubPage::SaveSize(const QSize& size)
{

}

} // end namespace view
} // end namespace mp