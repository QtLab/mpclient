#include "Hash.h"
#include <Windows.h>

namespace ldr {

#define BUFSIZE 1024
#define MD5LEN  16

bool Hash::ComputeFileMD5(const std::string& filePath, std::string& md5)
{
	md5.clear();

	DWORD dwStatus = 0;
	BOOL bResult = FALSE;
	HCRYPTPROV hProv = 0;
	HCRYPTHASH hHash = 0;
	HANDLE hFile = NULL;
	BYTE rgbFile[BUFSIZE];
	DWORD cbRead = 0;
	BYTE rgbHash[MD5LEN];
	DWORD cbHash = 0;
	CHAR rgbDigits[] = "0123456789ABCDEF";

	// Logic to check usage goes here.

	hFile = CreateFileA(filePath.c_str(),
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_FLAG_SEQUENTIAL_SCAN,
		NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		dwStatus = GetLastError();
		//TODO: logging
		return false;
	}

	// Get handle to the crypto provider
	if (!CryptAcquireContext(&hProv,
		NULL,
		NULL,
		PROV_RSA_FULL,
		CRYPT_VERIFYCONTEXT))
	{
		dwStatus = GetLastError();
		CloseHandle(hFile);
		//TODO: logging
		return false;
	}

	if (!CryptCreateHash(hProv, CALG_MD5, 0, 0, &hHash))
	{
		dwStatus = GetLastError();
		CloseHandle(hFile);
		CryptReleaseContext(hProv, 0);
		//TODO: logging
		return false;
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
			CryptReleaseContext(hProv, 0);
			CryptDestroyHash(hHash);
			CloseHandle(hFile);
			//TODO: logging
			return false;
		}
	}

	if (!bResult)
	{
		dwStatus = GetLastError();
		CryptReleaseContext(hProv, 0);
		CryptDestroyHash(hHash);
		CloseHandle(hFile);
		//TODO: logging
		return false;
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
		return false;
	}

	CryptDestroyHash(hHash);
	CryptReleaseContext(hProv, 0);
	CloseHandle(hFile);

	return true; 
}

bool Hash::ComputeMD5(const std::string& data, std::string& md5)
{
	DWORD dwStatus = 0;
	BOOL bResult = FALSE;
	HCRYPTPROV hProv = 0;
	HCRYPTHASH hHash = 0;
	DWORD cbRead = 0;
	BYTE rgbHash[MD5LEN];
	DWORD cbHash = 0;
	CHAR rgbDigits[] = "0123456789ABCDEF";

	// Get handle to the crypto provider
	if (!CryptAcquireContext(&hProv,
		NULL,
		NULL,
		PROV_RSA_FULL,
		CRYPT_VERIFYCONTEXT))
	{
		dwStatus = GetLastError();
		CryptReleaseContext(hProv, 0);
		return false;
	}

	if (!CryptCreateHash(hProv, CALG_MD5, 0, 0, &hHash))
	{
		dwStatus = GetLastError();
		CryptReleaseContext(hProv, 0);
		//TODO: logging
		return false;
	}
	
	if (!CryptHashData(hHash, (BYTE*)data.c_str(), data.size(), 0))
	{
		dwStatus = GetLastError();
		CryptReleaseContext(hProv, 0);
		//TODO: logging
		return false;
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
		return false;
	}

	CryptDestroyHash(hHash);
	CryptReleaseContext(hProv, 0);

	return true;
}

}