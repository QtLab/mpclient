#ifndef MP_RADIO_PAGE_WIDGET_H
#define MP_RADIO_PAGE_WIDGET_H

#include "TabPage.h"
#include "Prerequirements.h"

#include <QAbstractItemModel>

namespace mp {
namespace view {

class RadioPage : public TabPage
{
	Q_OBJECT

public:
	RadioPage(QWidget* parent, QAbstractItemModel* categoriesModel, QAbstractItemModel* allStationsModel, 
				QAbstractItemModel* topStationsModel, QAbstractItemModel* lastStationsModel, QAbstractItemModel* searchStationsModel);

	virtual ~RadioPage();

	// User leave the page
	void Enter();
	// User leave the page
	bool Leave();
	// Languege changed
	void RetranslateUI();
	QString Name() const;

	bool Resizable() const;
	QSize RestoreSize() const;
	void SaveSize(const QSize& size);

	void SetVolume(qreal val);
	void SetPlayingState(bool play);
	void SetMetadata(const QString& metadata);
	void SetPlayStationName(const QString& name);
	void SetCategory(int id);

signals:
	void PlayRadio(int id);
	void ResumeRadio();
	void PauseRadio();
	void VolumeChanged(qreal value);
	void PauseCurrentRadio();
	void CategoryChanged(int id);
	void SearchFilterChanged(QString filter);
	void SearchTracks(QString filter);

private:
	// QML view
	QQuickView *							m_quickView;
};


}
}

#endif