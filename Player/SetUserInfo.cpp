#include "SetUserInfo.h"
#include "DownlaodManager.h"
#include "UrlBuilder.h"
#include "Config.h"

#include <QDebug>
#include <QNetworkReply>

namespace mp {

void SetUserInfo::SendIfVersionChanged()
{
	QString currentVersion(PLAYER_VERSION);
	QString versionFromConfig(Config::Inst().Version());

	if(currentVersion.compare(versionFromConfig) != 0)
	{
		new SetUserInfo(currentVersion, false);
	}
}

void SetUserInfo::SendInstall()
{
	new SetUserInfo(QString(PLAYER_VERSION), true);
}

SetUserInfo::SetUserInfo(const QString& currentVersion, bool isInstall)
{
	QString userId = Config::Inst().UserId();
	QString source = Config::Inst().Source();

	QUrl url = UrlBuilder::CreateSetUserInfo(userId, currentVersion, source, isInstall);
	DownlaodManager::Global().Get(url, this, SLOT(SetUserInfoHttpRequestFinished()));
}

void SetUserInfo::SetUserInfoHttpRequestFinished()
{
	QNetworkReply * reply = qobject_cast<QNetworkReply*>(sender());

	if(reply->error() == QNetworkReply::NoError)
	{
		Config::Inst().SetVersion(QString(PLAYER_VERSION));
	}
	else
	{
		qDebug() << "SetUserInfoHttpRequestFinished error: " << reply->errorString();
	}

	reply->deleteLater();
	this->deleteLater();
}

}