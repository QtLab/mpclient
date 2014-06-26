#ifndef MP_REQUEST_H
#define MP_REQUEST_H

#include <QObject>
#include <QMetaType>
#include <QUrl>
#include <QString>
#include <QByteArray>

namespace mp {

// Url for request, he may have a link to the body of the request
class UrlBuilder
{
public:
	static QUrl DefaultHostUrl();
	static void SetUserId(QUrl& url, const QString& userId);
	static void SetQueryParameter(QUrl& url, const QString& key, const QString& value);

	static QUrl CreateUpdate(const QString& userId);
	static QUrl CreateCurrentDirUrl(const QString& relativePath);
	static QUrl CreateInixMedia(const QString& filter);
	static QUrl CreateMp3Xml(const QString& filter);
	static QUrl CreateSetUserInfo(const QString& userId, const QString& version, const QString& source, bool isInstall);
};

}

#endif