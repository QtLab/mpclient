#include "MainWindow.h"
#include "TabWidget.h"
#include "Titlebar.h"
#include "WidgetUtils.h"

#include <QDebug>
#include <QVBoxLayout>

namespace mp {

MainWindow::MainWindow(QWidget *parent, Qt::WindowFlags flags)
	: QMainWindow(parent, flags)
{
	setWindowFlags( Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint );

	WidgetUtils::LoadStyleSheets(this, "Player.qss");

	setWindowIcon(QIcon(":/mp/Resources/Player.ico"));

	resize(580, 351);

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
	m_titleBar->installEventFilter(this);
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

bool MainWindow::eventFilter(QObject *object, QEvent *evt)
{
	if (object == m_titleBar)
	{
		if(evt->type() == QEvent::MouseButtonPress)
		{
			m_cursorPosition = static_cast<QMouseEvent *>(evt)->globalPos();
		}
		else
		{
			if(evt->type() == QEvent::MouseMove)
			{
				QMouseEvent* moveEvt = static_cast<QMouseEvent *>(evt);

				const QPoint delta = moveEvt->globalPos() - m_cursorPosition;
				move(x()+delta.x(), y()+delta.y());
				m_cursorPosition = moveEvt->globalPos();
			}
		}
	}

	return false;
}

}