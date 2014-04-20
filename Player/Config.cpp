#include "Config.h"
#include "Common.h"
#include <Windows.h>
#include <Shlobj.h>
#include <QJsonDocument>
#include <QFile>
#include <QVariant>
#include <QDebug>

namespace mp {

Config* Config::m_instance = 0;

Config& Config::Inst()
{
	if(m_instance == nullptr)
	{
		m_instance = new Config();
	}

	return *m_instance;
}

Config::Config()
{
	Load();
}

const QString& Config::UserId() const
{
	return m_userId;
}

const QString& Config::Source() const
{
	return m_source;
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
				m_userId = settingsMap["UserId"].toString();
				m_source = settingsMap["Source"].toString();
			}
			else
			{
				qDebug() << configPath <<  " parse error: " << parseResult.errorString();
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