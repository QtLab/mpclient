#pragma once

#include <string>
#include <vector>

#include "FileToUpdate.h"

namespace ldr {

class HttpApi
{
public:
	HttpApi();
	String Domain() const;
	String CreateNewUser();
	bool GetUpdateInfo(const String& userId, FilesToUpdate& filesToUpdate);
	bool GetPakageInfo(const String& userId, FileToUpdate& pakage);
	bool DownloadFile(FileToUpdatePtr fileToDownlaod);

private:
	bool DoGetRequest(const String& domain, const String& query, std::string& data);
	bool DoGetRequest(const String& domain, const String& query, std::ostream& ostream);
	
private:
	String		m_domain;

};

}