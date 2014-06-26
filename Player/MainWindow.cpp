#include "MainWindow.h"
#include "TabWidget.h"
#include "Titlebar.h"
#include "WidgetUtils.h"
#include "NcFramelessHelper.h"
#include "Config.h"
#include "TabPage.h"

#include <QDebug>
#include <QVBoxLayout>
#include <QStatusBar>

namespace mp {
namespace view {

MainWindow::MainWindow(QWidget *parent, Qt::WindowFlags flags)
	: QMainWindow(parent, flags)
	,m_frameLessHelper(new NcFramelessHelper(this))
{
	m_frameLessHelper->setWidgetMovable(true);
	m_frameLessHelper->setWidgetResizable(false);
	m_frameLessHelper->activateOn(this);

	setWindowFlags( Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint );

	WidgetUtils::LoadStyleSheets(this, "Player.qss");

	setWindowIcon(QIcon(":/mp/Resources/Player.ico"));

	QWidget * central = new QWidget();
	central->setObjectName("centralWidget");
	setCentralWidget(central);

	m_layout = new QVBoxLayout(central);
	m_layout->setContentsMargins(0, 0, 0, 0);
	m_layout->setSpacing(0);

	m_titleBar = new Titlebar(this);
	m_layout->addWidget(m_titleBar);
	qDebug() << "Titlebar was created";

	m_tabWidget = new TabWidget(NULL, "tabWidget", "taBar");
	connect(m_tabWidget, SIGNAL(CurrentPageChanged(view::TabPage*,view::TabPage*)), SIGNAL(CurrentPageChanged(view::TabPage*,view::TabPage*)));
	m_tabWidget->tabBar()->setFont(Config::Inst().DefaultFont());
	m_tabWidget->setFont(Config::Inst().DefaultFont());
	m_layout->addWidget(m_tabWidget);
	qDebug() << "TabWidget was created";
}

MainWindow::~MainWindow()
{
}

Titlebar * MainWindow::TitleBar() const
{
	return m_titleBar;
}

TabWidget * MainWindow::Tabs() const
{
	return m_tabWidget;
}

int MainWindow::AddTab(TabPage * page)
{
	return m_tabWidget->AddPage(page);
}

int MainWindow::CurrentTabIndex() const
{
	int currentIdex = m_tabWidget->currentIndex();
	return currentIdex;
}

void MainWindow::SetCurrentTabIndex(int index)
{
	m_tabWidget->setCurrentIndex(index);
}

void MainWindow::SetResizable(bool resizable)
{
	m_frameLessHelper->setWidgetResizable(resizable);
}

QSize MainWindow::Size() const
{
	return size();
}

void MainWindow::SetSize(const QSize& size)
{
	resize(size);
}

void MainWindow::closeEvent(QCloseEvent *evt)
{
	// Hide the window to tray
	hide();
	evt->ignore();
}

void MainWindow::resizeEvent(QResizeEvent *evt)
{
	//TabPagePtr page = m_tabWidget->PageAtIndex(m_tabWidget->CurrentPageIndex());
	//if(page && page->Resizable())
	//{
	//	page->SaveSize(evt->size());
	//}
}

}
}