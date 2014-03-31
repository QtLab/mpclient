#ifndef MP_RADIO_PAGE_WIDGET_H
#define MP_RADIO_PAGE_WIDGET_H

#include "TabPage.h"

class QQuickView;

namespace mp {

class ChannelSourceModel;
class GenreModel;
class ChannelSource;

class RadioPage : public TabPage
{
	Q_OBJECT

public:
	RadioPage(QWidget* parent);
	virtual ~RadioPage();

	void Init();

	// User leave the page
	void Enter();

	// User leave the page
	bool Leave();

	// Languege changed
	void RetranslateUI();

	QString Name() const;

	// Connect event to declarative QML view
	void Connect(const char* signal, QObject* reciever, const char* slot);

	QString CurrentRaioChannelId();
	QString CurrentGenreId();


	Q_INVOKABLE void PlayCurrentRadio();
	Q_INVOKABLE void PauseCurrentRadio();
	Q_INVOKABLE void CurrentGenreChanged();

public slots:
	// Channels model was updated
	virtual void CurrentChannelChanged(ChannelSource* channel);
	virtual void UpdateCurrentGenreStations(ChannelSourceModel* channels);
	virtual void GenreModelUpdated(GenreModel* genres);
	virtual void UpdateTopStations(ChannelSourceModel* channels);
	virtual void UpdateLastStations(ChannelSourceModel* channels);

signals:
	void PlayRadio(const QString& id);
	void PauseRadio(const QString& id);
	void GenreChanged(const QString& id);
	void TopStationslUpdated();

private:
	// QML view
	QQuickView *				m_view;
};


}

#endif