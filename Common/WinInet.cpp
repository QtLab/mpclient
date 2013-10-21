#include "Common.h"
#include <Wininet.h>
#include <iostream>
#include <fstream>

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
	CATCH_ALL_EXCEPTIONS();
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
	CATCH_ALL_EXCEPTIONS();
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
	CATCH_ALL_EXCEPTIONS();
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
	}
	CATCH_ALL_EXCEPTIONS();

	return true;
}

bool HttpDownload(const std::wstring& url, std::string * data)
{
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
