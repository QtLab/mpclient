#include "HttpApi.h"
#include "Defs.h"
#include "Path.h"
#include "Hash.h"
#include "json/json.h"

#include <Windows.h>
#include <Wininet.h>
#include <sstream>
#include <fstream>

namespace ldr {

using namespace cmn;

HttpApi::HttpApi()
	:m_domain(DEFAULT_HOST)
{
}

String HttpApi::Domain() const
{
	return m_domain;
}

String HttpApi::CreateNewUser()
{
	std::string response;
	DoGetRequest(Domain(), "/Handlers/RegisterNewUser.ashx", response);

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


bool HttpApi::GetUpdateInfo(const std::string& userId, FilesToUpdate& fielsToUpdate)
{
	std::cout << "Get update info started..." << std::endl;

	std::string response;
	DoGetRequest(Domain(), STR"/Handlers/GetDataFile.ashx?UserId=" + userId, response);

	Json::Value root;
	Json::Reader reader;

	if(!reader.parse(response, root))
	{
		std::cout << "Parsing json error: " << reader.getFormatedErrorMessages() << std::endl;
		std::cout << "json: " << response;

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
				if(fileData->MD5() == STR"0")
				{
					fileData->Delete();
					delete fileData;
				}
				else
				{
					fielsToUpdate.push_back(fileData);
				}
			}
		}
	}

	return true;
}

bool HttpApi::GetPakageInfo(const String& userId, FileToUpdate& pakage)
{
	std::cout << "Get pakage info started..." << std::endl;

	std::string response;
	DoGetRequest(Domain(), STR"/Handlers/GetDataFile.ashx?IsZip=true&UserId=" + userId, response);

	Json::Value root;
	Json::Reader reader;

	if(!reader.parse(response, root))
	{
		std::cout << "Parsing json error: " << reader.getFormatedErrorMessages() << std::endl;
		std::cout << "json: " << response;

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

bool HttpApi::DownloadFile(FileToUpdatePtr fileToDownlaod)
{
	std::cout << "Starting downlaod file: " << fileToDownlaod->AbsolutePath() 
				<< " from: " << fileToDownlaod->Url() << std::endl;

	DeleteUrlCacheEntry(fileToDownlaod->Url().c_str());

	std::string directoryPath = fileToDownlaod->AbdoluteDirectory();
	if(!Path::DirectoryExists(directoryPath))
	{
		Path::Make(directoryPath);
		std::cout << "Create directory: " << directoryPath << " result: " << GetLastErrorString() << std::endl;
	}

	String tmpFilePath = fileToDownlaod->AbsolutePath() + STR"tmp";

	std::ofstream ostream(tmpFilePath, std::ios::out | std::ios::binary | std::ios::trunc);
	if(DoGetRequest(fileToDownlaod->Domain(), fileToDownlaod->Query(), ostream))
	{
		ostream.flush();
		ostream.close();

		String md5;
		if(Hash::ComputeFileMD5(tmpFilePath, md5))
		{
			if(IsEquals(md5, fileToDownlaod->MD5()))
			{
				if(!DeleteFile(fileToDownlaod->AbsolutePath().c_str()))
				{
					MoveFile(fileToDownlaod->AbsolutePath().c_str(), (fileToDownlaod->AbsolutePath() + STR"old").c_str());
				}

				BOOL result = MoveFile(tmpFilePath.c_str(), fileToDownlaod->AbsolutePath().c_str());

				if(!result)
				{
					std::cout << "Can't move file: " << tmpFilePath
								<< " to: " << fileToDownlaod->AbsolutePath()
								<< " error: " << GetLastErrorString() << std::endl;
				}
				else
				{
					return true;
				}
			}
			else
			{
				std::cout << tmpFilePath  << " md5: " << md5 
							<< fileToDownlaod->AbsolutePath() << " md5: " << fileToDownlaod->MD5() 
							<< std::endl;
			}
		}
		else
		{
			std::cout << "Can't compute md5 for: " << tmpFilePath << std::endl;
		}
	}

	return false;
}

bool HttpApi::DoGetRequest(const String& domain, const String& query, std::ostream& ostream)
{
	HINTERNET hIntSession = ::InternetOpen(APP_UA, INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);

	if(!hIntSession)
	{
		std::cout << "InternetOpen error: " << GetLastErrorString() << std::endl;
		::InternetCloseHandle(hIntSession);
		return false;
	}

	HINTERNET hHttpSession = InternetConnect(hIntSession, domain.c_str(), 80, 0, 0, INTERNET_SERVICE_HTTP, 0, NULL);

	if(!hHttpSession)
	{
		std::cout << "InternetConnect error: " << GetLastErrorString() << std::endl;
		::InternetCloseHandle(hHttpSession);
		return false;
	}

	HINTERNET hHttpRequest = HttpOpenRequest(
		hHttpSession, 
		GET_OPERATION,
		query.c_str(),
		0, 0, 0, INTERNET_FLAG_RELOAD | INTERNET_FLAG_NO_CACHE_WRITE | INTERNET_FLAG_PRAGMA_NOCACHE, 0);

	if(!hHttpRequest)
	{
		std::cout << "HttpOpenRequest error: " << GetLastErrorString() << std::endl;
		::InternetCloseHandle(hHttpRequest);
		::InternetCloseHandle(hHttpSession);
		::InternetCloseHandle(hIntSession);
		return false;
	}

	if(!HttpSendRequest(hHttpRequest, NULL, 0, NULL, 0))
	{
		std::cout << "HttpSendRequest error: " << GetLastErrorString() << std::endl;
		::InternetCloseHandle(hHttpRequest);
		::InternetCloseHandle(hHttpSession);
		::InternetCloseHandle(hIntSession);
		return false;
	}

	char szBuffer[32768];
	DWORD dwRead=0;
	while(::InternetReadFile(hHttpRequest, szBuffer, sizeof(szBuffer), &dwRead) && dwRead)
	{
		if(GetLastError() != 0)
		{
			std::cout << "InternetReadFile error: " << GetLastErrorString() << std::endl;
			::InternetCloseHandle(hHttpRequest);
			::InternetCloseHandle(hHttpSession);
			::InternetCloseHandle(hIntSession);
			return false;
		}

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
