#include "Defs.h"
#include "Path.h"
#include "zlib.h"
#include "unzip.h"

#include <sstream>
#include <fstream>
#include <iostream>

const int SizeBuffer = 32768;


bool Unzip(const std::string& pakage)
{
	unzFile archive = unzOpen(pakage.c_str());

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
				ldr::Path::Make(ldr::Path::CurrentPathCombine(dosFileName));
			}
			else
			{
				ldr::String filePath = ldr::Path::CurrentPathCombine(dosFileName);

				ldr::String oldFilePath = filePath + "old";

				if(ldr::Path::FileExists(filePath))
				{
					MoveFile(filePath.c_str(), oldFilePath.c_str());
					DWORD err = GetLastError();
					std::cout << err;
				}
				else
				{
					ldr::String dir = ldr::Path::DirectoryFromFilePath(filePath);

					if(!ldr::Path::DirectoryExists(dir))
					{
						ldr::Path::Make(dir);
					}
				}

				std::ofstream ostream(filePath, std::ios::out | std::ios::binary | std::ios::trunc);
				char szBuffer[SizeBuffer];
				while ((bytesReaded = unzReadCurrentFile(archive, buffer, SizeBuffer)) > 0)
				{
					ostream.write((const char*)szBuffer, bytesReaded);
					totalsize += bytesReaded;
				}

				DeleteFile(oldFilePath.c_str());
				unzCloseCurrentFile(archive);
			}
		} 
		while (unzGoToNextFile(archive) == UNZ_OK);
	} 

	unzClose(archive);

	return true;
}