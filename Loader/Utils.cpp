#include "Defs.h"
#include "Path.h"
#include "zlib.h"
#include "unzip.h"

#include <sstream>
#include <fstream>
#include <iostream>

namespace ldr {

const int SizeBuffer = 32768;

using namespace cmn;

bool Unzip(const String& pakagePath)
{
	std::cout << "Unzip pakage: " << pakagePath << std::endl;

	unzFile archive = unzOpen(pakagePath.c_str());

	if (unzGoToFirstFile(archive) == UNZ_OK) 
	{
		do 
		{
			if (unzOpenCurrentFile(archive) != UNZ_OK) 
				continue;

			unz_file_info unzfi;
			char dosFileName[MAX_PATH];

			unzGetCurrentFileInfo(archive, &unzfi, dosFileName, MAX_PATH, NULL, 0, NULL, 0);

			int bytesReaded = 0, totalsize = 0;

			unsigned char buffer[SizeBuffer];
			::memset(buffer, 0, SizeBuffer);

			char lastCh = dosFileName[strlen(dosFileName) - 1];
			if(lastCh == '/')
			{
				unzCloseCurrentFile(archive);
				continue;
			}

			String filePath = cmn::Path::CurrentPathCombine(dosFileName);

			std::cout << "Unzip file: " << dosFileName << " to: "<< filePath << std::endl;

			String oldFilePath = filePath + "old";

			if(Path::FileExists(filePath))
			{
				if(!MoveFile(filePath.c_str(), oldFilePath.c_str()))
				{
					std::cout << "Can't move file: " << filePath
								<< " to: " << oldFilePath
								<< " error: " << GetLastErrorString() << std::endl;
				}
			}
			else
			{
				String dir = Path::DirectoryFromFilePath(filePath);

				if(!Path::DirectoryExists(dir))
				{
					Path::Make(dir);
					std::cout << "Create directory: " << dir << " result: " << GetLastErrorString() << std::endl;
				}
			}

			HANDLE hFile = CreateFile(filePath.c_str(),// name of the write
				GENERIC_WRITE,				// open for writing
				0,							// do not share
				NULL,						// default security
				CREATE_NEW,					// create new file only
				FILE_ATTRIBUTE_NORMAL,		// normal file
				NULL);						// no attr. template

			if(hFile == INVALID_HANDLE_VALUE)
			{
				std::cout << "CreateFile: " << filePath << " error: " << GetLastErrorString() << std::endl;
				unzCloseCurrentFile(archive);
				CloseHandle(hFile);
				continue;
			}

			while ((bytesReaded = unzReadCurrentFile(archive, buffer, SizeBuffer)) > 0)
			{
				DWORD dwBytesWritten;

				WriteFile( 
					hFile,           // open file handle
					buffer,      // start of data to write
					bytesReaded,  // number of bytes to write
					&dwBytesWritten, // number of bytes that were written
					NULL);            // no overlapped structure
				totalsize += bytesReaded;

				if(GetLastError() != 0)
				{
					std::cout << "WriteFile: " << filePath << " error: " << GetLastErrorString() << std::endl;
					unzCloseCurrentFile(archive);
					CloseHandle(hFile);
					continue;
				}

			}

			DeleteFile(oldFilePath.c_str());
			unzCloseCurrentFile(archive);
			CloseHandle(hFile);
		} 
		while (unzGoToNextFile(archive) == UNZ_OK);
	} 

	unzClose(archive);

	return true;
}

// Create a string with last error message
String GetLastErrorString()
{
	DWORD error = GetLastError();
	if (error)
	{
		LPVOID lpMsgBuf;
		DWORD bufLen = FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | 
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			error,
			MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US),
			(LPTSTR) &lpMsgBuf,
			0, NULL );
		if (bufLen)
		{
			LPCSTR lpMsgStr = (LPCSTR)lpMsgBuf;
			std::string result(lpMsgStr, lpMsgStr+bufLen);
      
			LocalFree(lpMsgBuf);

			return result;
		}
	}

	return "Success";
}

bool IsEquals(const String& st0, const String& st1)
{
	if(st0.compare(st1) == 0)
	{
		return true;
	}
	else
	{

#ifdef _UNICODE
		if(_wcsicmp(st0.c_str(), st1.c_str()) == 0)
#else
		if(_stricmp(st0.c_str(), st1.c_str()) == 0)
#endif
		{
			return true;
		}
	}

	return false;
}

void WaitMinutes(int minutes)
{
	Sleep(minutes * 60 * 1000);
}

const String& BoolToString(bool b)
{
	static const String _TRUE("true");
	static const String _FALSE("false");

	return b ? _TRUE : _FALSE;
}

#define LoaderMutexName "{C0A20390-C055-496D-9EB0-50C726E6A2B7}"

HANDLE hSignleMutex = INVALID_HANDLE_VALUE;

bool IsLoaderAlredyExists()
{
    // Try to open the mutex.
    hSignleMutex = OpenMutexA(MUTEX_ALL_ACCESS, 0, LoaderMutexName);

    if (hSignleMutex == INVALID_HANDLE_VALUE)
	{
		hSignleMutex = CreateMutexA(0, 0, LoaderMutexName);
		return false;
	}
    else
	{
      // The mutex exists so this is the
      // the second instance so return.
      return true;
	}
}

void FreeSingleInstanceMutex()
{
	HANDLE hMutex = OpenMutexA(MUTEX_ALL_ACCESS, 0, LoaderMutexName);
	CloseHandle(hMutex);
}

}

