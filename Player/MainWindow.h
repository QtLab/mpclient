#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsObject>
#include <QtGui>

// Forward declaration

QT_BEGIN_NAMESPACE
class QVBoxLayout;
class QDeclarativeView;
QT_END_NAMESPACE

class NcFramelessHelper;

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

	int AddTab(TabPage * page);
	void SetResizable(bool resizable);
	QSize Size() const;
	void SetSize(const QSize& size);

	void closeEvent(QCloseEvent *evt);

signals:
	void CurrentTabChanged(int index);

private:
	QVBoxLayout *			m_layout;

	NcFramelessHelper *		m_frameLessHelper;
	QSize					m_lastTVSize;

	Titlebar*				m_titleBar;
	QPoint					m_cursorPosition;
	TabWidget *				m_tabWidget;
};

}

#endif // MAINWINDOW_H
