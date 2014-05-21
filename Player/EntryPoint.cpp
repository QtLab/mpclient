#include "AppController.h"
#include "WebView.h"
#include "Config.h"
#include "Log.h"

#include <QDir>

int main(int argc, char *argv[])
{
#ifndef _DEBUG
	// Write to the log file only in release mode
	qInstallMessageHandler(mp::Log);
#endif

	mp::AppController app(argc, argv);

	QDir::setCurrent(QCoreApplication::applicationDirPath());

	mp::WebView::SetupGlobalSettings();

	app.CreateView();
	app.InitSignalsSlots();

	return app.exec();
}

/*
#include <QtXml/QDomDocument>
#include <QJsonDocument>
#include <QJsonObject>

#include "CategoriesModel.h"
#include "ChannelSourceModel.h"

void GenresXmlToJson()
{
	QJsonDocument document;
	QJsonArray jsonArray;

	QFile dat("categories.xml");
	if(dat.open(QIODevice::ReadOnly))
	{
		QDomDocument dom;
		QString error;
		dom.setContent(&dat, false, &error);	

		QDomNode response = dom.namedItem("pcradio");

		QDomNode genres = response.namedItem("genres");
		QDomNodeList genresList = genres.childNodes();
		for(int i =0; i < genresList.count(); i++)
		{
			QDomNode genre = genresList.at(i);
			int id = genre.namedItem("id").firstChild().nodeValue().toInt();
			QString name = genre.namedItem("name").firstChild().nodeValue();

			QJsonObject jsonObj;
			jsonObj.insert("id", QJsonValue::fromVariant(id));
			jsonObj.insert("name", QJsonValue::fromVariant(name));
			jsonArray.append(QJsonValue(jsonObj));
		}
	}

	document.setArray(jsonArray);
	QByteArray json = document.toJson();

	QFile file("radiogenres");
	if(file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		qint64 written = file.write(json);
		int toWrite = json.size();
	}
}


void StationsXmlToJson()
{
	QJsonDocument document;
	QJsonArray jsonArray;

	QFile dat("categories.xml");
	if(dat.open(QIODevice::ReadOnly))
	{
		QDomDocument dom;
		QString error;
		dom.setContent(&dat, false, &error);	

		QDomNode response = dom.namedItem("pcradio");

		QDomNode channels = response.namedItem("channels");
		QDomNodeList channelsList = channels.childNodes();
		int id = 0;
		for(int i = 0; i < channelsList.count(); i++)
		{
			QDomNode channel = channelsList.at(i);
			QString name = channel.namedItem("title").firstChild().nodeValue();
			name = name.replace("\"", "");
			QString url = channel.namedItem("url_hi").firstChild().nodeValue();
			int genreId = channel.namedItem("genre_id").firstChild().nodeValue().toInt();

			QJsonObject jsonObj;
			jsonObj.insert("id", QJsonValue::fromVariant(id));
			jsonObj.insert("name", QJsonValue::fromVariant(name));
			jsonObj.insert("url", QJsonValue::fromVariant(url));

			QJsonArray categories;
			categories.append(QJsonValue::fromVariant(genreId));
			jsonObj.insert("categories", categories); 

			jsonArray.append(QJsonValue(jsonObj));

			id++;
		}
	}

	document.setArray(jsonArray);
	QByteArray json = document.toJson();

	QFile file("radio");
	if(file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		qint64 written = file.write(json);
		int toWrite = json.size();
	}
}
*/