#ifndef MP_TITLE_BAR_WIDGET_H
#define MP_TITLE_BAR_WIDGET_H

#include <QWidget>
#include <QLabel>

class QHBoxLayout;
class QMouseEvent;

namespace mp {

class ChannelMetadata;
class LinkButton;

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

private slots:
	void CloseClicked();
	void MinimizeClicked();
	void MetadataUpdated(const ChannelMetadata& metadata);

private:
	QWidget	*						m_parent;
	QPoint							m_cursorPosition;
	QHBoxLayout *					m_layout;
	QLabel *						m_logo;

	LinkButton *					m_minimizeLnk;
	LinkButton *					m_closeLnk;
};

}

#endif // MP_TITLE_BAR_WIDGET_H
