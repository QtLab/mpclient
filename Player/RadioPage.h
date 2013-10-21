#ifndef MP_RADIO_PAGE_WIDGET_H
#define MP_RADIO_PAGE_WIDGET_H

#include "TabPage.h"

class QDeclarativeView;

namespace mp {

class ChannelSourceModel;
class GenreModel;
class ChannelSource;

class RadioPage : public TabPage
{
	Q_OBJECT

public:
	RadioPage(QWidget* parent, ChannelSourceModel* channels, GenreModel* genres);
	virtual ~RadioPage();

	// User leave the page
	virtual void Enter();

	// User leave the page
	virtual bool Leave();

	// Languege changed
	virtual void RetranslateUI();

	// Connect event to declarative QML view
	void Connect(const char* signal, QObject* reciever, const char* slot);

public slots:
	// Channels model was updated
	virtual void CurrentChannelChanged(ChannelSource* channel);
	virtual void ChannelSourceModelUpdated(ChannelSourceModel* channels);
	virtual void GenreModelUpdated(GenreModel* genres);

private:
	QDeclarativeView *		m_view;
};


}

#endif