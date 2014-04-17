#pragma once

#include "Defs.h"

namespace ldr {

class StartupParameters
{
public:
	StartupParameters();

	const String& Source() const;

	bool IsInstall() const;

	bool IsSilent() const;

private:
	void ProcessOption(const String& key, const String& value);

private:
	String			m_source;
	bool			m_isInstall;
	bool			m_isSilent;
};

}