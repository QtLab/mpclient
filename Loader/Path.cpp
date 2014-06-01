#include "Path.h"
#include <Windows.h>

namespace ldr {

String ReplaceInStringWith(String& text, const String& s, const String& d)
{
	for(unsigned index=0; index=text.find(s, index), index!= String::npos;)
	{
		text.replace(index, s.length(), d);
		index += d.length();
	}
	return text;
}


void Path::SetupCurrentDirectoryPath()
{ 
	SetCurrentDirectory(CurrentDirectoryPath().c_str());
}

String Path::CurrentDirectoryPath()
{
	CHAR buffer[MAX_PATH];
	GetModuleFileName( NULL, buffer, MAX_PATH );
	String::size_type pos = String( buffer ).find_last_of(PATH_SEPARATOR);
	String dir = String(buffer).substr( 0, pos);

	return dir;
}

bool Path::Make(const String& p)
{
	String::size_type pos = 0;

	String path = p;

	while ((pos = path.find_first_of(_T("\\/"), pos + 1)) != String::npos)
	{
		::CreateDirectory(path.substr(0, pos + 1).c_str(), nullptr);
	}

	if ((pos = path.find_first_of("\\/", path.length() - 1)) == String::npos)
	{
		path.append("\\/");
	}

	bool result = (bool)CreateDirectory(path.c_str(), nullptr);
	return result;
}

bool Path::FileExists(const String& path)
{
	DWORD dwAttrib = GetFileAttributes(path.c_str());

	return (dwAttrib != INVALID_FILE_ATTRIBUTES && 
		!(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}

bool Path::DirectoryExists(const String& path)
{
	DWORD ftyp = GetFileAttributes(path.c_str());
	if (ftyp == INVALID_FILE_ATTRIBUTES)
		return false;  //something is wrong with your path!

	if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
		return true;   // this is a directory!

	return false;    // this is not a directory!
}

String Path::DirectoryFromFilePath(const std::string& path)
{
	unsigned found = path.find_last_of(PATH_SEPARATOR);
	String dir = path.substr(0,found);
	return dir;
}

String Path::CurrentPathCombine(const String& path)
{
	String resultPath = CurrentDirectoryPath() + PATH_SEPARATOR + path;
	ReplaceInStringWith(resultPath, "/", PATH_SEPARATOR);
	return resultPath;
}

}