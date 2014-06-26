#include "Config.h"
#include "Common.h"
#include <Windows.h>
#include <Shlobj.h>
#include <QJsonDocument>
#include <QFile>
#include <QVariant>
#include <QDebug>
#include <QFontDatabase>
#include <QStandardPaths>

namespace mp {

Config* Config::m_instance = 0;

const QString VOLUME_KEY_NAME_FORMAT	= "volume_%0";
const QString TAB_SIZE_KEY_FORMAT		= "tabsize_%0";
const QString SOURCE_KEY_NAME			= "source";
const QString USER_ID_KEY_NAME			= "userid";
const QString VERSION_KEY_NAME			= "version";
const QString TRACKS_PATH_KEY_NAME		= "trackspath";

Config& Config::Inst()
{
	if(m_instance == nullptr)
	{
		m_instance = new Config();
	}

	return *m_instance;
}

Config::Config()
	:m_settings("Unisonbox", "Player")
{
	int id = QFontDatabase::addApplicationFont(":/mp/Resources/txdJ2vM9.ttf");
	QString family = QFontDatabase::applicationFontFamilies(id).at(0);
	m_defaultFont = QFont(family);

	Load();
}

QString Config::UserId() const
{
	return m_settings.value(USER_ID_KEY_NAME).toString();
}

QString Config::Source() const
{
	return m_settings.value(SOURCE_KEY_NAME).toString();
}

QString Config::Version() const
{
	QString version = m_settings.value(VERSION_KEY_NAME).toString();
	return version;
}

void Config::SetVersion(const QString& version)
{
	m_settings.setValue(VERSION_KEY_NAME, version);
}

const QFont& Config::DefaultFont() const
{
	return m_defaultFont;
}

void Config::SetVolume(qreal value, const QString& streamName)
{
	QString key = VOLUME_KEY_NAME_FORMAT.arg(streamName);
	m_settings.setValue(key, value);

	emit VolumeChanged(value, streamName);
}

qreal Config::Volume(const QString& streamName) const
{
	QString key = VOLUME_KEY_NAME_FORMAT.arg(streamName);

	qreal value = m_settings.value(key, 0.5f).toReal();
	return value;
}

QSize Config::TabSize(const QString& name, const QSize& defaultSize) const
{
	QString key = TAB_SIZE_KEY_FORMAT.arg(name);
	QSize size = m_settings.value(key, defaultSize).toSize();
	return size;
}

void Config::SetTabSize(const QString& name, const QSize& size)
{
	QString key = TAB_SIZE_KEY_FORMAT.arg(name);
	m_settings.setValue(key, size);
}

QString Config::PathToSaveTracks() const
{
	QString downlaodLocation = QStandardPaths::standardLocations(QStandardPaths::DownloadLocation).first();
	QString pathToSaveTracks = m_settings.value(TRACKS_PATH_KEY_NAME, downlaodLocation).toString();
	return pathToSaveTracks;
}

void Config::SetPathToSaveTracks(const QString& path)
{
	 m_settings.setValue(TRACKS_PATH_KEY_NAME, path);
}

void Config::Load()
{
	CHAR configPath[MAX_PATH];
	if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_COMMON_APPDATA, NULL, 0, configPath)))
	{
		sprintf_s(configPath, "%s\\%s", configPath, CONFIG_DIR);
		sprintf_s(configPath, "%s\\config.json", configPath);

		QFile file(configPath);
		if(file.open(QIODevice::ReadOnly))
		{
			QByteArray json = file.readAll();

			QJsonParseError parseResult;
			QJsonDocument d = QJsonDocument::fromJson(json, &parseResult);
			
			if(parseResult.error == QJsonParseError::NoError)
			{
				QVariantMap settingsMap = d.toVariant().toMap();
				QString userId = settingsMap["UserId"].toString();
				QString source = settingsMap["Source"].toString();

				if(!userId.isEmpty() && userId != m_settings.value(USER_ID_KEY_NAME).toString())
				{
					m_settings.setValue(USER_ID_KEY_NAME, userId);
				}

				if(!source.isEmpty() && source != m_settings.value(SOURCE_KEY_NAME).toString())
				{
					m_settings.setValue(SOURCE_KEY_NAME, source);
				}
			}
			else
			{
				qDebug() << configPath <<  "Config parse error: " << parseResult.errorString();
			}
		}
		else
		{
			qDebug() << "Can't load: " << configPath;
		}
	}
	else
	{
		qDebug() << "Config::Load() error";
	}
}


}