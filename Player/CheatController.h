#ifndef MP_CHEAT_CONTROLLER_H
#define MP_CHEAT_CONTROLLER_H

#include <QtGui/QApplication>
#include <QtDeclarative/QDeclarativeView>

namespace mp {

class MainWindow;

class CheatController : public QApplication
{
	Q_OBJECT

public:
	CheatController(int argc, char *argv[]);
	~CheatController();

	void CreateView();
	void InitSignalSlotConnections();

	static CheatController& Inst();
	
private:
	bool notify(QObject* receiver, QEvent* even);

private:
	MainWindow *			m_view;
};

}

#endif