#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>

#include "Prerequirements.h"

namespace mp {
namespace view {

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
	int CurrentTabIndex() const;
	void SetCurrentTabIndex(int index);
	
	void SetResizable(bool resizable);
	QSize Size() const;
	void SetSize(const QSize& size);

private:
	void closeEvent(QCloseEvent *evt);
	void resizeEvent(QResizeEvent *evt);

private slots:
	void SizeChanged(const QSize& newSize, bool canResize);

signals:
	void CurrentPageChanged(view::TabPage * newPage, view::TabPage * oldPage);

private:
	QVBoxLayout *			m_layout;
	NcFramelessHelper *		m_frameLessHelper;
	QSize					m_lastTVSize;

	Titlebar*				m_titleBar;
	QPoint					m_cursorPosition;
	TabWidget *				m_tabWidget;
};

} // end namespace view
} // end namespace mp

#endif // MAINWINDOW_H
