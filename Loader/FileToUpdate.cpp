#include "FileToUpdate.h"
#include "Path.h"

#include <Wininet.h>

namespace ldr {

bool CrackUrl(const String& url, String& domain, String& query)
{
	URL_COMPONENTS uc = {};

	TCHAR HostName[1000];
	TCHAR UrlPath[1000];

	uc.dwStructSize = sizeof(uc);
	uc.lpszHostName = HostName;
	uc.lpszUrlPath = UrlPath;

	uc.dwHostNameLength = 1000;
	uc.dwUrlPathLength = 1000;

	if(InternetCrackUrl(url.c_str(), url.length(), 0, &uc))
	{
		domain = HostName;
		query = UrlPath;

		return true;
	}

	return false;
}

const String& FileToUpdate::Domain() const
{
	return m_domain;
}

const String& FileToUpdate::Query() const
{
	return m_query;
}

void FileToUpdate::SetUrl(const String& url)
{
	CrackUrl(url, m_domain, m_query);
	m_url = url;
}

void FileToUpdate::SetRelativePath(const String& relativePath)
{
	m_relativePath = relativePath;
	m_absolutePath = Path::CurrentPathCombine(relativePath);
}

void FileToUpdate::SetMD5(const String& md5)
{
	m_md5 = md5;
}

const String& FileToUpdate::Url() const 
{ 
	return m_url; 
}

const String& FileToUpdate::RelativePath() const 
{
	return m_relativePath;
}

const String& FileToUpdate::AbsolutePath() const
{
	return m_absolutePath;
}

String FileToUpdate::AbdoluteDirectory() const
{
	return Path::DirectoryFromFilePath(AbsolutePath());
}

const String& FileToUpdate::MD5() const 
{ 
	return m_md5;
}

bool FileToUpdate::FillFromJsonValue(FilesToUpdatePtr fileToUpdate, const Json::Value& value)
{
	if(value.type() == Json::objectValue)
	{
		Json::Value md5 = value["M"];
		Json::Value url = value["U"];
		Json::Value name = value["N"];

		if(!md5.isNull() && !url.isNull()  && !name.isNull())
		{
			fileToUpdate->SetUrl(url.asString());
			fileToUpdate->SetMD5(md5.asString());
			fileToUpdate->SetRelativePath(name.asString());

			return true;
		}
	}

	return false;
}

FilesToUpdatePtr FileToUpdate::CreateFromJsonValue(const Json::Value& value)
{
	if(value.type() == Json::objectValue)
	{
		Json::Value md5 = value["M"];
		Json::Value url = value["U"];
		Json::Value name = value["N"];

		if(!md5.isNull() && !url.isNull()  && !name.isNull())
		{
			FileToUpdate * fd = new FileToUpdate();
			fd->SetUrl(url.asString());
			fd->SetMD5(md5.asString());
			fd->SetRelativePath(name.asString());

			return fd;
		}
	}

	return FilesToUpdatePtr();
}

}