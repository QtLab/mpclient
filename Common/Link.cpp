#include "Common.h"

HRESULT CreateLink(LPCWSTR lpszPathObj, LPCWSTR lpszPathLink, LPCWSTR lpszDesc, LPCWSTR args) 
{
	try
	{
		HRESULT hres; 
		IShellLink* psl; 

		CoInitialize(NULL);

		// Get a pointer to the IShellLink interface. It is assumed that CoInitialize
		// has already been called.
		hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (LPVOID*)&psl); 
		if (SUCCEEDED(hres)) 
		{ 
			IPersistFile* ppf; 

			// Set the path to the shortcut target and add the description. 
			psl->SetPath(lpszPathObj); 
			psl->SetDescription(lpszDesc); 

			TCHAR currentDir[MAX_PATH];
			GetCurrentDirectory(MAX_PATH, currentDir);

			psl->SetWorkingDirectory(currentDir);

			if(args != NULL)
				psl->SetArguments(args);

			//psl->SetIconLocation(lpszPathObj, IDI_ZAXARLOADER);

			// Query IShellLink for the IPersistFile interface, used for saving the 
			// shortcut in persistent storage. 
			hres = psl->QueryInterface(IID_IPersistFile, (LPVOID*)&ppf); 

			if (SUCCEEDED(hres)) 
			{ 
				WCHAR wsz[MAX_PATH]; 

				// Ensure that the string is Unicode. 
				//MultiByteToWideChar(CP_ACP, 0, lpszPathLink, -1, wsz, MAX_PATH); 
				// Add code here to check return value from MultiByteWideChar 
				// for success.

				// Save the link by calling IPersistFile::Save. 
				hres = ppf->Save(lpszPathLink, TRUE);
				ppf->Release();
			} 
			psl->Release(); 
		} 
		return hres; 
	}
	catch(char* lpstrErr) 
	{
		Log((std::string(__FUNCDNAME__) + " unhanded exception: " + lpstrErr).c_str());
	}
	catch(std::exception e)
	{
		Log((std::string(__FUNCDNAME__) + " unhanded exception: " + e.what()).c_str());
	}
	catch(...) 
	{
		Log((std::string(__FUNCDNAME__) + " unknown unhanded exception").c_str());
	}
}

HRESULT ResolveLinkPath(LPCWSTR lpszLinkFile, LPWSTR lpszPath, int iPathBufferSize) 
{ 
	try
	{
		HRESULT hres; 
		IShellLink* psl; 
		WCHAR szGotPath[MAX_PATH]; 
		WCHAR szDescription[MAX_PATH]; 
		WIN32_FIND_DATA wfd; 

		*lpszPath = 0; // Assume failure 

		CoInitialize(NULL);

		// Get a pointer to the IShellLink interface. It is assumed that CoInitialize
		// has already been called. 
		hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (LPVOID*)&psl); 
		if (SUCCEEDED(hres)) 
		{ 
			IPersistFile* ppf; 

			// Get a pointer to the IPersistFile interface. 
			hres = psl->QueryInterface(IID_IPersistFile, (void**)&ppf); 

			if (SUCCEEDED(hres)) 
			{ 
				// Add code here to check return value from MultiByteWideChar 
				// for success.

				// Load the shortcut. 
				hres = ppf->Load(lpszLinkFile, STGM_READ); 

				if (SUCCEEDED(hres)) 
				{ 
					// Resolve the link. 
					hres = psl->Resolve(NULL, 0); 

					if (SUCCEEDED(hres)) 
					{ 
						// Get the path to the link target. 
						hres = psl->GetPath(szGotPath, MAX_PATH, (WIN32_FIND_DATA*)&wfd, SLGP_SHORTPATH); 

						if (SUCCEEDED(hres)) 
						{ 
							// Get the description of the target. 
							hres = psl->GetDescription(szDescription, MAX_PATH); 

							if (SUCCEEDED(hres)) 
							{
								hres = StringCbCopy(lpszPath, iPathBufferSize, szGotPath);
								if (SUCCEEDED(hres))
								{
									// Handle success
								}
								else
								{
									// Handle the error
								}
							}
						}
					} 
				} 

				// Release the pointer to the IPersistFile interface. 
				ppf->Release(); 
			} 

			// Release the pointer to the IShellLink interface. 
			psl->Release(); 
		} 
		return hres; 
	}
	catch(char* lpstrErr) 
	{
		Log((std::string(__FUNCDNAME__) + " unhanded exception: " + lpstrErr).c_str());
	}
	catch(std::exception e)
	{
		Log((std::string(__FUNCDNAME__) + " unhanded exception: " + e.what()).c_str());
	}
	catch(...) 
	{
		Log((std::string(__FUNCDNAME__) + " unknown unhanded exception").c_str());
	}
}

