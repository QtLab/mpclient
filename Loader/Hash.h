#pragma once

#include "Defs.h"

namespace ldr {

class Hash
{
public:
	static bool ComputeFileMD5(const String& filePath, String& md5);
	static bool ComputeMD5(const String& data, String& md5);
};

}