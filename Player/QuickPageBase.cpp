#include "QuickPageBase.h"
#include "Path.h"

#include <QQuickView>
#include <QQmlContext>
#include <QQuickItem>
#include <QMenu>
#include <QToolTip>
#include <QApplication>
#include <QClipboard>
#include <QHBoxLayout>
#include <QDebug>

namespace mp {
namespace view {

QuickPageBase::QuickPageBase(QWidget* parent, const QString& qmlFile)
	:TabPage(parent)
{
	m_layout = new QVBoxLayout(this);
	m_layout->setContentsMargins(0, 0, 0, 0);

	m_quickView = new QQuickView();
	m_quickViewWidget = QWidget::createWindowContainer(m_quickView, this);

	m_layout->addWidget(m_quickViewWidget);

	QString localUrl = QUrl::fromLocalFile(Path::QmlFile(qmlFile)).toString();
	qDebug()  << "Load qml from: "<< localUrl;

	m_quickView->setSource(localUrl);
	m_quickView->setResizeMode(QQuickView::SizeRootObjectToView);

	connect(RootQuickItem(), SIGNAL(showSearchContextMenu(bool)), this, SLOT(ShowSearchContextMenu(bool)));
	connect(RootQuickItem(), SIGNAL(showTooltip(QString)), this, SLOT(ShowTooltip(QString)));
}

QuickPageBase::~QuickPageBase()
{
}

QQuickView * QuickPageBase::QuickView() const
{
	return m_quickView;
}

QWidget * QuickPageBase::QuickViewWidget() const
{
	return m_quickViewWidget;
}

QQuickItem * QuickPageBase::RootQuickItem() const
{
	return m_quickView->rootObject();
}

QQmlContext* QuickPageBase::RootContext() const
{
	return m_quickView->rootContext();
}

QVBoxLayout * QuickPageBase::Layout() const
{
	return m_layout;
}

void QuickPageBase::ShowSearchContextMenu(bool hasSelectedText)
{
	QMenu menu(this);

	if(hasSelectedText)
	{
		menu.addAction(tr("Copy"), this, SLOT(CopyFromSearchEdit()));
		menu.addAction(tr("Cut"), this, SLOT(CutFromSearchEdit()));
	}

	QString txt = QApplication::clipboard()->text();
	if(txt.length() > 0)
	{
		menu.addAction(tr("Paste"), this, SLOT(PasteToSearchEdit()));
	}

	menu.exec(QCursor::pos());
}

void QuickPageBase::CopyFromSearchEdit()
{
	QMetaObject::invokeMethod(RootQuickItem(), "copyFromSearchEdit");
}

void QuickPageBase::CutFromSearchEdit()
{
	QMetaObject::invokeMethod(RootQuickItem(), "cutFromSearchEdit");
}

void QuickPageBase::PasteToSearchEdit()
{
	QMetaObject::invokeMethod(RootQuickItem(), "pasteToSearchEdit");
}

void QuickPageBase::ShowTooltip(QString text)
{
	QPoint pos = QCursor::pos();
	QToolTip::showText(pos, text, this, QRect(), 2000);
}

} // end namespace view
} // end namespace mp