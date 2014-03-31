#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsObject>
#include <QtGui>

// Forward declaration
class QVBoxLayout;
class QDeclarativeView;

namespace mp {

// Forward declaration
class Titlebar;
class TabWidget;
class TabPage;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0, Qt::WindowFlags flags = 0);
	~MainWindow();

	Titlebar * TitleBar() const;
	TabWidget * Tabs() const;

	void AddPage(TabPage * page);

	void closeEvent(QCloseEvent *evt);

private:
	QVBoxLayout *			m_layout;
	Titlebar*				m_titleBar;
	TabWidget *				m_tabWidget;
};

}

#endif // MAINWINDOW_H
