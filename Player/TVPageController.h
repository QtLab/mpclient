#ifndef MP_TV_PAGE_CONTROLLER_H
#define MP_TV_PAGE_CONTROLLER_H

#include "IPageController.h"

namespace mp {
namespace controller {

class TVPageController : public IPageController
{
	Q_OBJECT

public:	
	TVPageController();
	~TVPageController();

	bool IsActive() const;
	view::TabPage* View() const;
	void ReLoadData();
	void Search(const QString& filter);
	void Stop();

signals:
	void FlashInstalled();

private:
	// Widget view
	view::TabPagePtr		m_view;
};

}
}

#endif