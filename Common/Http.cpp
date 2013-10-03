#include "Common.h"
#include <Wininet.h>
#include <iostream>
#include <fstream>
#include <list>

HINTERNET netstart ()
{
	try
	{
		const HINTERNET handle =
			InternetOpenA(0, INTERNET_OPEN_TYPE_DIRECT, 0, 0, 0);
		if ( handle == 0 )
		{
			const ::DWORD error = ::GetLastError();
			std::cerr
				<< "InternetOpen(): " << error << "."
				<< std::endl;
		}
		return (handle);
	}
	catch(std::exception e)
	{
		Log((std::string(__FUNCDNAME__) + " unhanded exception: " + e.what()).c_str());
	}
}

void netclose ( HINTERNET object )
{
	try
	{
		const BOOL result = InternetCloseHandle(object);
		if ( result == FALSE )
		{
			const DWORD error = GetLastError();
			std::cerr
				<< "InternetClose(): " << error << "."
				<< std::endl;
		}
	}
	catch(std::exception e)
	{
		Log((std::string(__FUNCDNAME__) + " unhanded exception: " + e.what()).c_str());
	}
}

HINTERNET netopen ( HINTERNET session, LPCWSTR url )
{
	try
	{
		const ::HINTERNET handle =
			::InternetOpenUrlW(session, url, 0, 0, 0, 0);
		if ( handle == 0 )
		{
			const ::DWORD error = ::GetLastError();
			std::cerr
				<< "InternetOpenUrl(): " << error << "."
				<< std::endl;
		}

		return (handle);
	}
	catch(std::exception e)
	{
		Log((std::string(__FUNCDNAME__) + " unhanded exception: " + e.what()).c_str());
	}
}

bool netfetch ( HINTERNET istream, std::string * out )
{
	try
	{
		static const ::DWORD SIZE = 1024;
		DWORD error = ERROR_SUCCESS;
		BYTE data[SIZE];
		DWORD size = 0;
		do {
			BOOL result = InternetReadFile(istream, data, SIZE, &size);
			if ( result == FALSE )
			{
				error = GetLastError();
				std::cerr
					<< "InternetReadFile(): " << error << "."
					<< std::endl;

				return false;
			}

			out->append((char*)data, size);
		}
		while ((error == ERROR_SUCCESS) && (size > 0));

		return true;
	}
	catch(std::exception e)
	{
		Log((std::string(__FUNCDNAME__) + " unhanded exception: " + e.what()).c_str());
	}
}

bool HttpDownload(const std::wstring& url, std::string * data)
{
	try
	{
		DeleteUrlCacheEntry(url.c_str());
		DeleteUrlCacheEntry(url.c_str());

		HINTERNET session = netstart();
		if(session)
		{
			const HINTERNET istream = netopen(session, url.c_str());

			if(istream)
			{
				if(!netfetch(istream, data))
				{
					data->clear();
					netclose(session);
					return false;
				}

				netclose(session);
				return true;
			}
		}

		return false;
	}
	catch(std::exception e)
	{
		Log((std::string(__FUNCDNAME__) + " unhanded exception: " + e.what()).c_str());
	}
}

void SendBugReport()
{
	try
	{
		SYSTEMTIME st;
		GetSystemTime (&st);
		TCHAR buff[20];

		TCHAR path[MAX_PATH];
		GetCurrentDirectory(MAX_PATH, path);
		wsprintf(path, L"%s\\logs\\%02d-%02d-%d.log", path, st.wDay, st.wMonth, st.wYear);

		if(FileExists(path))
		{
			std::ifstream hFile(path);

			std::list<std::string> lines;
			std::list<std::string>::iterator iter;

			std::string line;
			while(std::getline(hFile, line))
			{
				lines.push_back(line);
				if(lines.size() > 40)
				{
					lines.pop_front();
				}
			}

			iter = lines.begin();

			std::string log;
			while(iter != lines.end())
			{
				log.append((*iter));
				iter++;
			}

			std::wstring url(L"http://zaxargames.com/client/errors.php?log=");
			url += StringToWstring(log);
			url += L"&guid=" + GetGuid();
			url += L"&email=" + GetEmail();
			std::string resp;

			HttpDownload(url, &resp);
		}
	}
	catch(std::exception e)
	{
		Log((std::string(__FUNCDNAME__) + " unhanded exception: " + e.what()).c_str());
	}
}

std::wstring mDoaminToUpdate = L"http://www.zaxargames.com";

void SetDomainToUpdate(const std::wstring& domain)
{
	mDoaminToUpdate = domain;
}

std::wstring DomainToUpdate()
{
	return mDoaminToUpdate;
}
