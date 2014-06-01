#pragma once

#include <string>

namespace ldr {

class StartupParameters
{
public:
	StartupParameters();

	const std::string& Source() const;

	bool IsInstall() const;

	bool IsSilent() const;

private:
	std::string		m_source;
	bool			m_isInstall;
	bool			m_isSilent;
};

}