#include "Config.h"
#include "Path.h"

#include "json/json.h"

#include <shlobj.h>
#include <sstream>
#include <fstream>

namespace ldr {

Config::Config()
	:m_hasInstallSuccessfull(false)
{
}

String Config::ConfigFilePath()
{
	CHAR configPath[MAX_PATH];
	if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_COMMON_APPDATA, NULL, 0, configPath)))
	{
		sprintf(configPath, "%s\\%s", configPath, APP_NAME);

		if(!Path::DirectoryExists(configPath))
		{
			if(!Path::Make(configPath))
			{				
				// Write to log
				return false;
			}
		}

		sprintf(configPath, "%s\\config.json", configPath, APP_NAME);
		return configPath;
	}

	return String();
}

bool Config::ReLoad()
{
	String configPath = ConfigFilePath();

	std::ifstream stream(configPath);

	if(stream.is_open())
	{
		std::cout << "Config: " << configPath << " laoded successfull" << std::endl;

		std::string configDocument;
		configDocument.assign((std::istreambuf_iterator<char>(stream)), 
								std::istreambuf_iterator<char>());

		Json::Value root;
		Json::Reader reader;

		if(!reader.parse(configDocument, root))
		{
			//TODO: logging
			return false;
		}

		m_userId = root["UserId"].asString();
		m_source = root["Source"].asString();
		m_hasInstallSuccessfull = root["HasSuccessfullInstall"].asBool();
		return true;
	}

	return false;
}

bool Config::Save()
{
	Json::Value root; 
	root["UserId"] = m_userId;
	root["Source"] = m_source;
	root["HasSuccessfullInstall"] = m_hasInstallSuccessfull;

	String configPath = ConfigFilePath();

	std::ofstream ostream(configPath, std::ios::out | std::ios::binary | std::ios::trunc);
	ostream << root;

	bool good = ostream.good();

	std::cout << "Config: " << configPath << " saved successfull" << std::endl;

	return good;
}

const std::string& Config::UserId() const
{
	return m_userId;
}

void Config::SetUserId(const String& id)
{
	m_userId = id;
	Save();
}

const std::string& Config::Source() const
{
	return m_source;
}

void Config::SetSource(const String& source)
{
	m_source = source;
}

bool Config::InstallSuccessfull() const
{
	return m_hasInstallSuccessfull;
}

void Config::SetInstallSuccessfull(bool success)
{
	m_hasInstallSuccessfull = success;
	Save();
}

}