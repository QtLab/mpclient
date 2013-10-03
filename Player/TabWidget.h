#ifndef MP_TAB_WIDGET_H
#define MP_TAB_WIDGET_H

#include <QTabWidget>
#include <QTabBar>
#include <QObject>
#include <QString>
#include <QResizeEvent>
#include <QMainWindow>
#include <QDebug>
#include <QPainter>
#include <QStyle>
#include <QPaintEvent>

namespace mp {

class TabBar : public QTabBar
{
	Q_OBJECT

public:
	TabBar(QWidget * parent);

	QSize TabBar::tabSizeHint(int indx) const;
	//void paintEvent(QPaintEvent *);

	void setFont( const QFont& font );
};


class TabPage;

class TabWidget : public QTabWidget
{
	Q_OBJECT

public:
	TabWidget(QWidget * parent, const QString& name = QString::null, const QString& tabBarName = QString::null, bool useCustomTabBar = true);
	QTabBar* TabBar();

	int AddPage(TabPage * page, const QString& name);
	void SetTvPage(TabPage * page);
	void SetRadioPage(TabPage * page);

private:
	TabPage *		m_tvPage;
	int				m_tvPageIndex;

	TabPage *		m_radioPage;
	int				m_radioPageIndex;
};

}

#endif