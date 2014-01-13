#include "MainWindow.h"
#include "NcFramelessHelper.h"
#include "TabWidget.h"
#include "Titlebar.h"
#include "WidgetUtils.h"
#include "MultiEditDialog.h"

#include <QDebug>
#include <QVBoxLayout>


namespace mp {

MainWindow::MainWindow(QWidget *parent, Qt::WindowFlags flags)
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

	QWidget * borderTop = new  QWidget();
	borderTop->setObjectName("borderTop");
	m_layout->addWidget(borderTop, 1);

	m_titleBar = new Titlebar(this);
	m_layout->addWidget(m_titleBar);
	// Installs an event filter filterObj on this object.
	//installEventFilter(this);

	WidgetUtils::LoadStyleSheets(this, "Player.qss");
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

bool MainWindow::eventFilter(QObject *obj, QEvent *evt)
{
	if (evt->type() == QEvent::MouseButtonRelease) 
	{
		QMouseEvent * mouseEvt = static_cast<QMouseEvent *>(evt);
		if(mouseEvt && mouseEvt->button() == Qt::RightButton)
		{
			QWidget * widg = qobject_cast<QWidget*>(obj);
			if(widg)
			{
				QString stylesheet = widg->styleSheet();
				MultiEditDialog* dlg = new MultiEditDialog(stylesheet);
				stylesheet = dlg->ShowModal();
				widg->setStyleSheet(stylesheet);

				return true;
			}

			//QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
			//qDebug() << "Ate key press" << keyEvent->key();
			return QMainWindow::eventFilter(obj, evt);
		} 
		else 
		{
			// pass the event on to the parent class
			return QMainWindow::eventFilter(obj, evt);
		}
	}


	return QMainWindow::eventFilter(obj, evt);
}

}