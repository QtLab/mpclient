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
	String CreateNewUser(const String& source);
	void UpdateUser(const String& guid, const String& source);
	bool GetUpdate(const std::string& userId, FilesToUpdate& filesToUpdate);
	bool GetPakage(const std::string& userId, FileToUpdate& pakage);
	bool DownloadFile(const FileToUpdate& fileToDownlaod);

private:
	bool DoGetRequest(const String& domain, const String& query, std::string& data);
	bool DoGetRequest(const String& domain, const String& query, std::ostream& ostream);
	
private:
	String		m_domain;

};

}