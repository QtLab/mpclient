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
	void SetUserId(const std::string& id);

	const std::string& Source() const;
	void SetSource(const std::string& guid);

	bool InstallSuccessfull() const;
	void SetInstallSuccessfull(bool success);

private:
	String			m_userId;
	String			m_source;
	bool			m_hasInstallSuccessfull;
};

}