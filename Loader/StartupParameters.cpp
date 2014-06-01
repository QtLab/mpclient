#include "StartupParameters.h"

namespace ldr {

StartupParameters::StartupParameters()
	:m_isSilent(true)
	,m_isInstall(false)
{
}

const std::string& StartupParameters::Source() const
{
	return m_source;
}

bool StartupParameters::IsInstall() const
{
	return m_isInstall;
}


bool StartupParameters::IsSilent() const
{
	return m_isSilent;
}

}