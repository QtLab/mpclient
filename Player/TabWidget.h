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

class TabPage;

class TabWidget : public QTabWidget
{
	Q_OBJECT

public:
	TabWidget(QWidget * parent, const QString& name = QString(), const QString& tabBarName = QString());
	int AddPage(TabPage * page, const QString& name);

private:
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