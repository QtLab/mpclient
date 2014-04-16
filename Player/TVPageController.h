#ifndef MP_TV_PAGE_CONTROLLER_H
#define MP_TV_PAGE_CONTROLLER_H

#include "Prerequirements.h"

namespace mp {

class TVPageController : public QObject
{
	Q_OBJECT

public:	
	TVPageController();
	~TVPageController();

	void ReLoadData();
	TabPage* View();

public slots:

private:
	// Model - tv channnels
	ChannelSourceModelPtr				m_channels;
	// Widget view
	TabPagePtr							m_view;
};

}

#endif