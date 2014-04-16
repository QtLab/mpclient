#include "MainWindow.h"
#include "NcFramelessHelper.h"
#include "TabWidget.h"
#include "Titlebar.h"
#include "WidgetUtils.h"

#include <QDebug>
#include <QVBoxLayout>

namespace mp {

MainWindow::MainWindow(QWidget *parent, Qt::WindowFlags flags)
	: QMainWindow(parent, flags)
{
	WidgetUtils::LoadStyleSheets(this, "Player.qss");

	setWindowIcon(QIcon(":/mp/Resources/Player.ico"));

	resize(550, 600);

	//NcFramelessHelper* f = new NcFramelessHelper(this);
	//f->setWidgetMovable(true);
	//f->setWidgetResizable(true);
	//f->activateOn(this);

	QWidget * central = new QWidget(this);
	setCentralWidget(central);

	qDebug() << "m_centralWidget was created";

	m_layout = new QVBoxLayout(central);
	m_layout->setContentsMargins(0, 0, 0, 0);
	m_layout->setSpacing(0);

	QWidget * borderTop = new  QWidget();
	borderTop->setObjectName("borderTop");
	m_layout->addWidget(borderTop, 1);

	m_titleBar = new Titlebar(this);
	m_layout->addWidget(m_titleBar);

	qDebug() << "Titlebar was created";

	m_tabWidget = new TabWidget(NULL, "tabWidget", "taBar");
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

void MainWindow::AddTab(TabPage * page)
{
	m_tabWidget->AddPage(page);
}

void MainWindow::closeEvent(QCloseEvent *evt)
{
	// Hide the window to tray
	hide();
	evt->ignore();
}

}