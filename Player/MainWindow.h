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

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0, Qt::WFlags flags = 0);
	~MainWindow();

	Titlebar * TitleBar() const;
	void AddWidget(QWidget* widget);

private:
	QVBoxLayout *			m_layout;
	Titlebar*				m_titleBar;
};

}

#endif // MAINWINDOW_H
