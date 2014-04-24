#ifndef MP_RADIO_PAGE_WIDGET_H
#define MP_RADIO_PAGE_WIDGET_H

#include "TabPage.h"
#include "Prerequirements.h"

namespace mp {

class RadioPage : public TabPage
{
	Q_OBJECT

public:
	RadioPage(QWidget* parent, CategoriesModelPtr categories, ChannelSourceModelPtr allStations, 
				ChannelSourceModelPtr topStations, ChannelSourceModelPtr lastStation);

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


	Q_INVOKABLE void PlayChannel(const QString& id);
	Q_INVOKABLE void PauseCurrentChannel();

	//Q_INVOKABLE void CurrentGenreChanged();

/*
public slots:
	// Channels model was updated
	virtual void CurrentChannelChanged(ChannelSource* channel);
	virtual void UpdateCurrentGenreStations(ChannelSourceModel* channels);
	virtual void GenreModelUpdated(GenreModel* genres);
	virtual void UpdateTopStations(ChannelSourceModel* channels);
	virtual void UpdateLastStations(ChannelSourceModel* channels);
*/

private slots:
	void GenreChanged(int genreId);

signals:
	void PlayRadio(const QString& id);
	void PauseCurrentRadio();
	void GenreChanged(const QString& id);
	void TopStationslUpdated();

private:
	// QML view
	QQuickView *							m_quickView;
	ChannelSourceSortFilterProxyModel *		m_allStationsProxyModel;
	ChannelSourceSortFilterProxyModel *		m_lastStationsProxyModel;
	ChannelSourceSortFilterProxyModel *		m_topStationsProxyModel;
};


}

#endif