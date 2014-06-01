#include "HttpApi.h"
#include "Defs.h"
#include "Path.h"

#include "json/json.h"

#include <Windows.h>
#include <Wininet.h>
#include <sstream>
#include <fstream>

namespace ldr {

HttpApi::HttpApi()
	:m_domain("109.120.163.35")
{
}

String HttpApi::Domain() const
{
	return m_domain;
}

String HttpApi::CreateNewUser(const String& source)
{
	std::string response;
	DoGetRequest(Domain(), "/Handlers/RegisterUpdateUser.ashx?Source=" + source, response);

	Json::Value root;
	Json::Reader reader;

	if(!reader.parse(response, root))
	{
		//TODO: logging
		return std::string();
	}
	
	std::string guid = root["I"].asString();
	return guid;
}

void HttpApi::UpdateUser(const String& guid, const String& source)
{
	std::string response;
	DoGetRequest(Domain(), "/Handlers/RegisterUpdateUser.ashx?UserId=" + guid + "Source=" + source, response);
}

bool HttpApi::GetUpdate(const std::string& userId, FilesToUpdate& fielsToUpdate)
{
	std::string response;
	DoGetRequest(Domain(), "/Handlers/GetDataFile.ashx?UserId=" + userId, response);

	Json::Value root;
	Json::Reader reader;

	if(!reader.parse(response, root))
	{
		//TODO: logging
		return false;
	}
	
	Json::Value files = root["F"];

	if(files.type() == Json::arrayValue)
	{
		for( Json::ValueIterator itr = files.begin() ; itr != files.end() ; itr++ ) 
		{
			FileToUpdate * fileData = FileToUpdate::CreateFromJsonValue((*itr));
			if(fileData)
			{
				fielsToUpdate.push_back(fileData);
			}
		}
	}

	return true;
}

bool HttpApi::GetPakage(const String& userId, FileToUpdate& pakage)
{
	std::string response;
	DoGetRequest(Domain(), "/Handlers/GetDataFile.ashx?IsZip=true&UserId=" + userId, response);

	Json::Value root;
	Json::Reader reader;

	if(!reader.parse(response, root))
	{
		//TODO: logging
		return false;
	}

	Json::Value pakageJson = root["F"];
	if(!pakageJson.isNull())
	{
		pakageJson = pakageJson[(Json::UInt)0];
		return FileToUpdate::FillFromJsonValue(&pakage, pakageJson);
	}

	return false;
}

bool HttpApi::DownloadFile(const FileToUpdate& fileToDownlaod)
{
	DeleteUrlCacheEntry(fileToDownlaod.Url().c_str());

	std::string directoryPath = Path::DirectoryFromFilePath(fileToDownlaod.AbsolutePath());
	if(!Path::DirectoryExists(directoryPath))
	{
		Path::Make(directoryPath);
	}

	String oldFilePath = fileToDownlaod.AbsolutePath() + "old";

	if(Path::FileExists(fileToDownlaod.AbsolutePath()))
	{
		MoveFile(fileToDownlaod.AbsolutePath().c_str(), oldFilePath.c_str());
		DWORD err = GetLastError();
		std::cout << err;
	}

	std::ofstream ostream(fileToDownlaod.AbsolutePath(), std::ios::out | std::ios::binary | std::ios::trunc);
	if(DoGetRequest(fileToDownlaod.Domain(), fileToDownlaod.Query(), ostream))
	{
		DeleteFile(oldFilePath.c_str());
	}

	return true;
}

bool HttpApi::DoGetRequest(const String& domain, const String& query, std::ostream& ostream)
{
	HINTERNET hIntSession = ::InternetOpen(APP_UA, INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);

	HINTERNET hHttpSession = InternetConnect(hIntSession, domain.c_str(), 80, 0, 0, INTERNET_SERVICE_HTTP, 0, NULL);

	HINTERNET hHttpRequest = HttpOpenRequest(
		hHttpSession, 
		GET_OPERATION,
		query.c_str(),
		0, 0, 0, INTERNET_FLAG_RELOAD | INTERNET_FLAG_NO_CACHE_WRITE | INTERNET_FLAG_PRAGMA_NOCACHE, 0);

	if(!HttpSendRequest(hHttpRequest, NULL, 0, NULL, 0))
	{
		DWORD dwErr = GetLastError();
		return false;
	}

	CHAR szBuffer[1024];
	DWORD dwRead=0;
	while(::InternetReadFile(hHttpRequest, szBuffer, sizeof(szBuffer), &dwRead) && dwRead)
	{
		ostream.write((const char*)szBuffer, dwRead);
	}

	::InternetCloseHandle(hHttpRequest);
	::InternetCloseHandle(hHttpSession);
	::InternetCloseHandle(hIntSession);

	return true;
}

bool HttpApi::DoGetRequest(const String& domain, const String& query, std::string& data)
{
	std::ostringstream stream;
	if(DoGetRequest(domain, query, stream))
	{
		data = stream.str();
		return true;
	}

	return false;
}

}
