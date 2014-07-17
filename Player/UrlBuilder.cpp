#include "UrlBuilder.h"
#include "Common.h"
#include <QDir>
#include <QUrlQuery>

namespace mp {

static const QString DEFAULT_HOST_N(DEFAULT_HOST);
static const QString USERID_PK("UserId");
static const QString HTTP_SCHEME("http");

QUrl UrlBuilder::DefaultHostUrl()
{
	QUrl url;
	url.setScheme(HTTP_SCHEME);
	url.setHost(DEFAULT_HOST_N);

	return url;
}

void UrlBuilder::SetUserId(QUrl& url, const QString& userId)
{
	SetQueryParameter(url, USERID_PK, userId);
}

void UrlBuilder::SetQueryParameter(QUrl& url, const QString& key, const QString& value)
{
	QUrlQuery urlQuery(url);
	urlQuery.removeQueryItem(key);
	urlQuery.addQueryItem(key, value);
	url.setQuery(urlQuery);
}

QUrl UrlBuilder::CreateUpdate(const QString& userId)
{
	QUrl url = DefaultHostUrl();
	url.setPath("/Handlers/GetDataFile.ashx", QUrl::ParsingMode::TolerantMode);
	SetUserId(url, userId);
	
	return url;
}

QUrl UrlBuilder::CreateCurrentDirUrl(const QString& relativePath)
{
	QString current = QDir::current().filePath(relativePath);
	return QUrl::fromLocalFile(current);
}

QUrl UrlBuilder::CreateInixMedia(const QString& filter)
{
	static const QString INIXMEDIA_DOMAIN("inixmedia.net");
	static const QString INIXMEDIA_QUERY("/vk_music.php");
	static const QString INIXMEDIA_Q("q");
	static const QString INIXMEDIA_LAST("last");
	static const QString INIXMEDIA_ZERO("0");

	QUrl url;
	url.setScheme(HTTP_SCHEME);
	url.setHost(INIXMEDIA_DOMAIN);
	url.setPath(INIXMEDIA_QUERY, QUrl::ParsingMode::TolerantMode);

	QUrlQuery urlQuery(url);
	urlQuery.addQueryItem(INIXMEDIA_Q, filter);
	urlQuery.addQueryItem(INIXMEDIA_LAST, INIXMEDIA_ZERO);
	url.setQuery(urlQuery);

	return url;
}

QUrl UrlBuilder::CreateMp3Xml(const QString& filter)
{
	static const QString MP3XML_DOMAIN("mp3xml.com");
	static const QString MP3XML_QUERY("/search-xml.php");
	static const QString MP3XML_Q("q");
	static const QString MP3XML_KEY("key");
	static const QString MP3XML_KEY_VALUE("Ghf47BGfd4");
	static const QString MP3XML_SUBID("subid");
	static const QString MP3XML_SUBID_VALUE("soft");

	QUrl url;
	url.setScheme(HTTP_SCHEME);
	url.setHost(MP3XML_DOMAIN);
	url.setPath(MP3XML_QUERY, QUrl::ParsingMode::TolerantMode);

	QUrlQuery urlQuery(url);
	urlQuery.addQueryItem(MP3XML_Q, filter);
	urlQuery.addQueryItem(MP3XML_KEY, MP3XML_KEY_VALUE);
	urlQuery.addQueryItem(MP3XML_SUBID, MP3XML_SUBID_VALUE);
	url.setQuery(urlQuery);

	return url;
}

QUrl UrlBuilder::CreateSetUserInfo(const QString& userId, const QString& version, const QString& source, bool isInstall)
{
	QUrl url = DefaultHostUrl();
	url.setPath("/Handlers/SetUserInfo.ashx", QUrl::ParsingMode::TolerantMode);

	QUrlQuery urlQuery(url);
	urlQuery.addQueryItem(USERID_PK, userId);
	urlQuery.addQueryItem("UserSource", source);
	urlQuery.addQueryItem("UserVersion", version);

	if(isInstall)
	{
		urlQuery.addQueryItem("UserSetInstalled", "true");
	}

	url.setQuery(urlQuery);

	return url;
}

QUrl UrlBuilder::CreateFlashMSIUrl()
{
	static QUrl url("http://download.macromedia.com/get/flashplayer/current/licensing/win/install_flash_player_11_plugin.msi");
	return url;
}

}