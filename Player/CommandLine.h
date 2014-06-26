#ifndef MP_COMMAND_LINE_H
#define MP_COMMAND_LINE_H

namespace mp {

class CommandLine
{
public:
	CommandLine();
	bool IsSilent() const;
	bool IsInstall() const;

private:
	bool		m_silent;
	bool		m_isInstall;
};

}

#endif