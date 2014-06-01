#pragma once

#include "Defs.h"
#include <string>

namespace ldr {

class Path
{
public:
	static void SetupCurrentDirectoryPath();
	static String CurrentDirectoryPath();
	static bool Make(const String& path);
	static bool FileExists(const String& path);
	static bool DirectoryExists(const String& path);
	static String DirectoryFromFilePath(const String& path);
	// Parameter path should be without "\\" at start
	static String CurrentPathCombine(const String& path);
};

}