void CreateLinkToLoaderInStartupFolder()
{
	try
	{
		TCHAR startupPath[MAX_PATH];
		SHGetFolderPath(0, CSIDL_STARTUP, 0, 0, startupPath);
		wsprintf(startupPath, L"%s\\MediaPlayer.lnk", startupPath);

		TCHAR currentDir[MAX_PATH];
		GetCurrentDirectory(MAX_PATH, currentDir);
		wsprintf(currentDir, L"%s\\Loader.exe", currentDir);
		CreateLink(currentDir, startupPath, L"MediaPlayer", L"/verysilent");
	}
	catch(char* lpstrErr) 
	{
		Log((std::string(__FUNCDNAME__) + " unhanded exception: " + lpstrErr).c_str());
	}
	catch(std::exception e)
	{
		Log((std::string(__FUNCDNAME__) + " unhanded exception: " + e.what()).c_str());
	}
	catch(...) 
	{
		Log((std::string(__FUNCDNAME__) + " unknown unhanded exception").c_str());
	}
}

void CreateLinkToLoaderInStartMenu()
{
	try
	{
		TCHAR menuPath[MAX_PATH];
		SHGetFolderPath(0, CSIDL_STARTMENU, 0, 0, menuPath);

		if(!DirectoryExists(menuPath))
		{
			CreateDirectoryRecursive(menuPath);
		}

		TCHAR currentDir[MAX_PATH], loaderPath[MAX_PATH], gbPath[MAX_PATH];

		GetCurrentDirectory(MAX_PATH, currentDir);
		wsprintf(loaderPath, L"%s\\Loader.exe", currentDir);
		wsprintf(menuPath, L"%s\\Programs\\Media Player\\Update.lnk", menuPath);
		CreateLink(loaderPath, menuPath, L"Media Player Update", NULL);

		GetCurrentDirectory(MAX_PATH, currentDir);
		wsprintf(loaderPath, L"%s\\Player.exe", currentDir);
		SHGetFolderPath(0, CSIDL_STARTMENU, 0, 0, menuPath);
		wsprintf(gbPath, L"%s\\Programs\\Media Player\\Media Player.lnk", menuPath);
		CreateLink(loaderPath, gbPath, L"Media Player", NULL);
	}
	catch(char* lpstrErr) 
	{
		Log((std::string(__FUNCDNAME__) + " unhanded exception: " + lpstrErr).c_str());
	}
	catch(std::exception e)
	{
		Log((std::string(__FUNCDNAME__) + " unhanded exception: " + e.what()).c_str());
	}
	catch(...) 
	{
		Log((std::string(__FUNCDNAME__) + " unknown unhanded exception").c_str());
	}
}

void CreateLinkToLoader()
{
	try
	{
		TCHAR desctopPath[MAX_PATH];
		SHGetSpecialFolderPath(0, desctopPath, CSIDL_DESKTOP, FALSE);
		wsprintf(desctopPath, L"%s\\Media Player.lnk", desctopPath);

		TCHAR currentDir[MAX_PATH];
		GetCurrentDirectory(MAX_PATH, currentDir);
		wsprintf(currentDir, L"%s\\Loader.exe", currentDir);
		CreateLink(currentDir, desctopPath, L"Media Player", L"/verysilent");
	}
	catch(char* lpstrErr) 
	{
		Log((std::string(__FUNCDNAME__) + " unhanded exception: " + lpstrErr).c_str());
	}
	catch(std::exception e)
	{
		Log((std::string(__FUNCDNAME__) + " unhanded exception: " + e.what()).c_str());
	}
	catch(...) 
	{
		Log((std::string(__FUNCDNAME__) + " unknown unhanded exception").c_str());
	}
}
