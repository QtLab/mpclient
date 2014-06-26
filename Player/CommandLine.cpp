#include "CommandLine.h"

#include <QString>
#include <QStringList>
#include <QApplication>

namespace mp {

CommandLine::CommandLine()
	:m_silent(false)
	,m_isInstall(false)
{
	foreach(QString arg, qApp->arguments())
	{
		if(arg.compare("-s", Qt::CaseInsensitive) == 0 
			|| arg.compare("/s", Qt::CaseInsensitive) == 0)
		{
			m_silent = true;
		}
		else
		{
			if(arg.compare("-i", Qt::CaseInsensitive) == 0 
				|| arg.compare("/i", Qt::CaseInsensitive) == 0)
			{
				m_isInstall = true;
			}
		}
	}
}

bool CommandLine::IsSilent() const
{
	return m_silent;
}

bool CommandLine::IsInstall() const
{
	return m_isInstall;
}

}