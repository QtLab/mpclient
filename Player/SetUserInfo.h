#ifndef MP_SET_USER_INFO_H
#define MP_SET_USER_INFO_H

#include "Prerequirements.h"

namespace mp {

class SetUserInfo : public QObject
{
	Q_OBJECT

public:
	static void SendIfVersionChanged();
	static void SendInstall();

private:
	SetUserInfo(const QString& currentVersion, bool isInstall);

private slots:
	void SetUserInfoHttpRequestFinished();
};

}

#endif