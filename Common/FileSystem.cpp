#include "Common.h"

#include <stdio.h>
#include <windows.h>
#include <Wincrypt.h>
#include <algorithm >

#define BUFSIZE 1024
#define MD5LEN  16

static const std::wstring NullMD5(L"00000000000000000000000000000000");


DWORD ComputeMD5(std::wstring& md5, const std::wstring& file);

bool NullOrMd5NotEqueal(const FileData& file)
{
	try
	{
		TCHAR *filePath = (TCHAR *)file.FullPath.c_str();

		if(file.MD5.compare(NullMD5) == 0)
		{
			DeleteFile(filePath);
			return true;
		}

		if(!FileExists(filePath))
		{
			return false;
		}

		std::wstring md5;
		if(ComputeMD5(md5, file.FullPath) != 0)
		{
			return false;
		}
		else
		{
			if(file.MD5.compare(md5) == 0)
			{
				return true;
			}
		}

		return false;
	}
	CATCH_ALL_EXCEPTIONS();
}

void FilterFilesList(FilesList * files)
{
	try
	{
		files->erase(std::remove_if(files->begin(), files->end(), NullOrMd5NotEqueal), files->end());
	}
	CATCH_ALL_EXCEPTIONS();
}

bool FileExists(const std::wstring& file)
{
	try
	{
		DWORD dwAttrib = GetFileAttributes(file.c_str());

		return (dwAttrib != INVALID_FILE_ATTRIBUTES && 
			!(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
	}
	CATCH_ALL_EXCEPTIONS();
}

bool DirectoryExists(const std::wstring& dirName)
{
	try
	{
		DWORD ftyp = GetFileAttributes(dirName.c_str());
		if (ftyp == INVALID_FILE_ATTRIBUTES)
			return false;  //something is wrong with your path!

		if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
			return true;   // this is a directory!

		return false;    // this is not a directory!
	}
	CATCH_ALL_EXCEPTIONS();
}

DWORD ComputeMD5(std::wstring& md5, const std::wstring& file)
{
	try
	{
		DWORD dwStatus = 0;
		BOOL bResult = FALSE;
		HCRYPTPROV hProv = 0;
		HCRYPTHASH hHash = 0;
		HANDLE hFile = NULL;
		BYTE rgbFile[BUFSIZE];
		DWORD cbRead = 0;
		BYTE rgbHash[MD5LEN];
		DWORD cbHash = 0;
		CHAR rgbDigits[] = "0123456789abcdef";

		// Logic to check usage goes here.

		hFile = CreateFile(file.c_str(),
			GENERIC_READ,
			FILE_SHARE_READ,
			NULL,
			OPEN_EXISTING,
			FILE_FLAG_SEQUENTIAL_SCAN,
			NULL);

		if (INVALID_HANDLE_VALUE == hFile)
		{
			dwStatus = GetLastError();
			return dwStatus;
		}

		// Get handle to the crypto provider
		if (!CryptAcquireContext(&hProv,
			NULL,
			NULL,
			PROV_RSA_FULL,
			CRYPT_VERIFYCONTEXT))
		{
			dwStatus = GetLastError();
			printf("CryptAcquireContext failed: %d\n", dwStatus); 
			CloseHandle(hFile);
			return dwStatus;
		}

		if (!CryptCreateHash(hProv, CALG_MD5, 0, 0, &hHash))
		{
			dwStatus = GetLastError();
			printf("CryptAcquireContext failed: %d\n", dwStatus); 
			CloseHandle(hFile);
			CryptReleaseContext(hProv, 0);
			return dwStatus;
		}

		while (bResult = ReadFile(hFile, rgbFile, BUFSIZE, 
			&cbRead, NULL))
		{
			if (0 == cbRead)
			{
				break;
			}

			if (!CryptHashData(hHash, rgbFile, cbRead, 0))
			{
				dwStatus = GetLastError();
				printf("CryptHashData failed: %d\n", dwStatus); 
				CryptReleaseContext(hProv, 0);
				CryptDestroyHash(hHash);
				CloseHandle(hFile);
				return dwStatus;
			}
		}

		if (!bResult)
		{
			dwStatus = GetLastError();
			printf("ReadFile failed: %d\n", dwStatus); 
			CryptReleaseContext(hProv, 0);
			CryptDestroyHash(hHash);
			CloseHandle(hFile);
			return dwStatus;
		}

		cbHash = MD5LEN;
		if (CryptGetHashParam(hHash, HP_HASHVAL, rgbHash, &cbHash, 0))
		{
			for (DWORD i = 0; i < cbHash; i++)
			{
				md5 += rgbDigits[rgbHash[i] >> 4];
				md5 += rgbDigits[rgbHash[i] & 0xf];
			}
		}
		else
		{
			dwStatus = GetLastError();
			return dwStatus;
		}

		CryptDestroyHash(hHash);
		CryptReleaseContext(hProv, 0);
		CloseHandle(hFile);

		return 0; 
	}
	CATCH_ALL_EXCEPTIONS();
}   

void CreateDirectoryRecursive(std::wstring path)
{
	try
	{
		_ASSERT(!path.empty());
		typedef std::basic_string<_TCHAR> tstring;

		tstring::size_type pos = 0;

		while ((pos = path.find_first_of(_T("\\/"), pos + 1)) != tstring::npos)
		{
			::CreateDirectory(path.substr(0, pos + 1).c_str(), nullptr);
		}

		if ((pos = path.find_first_of(_T("\\/"), path.length() - 1)) == tstring::npos)
		{
			path.append(_T("\\"));
		}

		CreateDirectory(path.c_str(), nullptr);
	}
	CATCH_ALL_EXCEPTIONS();
}

void WriteToFile(const std::wstring& path, const char* buffer, DWORD dwBytesToWrite)
{
	try
	{
		HANDLE hFile;
		DWORD dwBytesWritten = 0;

		BOOL result = DeleteFile(path.c_str());

		if(!result)
		{
			MoveFile(path.c_str(), (path + L"old").c_str());
		}

		hFile = CreateFile(path.c_str(),// name of the write
			GENERIC_WRITE,				// open for writing
			0,							// do not share
			NULL,						// default security
			CREATE_NEW,					// create new file only
			FILE_ATTRIBUTE_NORMAL,		// normal file
			NULL);						// no attr. template

		WriteFile( 
			hFile,           // open file handle
			buffer,      // start of data to write
			dwBytesToWrite,  // number of bytes to write
			&dwBytesWritten, // number of bytes that were written
			NULL);            // no overlapped structure

		CloseHandle(hFile);
	}
	CATCH_ALL_EXCEPTIONS();
}

