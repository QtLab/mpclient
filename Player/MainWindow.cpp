#include "MainWindow.h"
#include "NcFramelessHelper.h"
#include "TabWidget.h"
#include "Titlebar.h"

#include <QDebug>
#include <QVBoxLayout>


namespace mp {

MainWindow::MainWindow(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	resize(550, 600);

	NcFramelessHelper* f = new NcFramelessHelper(this);
	f->setWidgetMovable(true);
	f->setWidgetResizable(true);
	f->activateOn(this);

	QWidget * central = new QWidget(this);
	setCentralWidget(central);

	qDebug() << "m_centralWidget was created";

	m_layout = new QVBoxLayout(central);
	m_layout->setContentsMargins(0, 0, 0, 0);
	m_layout->setSpacing(0);

	m_titleBar = new Titlebar(this);
	m_layout->addWidget(m_titleBar);
}

MainWindow::~MainWindow()
{
}

Titlebar * MainWindow::TitleBar() const
{
	return m_titleBar;
}

void MainWindow::AddWidget(QWidget* widget)
{
	m_layout->addWidget(widget);
}

}