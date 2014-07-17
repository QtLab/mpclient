#ifndef MP_IPAGE_CONTROLLER_H
#define MP_IPAGE_CONTROLLER_H

#include "Prerequirements.h"
#include <QMap>

namespace mp {
namespace controller {

class IPageController : public QObject
{
	Q_OBJECT

public:	
	IPageController() {}
	virtual ~IPageController() {}

	virtual bool IsActive() const= 0;
	virtual view::TabPage* View() const = 0;
	virtual void ReLoadData() = 0;
	virtual void Pause() = 0;
	virtual void Search(const QString& filter) = 0;

signals:
	void PauseAllControllers();
};

IPageControllerPtr CreatePageController(const QString& name);

} //namespace controller
} //namespace mp

#endif