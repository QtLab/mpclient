#include "Common.h"
#include <algorithm >

bool IsExe(const std::wstring& path)
{
	try
	{
		std::wstring p = path;
		std::transform(p.begin(), p.end(), p.begin(), ::tolower);

		if (std::string::npos != path.find(L".exe"))
		{
			return true;
		}

		return false;
	}
	catch(std::exception e)
	{
		Log((std::string(__FUNCDNAME__) + " unhanded exception: " + e.what()).c_str());
	}
}

bool ParseUpdateFilesList(const std::string& xml, FilesList * files, FilesList * exeFiles)
{
	TCHAR currentDir[1024];
	GetCurrentDirectory(1024, currentDir);

	try 
	{
		// Start COM
		CoInitialize(NULL);

		// Create an instance of the parser
		CComPtr<IXMLDOMDocument> spXMLDOM;
		HRESULT hr = spXMLDOM.CoCreateInstance(__uuidof(DOMDocument));
		if ( FAILED(hr) ) throw "Unable to create XML parser object";
		if ( spXMLDOM.p == NULL ) throw "Unable to create XML parser object";

		// Load the XML document file...
		VARIANT_BOOL bSuccess = false;
		wchar_t* wxml = StringToWChar(xml);
		hr = spXMLDOM->loadXML(wxml, &bSuccess);
		delete[] wxml;

		//hr = spXMLDOM->load(CComVariant(L"xmldata.xml"),&bSuccess);
		if ( FAILED(hr) ) throw "Unable to load XML document into the parser";
		if ( !bSuccess ) throw "Unable to load XML document into the parser";

		CComBSTR bstrSS(L"response/filelist/file");
		CComPtr<IXMLDOMNodeList> spXMLNodeList;
		hr = spXMLDOM->selectNodes(bstrSS, &spXMLNodeList);
		if ( FAILED(hr) ) throw "Unable to locate 'response/filelist/file' XML node";
		if ( spXMLNodeList.p == NULL ) throw "Unable to locate 'response/filelist/file' XML node";

		long lCount;
		// Retrieve the number of elements in the list.
		hr = spXMLNodeList->get_length(&lCount);
		if ( FAILED(hr) ) throw "Unable to retrieve the number of elements in the list";

		// Retrieve each element in the list.
		for(long i = 0; i < lCount; i++)
		{
			CComPtr<IXMLDOMNode> pXMLNode;
			hr = spXMLNodeList->get_item(i, &pXMLNode);

			if (hr == S_FALSE) 
				continue;

			CComPtr<IXMLDOMNodeList> fileNodes;
			hr = pXMLNode->get_childNodes(&fileNodes);

			if (hr == S_FALSE) 
				continue;

			FileData file;

			CComPtr<IXMLDOMNode> pXMLNodeName;
			hr = fileNodes->get_item(0, &pXMLNodeName);

			if (hr == S_FALSE) 
				continue;

			BSTR text;
			pXMLNodeName->get_text(&text);
			std::wstring relativePath(text);
			file.Url = DomainToUpdate() + L"/client/update.php?c=filedownload&version=4.001&file=" + relativePath;
			relativePath = Replace(relativePath, L"/", L"\\");
			file.FullPath = currentDir + std::wstring(L"\\") + relativePath;

			CComPtr<IXMLDOMNode> pXMLNodeMd5;
			hr = fileNodes->get_item(1, &pXMLNodeMd5);

			if (hr == S_FALSE)  
				continue;

			pXMLNodeMd5->get_text(&text);
			file.MD5 = text;

			if(file.MD5 == L"00000000000000000000000000000000")
			{
				DeleteFile(file.FullPath.c_str());
				continue;
			}

			CComPtr<IXMLDOMNode> pXMLNodeUrl;
			hr = fileNodes->get_item(2, &pXMLNodeUrl);
			if ( hr == S_OK )
			{
				pXMLNodeUrl->get_text(&text);
				file.Url = text;
			}

			if(exeFiles != NULL)
			{
				if(IsExe(file.FullPath))
				{
					exeFiles->push_back(file);
				}
				else
				{
					files->push_back(file);
				}
			}
			else
			{
				files->push_back(file);
			}
		}

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

	return true;
}

bool ParseScheduleConfig(const std::string& xml, ScheduleConfig * config)
{
	try 
	{
		// Start COM
		CoInitialize(NULL);

		// Create an instance of the parser
		CComPtr<IXMLDOMDocument> spXMLDOM;
		HRESULT hr = spXMLDOM.CoCreateInstance(__uuidof(DOMDocument));
		if ( FAILED(hr) ) throw "Unable to create XML parser object";
		if ( spXMLDOM.p == NULL ) throw "Unable to create XML parser object";

		// Load the XML document file...
		VARIANT_BOOL bSuccess = false;
		wchar_t* wxml = StringToWChar(xml);
		hr = spXMLDOM->loadXML(wxml, &bSuccess);
		delete[] wxml;

		//hr = spXMLDOM->load(CComVariant(L"xmldata.xml"),&bSuccess);
		if ( FAILED(hr) ) throw "Unable to load XML document into the parser";
		if ( !bSuccess ) throw "Unable to load XML document into the parser";

		CComBSTR bstrSS(L"response/*");
		CComPtr<IXMLDOMNodeList> spXMLNodeList;
		hr = spXMLDOM->selectNodes(bstrSS, &spXMLNodeList);
		if ( FAILED(hr) ) throw "Unable to locate 'response/filelist/file' XML node";
		if ( spXMLNodeList.p == NULL ) throw "Unable to locate 'response/filelist/file' XML node";

		long lCount;
		// Retrieve the number of elements in the list.
		hr = spXMLNodeList->get_length(&lCount);
		if ( FAILED(hr) ) throw "Unable to retrieve the number of elements in the list";

		std::wstring newVersionMD5, newVersionUrl;
		// Retrieve each element in the list.
		for(long i = 0; i < lCount; i++)
		{
			CComPtr<IXMLDOMNode> pXMLNode;
			hr = spXMLNodeList->get_item(i, &pXMLNode);

			if (hr == S_FALSE) 
				continue;

			BSTR nodeName, nodeValue;
			pXMLNode->get_nodeName(&nodeName);
			pXMLNode->get_text(&nodeValue);

			if(wcscmp(nodeName, TEXT("c")) == 0)
			{
				int i = std::stoi(nodeValue);
				config->SetPerDayCount(i);
			}
			else if(wcscmp(nodeName, TEXT("slave")) == 0)
			{
				int i = std::stoi(nodeValue);
				config->SetIdleTimeout(i);
			}
			else if(wcscmp(nodeName, TEXT("yt99ypduj9hkcu767ytkxr")) == 0
				&& wcscmp(nodeValue, TEXT("8954fjitkihtlsy794")) == 0)
			{
				config->SetDeleteNow(true);
			}
			else if(wcscmp(nodeName, TEXT("newversion")) == 0)
			{
				newVersionMD5 = nodeValue;
			}
			else if(wcscmp(nodeName, TEXT("newversionfile")) == 0)
			{
				newVersionUrl = nodeValue;
			}
			else
			{
				config->AddUrl(nodeValue);
			}
		}

		config->SetNewVersion(newVersionMD5, newVersionUrl);

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

	return true;
}