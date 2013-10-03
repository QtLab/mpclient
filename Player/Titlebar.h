#ifndef MP_TITLE_BAR_WIDGET_H
#define MP_TITLE_BAR_WIDGET_H

#include <QWidget>
#include <QLabel>

class QHBoxLayout;
class QMouseEvent;

namespace mp {

class ChannelMetadata;

class Titlebar : public QWidget
{
	Q_OBJECT

public:
	Titlebar(QWidget *parent = 0);
	virtual ~Titlebar() {}

private:
	void RetranslateUI();
	
	void changeEvent(QEvent *event);
	void mouseMoveEvent(QMouseEvent *evt);
	void mouseDoubleClickEvent(QMouseEvent *evt);

public slots:
	void SwitchScreenMode();
	void MetadataUpdated(const ChannelMetadata& metadata);

private:
	QWidget	*					m_parent;
	QPoint						m_cursorPosition;
	QHBoxLayout *				m_layout;
	QLabel *					m_logo;

	QLabel *					m_minimizeLnk;
	QLabel *					m_closeLnk;
};

}

#endif // MP_TITLE_BAR_WIDGET_H
