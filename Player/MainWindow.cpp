#include "MainWindow.h"
#include "TabWidget.h"
#include "Titlebar.h"
#include "WidgetUtils.h"
#include "NcFramelessHelper.h"
#include "Config.h"

#include <QDebug>
#include <QVBoxLayout>

namespace mp {

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
	connect(m_tabWidget, SIGNAL(currentChanged(int)), SIGNAL(CurrentTabChanged(int)));
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


}