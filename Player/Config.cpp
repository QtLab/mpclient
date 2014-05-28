#include "Config.h"
#include "Common.h"
#include <Windows.h>
#include <Shlobj.h>
#include <QJsonDocument>
#include <QFile>
#include <QVariant>
#include <QDebug>
#include <QFontDatabase>

namespace mp {

Config* Config::m_instance = 0;

const QString VolumeKeyName			= "volume";
const QString TVTabSizeKeyName		= "tvtabsize";
const QString SourceKeyName			= "source";
const QString UserIdKeyName			= "userid";

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
	return m_settings.value(UserIdKeyName).toString();
}

QString Config::Source() const
{
	return m_settings.value(SourceKeyName).toString();
}

const QFont& Config::DefaultFont() const
{
	return m_defaultFont;
}

void Config::SetVolume(qreal value)
{
	m_settings.setValue(VolumeKeyName, value);

	emit VolumeChanged(value);
}

qreal Config::Volume() const
{
	qreal value = m_settings.value(VolumeKeyName, 0.5f).toReal();
	return value;
}

void Config::SetTVTabWindowSize(const QSize& size)
{
	m_settings.setValue(TVTabSizeKeyName, size);
}

QSize Config::TVTabWindowSize() const
{
	QSize size = m_settings.value(TVTabSizeKeyName, QSize(750, 630)).toSize();
	return size;
}

QSize Config::RadioTabWindowSize() const
{
	return QSize(580, 351);
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

				if(!userId.isEmpty() && userId != m_settings.value(UserIdKeyName).toString())
				{
					m_settings.setValue(UserIdKeyName, userId);
				}

				if(source.isEmpty() && source != m_settings.value(SourceKeyName).toString())
				{
					m_settings.setValue(SourceKeyName, source);
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