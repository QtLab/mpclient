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

	bool WatchPlayer() const;

private:
	void ProcessOption(const String& key, const String& value);

private:
	String			m_source;
	bool			m_isSilent;
	bool			m_watchPlayer;
};

}