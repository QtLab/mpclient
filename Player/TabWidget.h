#ifndef MP_TAB_WIDGET_H
#define MP_TAB_WIDGET_H

#include <QTabWidget>
#include <QTabBar>
#include <QMap>

namespace mp {

class TabPage;

class TabWidget : public QTabWidget
{
	Q_OBJECT

public:
	TabWidget(QWidget * parent, const QString& name = QString(), const QString& tabBarName = QString());
	int AddPage(TabPage * page);

private:
	void RetranslateUI();
	void changeEvent(QEvent *event);

private:
	typedef	QMap<int, TabPage*> TabPages;
	TabPages		m_pages;
};

}

#endif