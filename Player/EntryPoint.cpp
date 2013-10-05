#include "AppController.h"
#include "Log.h"

int main(int argc, char *argv[])
{
	mp::AppController app(argc, argv);

#ifndef _DEBUG
	// ����� � ��� ���� ������ � ������
	qInstallMsgHandler(CustomMessageHandler);
#endif

	app.CreateView();
	app.InitSignalSlotConnections();

	return app.exec();
}

