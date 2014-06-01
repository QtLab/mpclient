#pragma once

#include <string>

namespace ldr {

class Hash
{
public:
	static bool ComputeFileMD5(const std::string& filePath, std::string& md5);
	static bool ComputeMD5(const std::string& data, std::string& md5);
};

}