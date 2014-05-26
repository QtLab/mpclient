#pragma once

#include "Common.h"
#include <windows.h>

namespace cmn {

class Process
{
public:
	Process(const String& exeName, const String& args, const String& parentDir = String());
	~Process();

	// Is process already exists
	bool Exists() const;
	// Start new process
	bool Start();
	void Wait() const;
	// Returns exit code of the current process
	DWORD ExitCode() const;

	static void Terminate(const String& processName);
	static HANDLE OpenProcess(const String& processName);

private:
	String			m_exeName;
	String			m_args;
	String			m_parentDir;
	HANDLE			m_hProcess;
};
		
}