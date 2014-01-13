#ifndef MP_APP_CONTROLLER_H
#define MP_APP_CONTROLLER_H

#include <QApplication>
#include <QtDeclarative/QDeclarativeView>

namespace mp {

class MainWindow;

class AppController : public QApplication
{
	Q_OBJECT

public:
	AppController(int argc, char *argv[]);
	~AppController();

	void CreateView();
	void InitSignalsSlots();

	static AppController& Inst();
	
private:
	bool notify(QObject* receiver, QEvent* even);

private:
	MainWindow *			m_view;
};

}

#endif