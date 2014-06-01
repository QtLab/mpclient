#ifndef MP_RADIO_PAGE_WIDGET_H
#define MP_RADIO_PAGE_WIDGET_H

#include "TabPage.h"
#include "Prerequirements.h"

namespace mp {

class RadioPage : public TabPage
{
	Q_OBJECT

public:
	RadioPage(QWidget* parent, QObject* categoriesModel, QObject* allStationsModel, 
				QObject* topStationsModel, QObject* lastStationsModel, QObject* searchStationsModel);

	virtual ~RadioPage();

	// User leave the page
	void Enter();
	// User leave the page
	bool Leave();
	// Languege changed
	void RetranslateUI();
	QString Name() const;

	void SetVolume(qreal val);
	void SetPlayingState(bool play);
	void SetMetadata(const QString& metadata);
	void SetCategory(int index);

signals:
	void PlayRadio(int id);
	void ResumeRadio();
	void PauseRadio();
	void VolumeChanged(qreal value);
	void PauseCurrentRadio();
	void CategoryChanged(int id);
	void SearchFilterChanged(QString seasrch);

private:
	// QML view
	QQuickView *							m_quickView;
};


}

#endif