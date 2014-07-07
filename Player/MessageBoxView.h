#ifndef MP_MESSAGE_BOX_VIEW_H
#define MP_MESSAGE_BOX_VIEW_H

#include "Prerequirements.h"
#include <QObject>

namespace mp {
namespace view {

class MessageBoxView : public QObject
{
	Q_OBJECT
public:
	static void ShowAbout(QWidget* parent = NULL);
	static void ShowNewVersionFound(QWidget* parent = NULL);
	static void ShowNewVersionNotFound(QWidget* parent = NULL);
};

}
}

#endif