#include "CommandLine.h"

#include <QString>
#include <QStringList>
#include <QApplication>

namespace mp {

CommandLine::CommandLine()
	:m_silent(false)
{
	foreach(QString arg, qApp->arguments())
	{
		if(arg.compare("-s", Qt::CaseInsensitive) == 0 
			|| arg.compare("/s", Qt::CaseInsensitive) == 0)
		{
			m_silent = true;
		}
	}
}

bool CommandLine::IsSilent() const
{
	return m_silent;
}

}