#pragma once

#include "Defs.h"

namespace ldr {

class Process
{
public:
	Process(const String& exeName, const String& args, const String& parentDir = String());
	~Process();
	// Returns exit code
	DWORD StartAndWait();

	static void Terminate(const String& name);

private:
	String			m_exeName;
	String			m_args;
	String			m_parentDir;
};
		
}