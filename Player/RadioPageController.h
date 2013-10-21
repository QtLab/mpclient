#ifndef MP_RADIO_PAGE_CONTROLLER_H
#define MP_RADIO_PAGE_CONTROLLER_H

#include "ChannelSourceModel.h"
#include "GenreModel.h"
#include "TabModel.h"

namespace mp {

// Forward declaration
class TabPage;
class TabWidget;
class RadioPage;

class RadioPageController : public QObject
{
	Q_OBJECT

public:	
	static RadioPageController& Inst();
	void ReLoadData();
	TabPage* View();

public slots:
	void PlayRadio(const QString& id);
	void PauseRadio();
	void RadioGenreSelect(const QString& id);

private:
	RadioPageController();
	~RadioPageController();

private:
	static RadioPageController*		m_instance;
	// All stations channnels
	ChannelSourceModel				m_stations;
	GenreModel						m_radioGenres;
	RadioPage*						m_view;
};

}

#endif