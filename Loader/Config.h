#pragma once

#include "Defs.h"

namespace ldr {

class Config
{
public:
	Config();

	String ConfigFilePath();
	bool ReLoad();
	bool Save();

	const std::string& UserId() const;
	void SetUserId(const String& id);

	const std::string& Source() const;
	void SetSource(const String& guid);

	bool InstallSuccessfull() const;
	void SetInstallSuccessfull(bool success);

	bool IsSilent() const;
	void SetSilentMode(bool silent);

private:
	String			m_userId;
	String			m_source;
	bool			m_hasInstallSuccessfull;
	bool			m_isSilent;
};

}