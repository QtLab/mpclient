#ifndef MP_POPUP_MENU_H
#define MP_POPUP_MENU_H

#include <QMenu>
#include <QEvent>

namespace mp {

class SystemTrayContextMenu : public QMenu
{
	Q_OBJECT

public:
	SystemTrayContextMenu(QWidget * parent);
	void changeEvent(QEvent *event);

protected:
	void Retranslate();

signals:
	void UpdateApplication();
	void AboutApplication();
	void CloseApplication();
};

}

#endif //MP_POPUP_MENU_H