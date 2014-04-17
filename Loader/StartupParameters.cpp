#include "StartupParameters.h"

namespace ldr {

char** CommandLineToArgvA(char* cmdLine, int* _argc);

StartupParameters::StartupParameters()
	:m_isSilent(true)
	,m_isInstall(false)
{
	char* cmdLine = GetCommandLineA();
	int argc = 0;
	char** argv = CommandLineToArgvA(cmdLine, &argc);

	int i;
	char *key, *value;

	for( i = 1; i <= argc; i++ )
	{
		if( *argv[i] == '/' || *argv[i] == '-')
		{
			key = argv[i] + 1;
			value = strchr(key, ':');
			if( value != NULL ) 
			{
				*value++ = 0;
			}
		}
		else
		{
			key = argv[i];
			value = NULL;
		}

		if(_stricmp(key, "S") == 0)
		{
			m_isSilent = true;
		}
		else
		{
			if(_stricmp(key, "I") == 0)
			{
				m_isInstall = true;
			}
			else
			{
				if(_stricmp(key, "source") == 0)
				{
					m_source = value;
				}
			}
		}
	}
}

const String& StartupParameters::Source() const
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

void StartupParameters::ProcessOption(const String& key, const String& value)
{
	if(key == "S" || key == "s")
	{
		m_isSilent = true;
	}
	else
	{
		if(key == "I" || key == "i")
		{
			m_isInstall = true;
		}
		else
		{
			if(IsEquals(key, "source"))
			{
				m_source = value;
			}
		}
	}

}

char** CommandLineToArgvA(char* CmdLine, int* _argc)
{
    PCHAR* argv;
    PCHAR  _argv;
    ULONG   len;
    ULONG   argc;
    CHAR   a;
    ULONG   i, j;

    BOOLEAN  in_QM;
    BOOLEAN  in_TEXT;
    BOOLEAN  in_SPACE;

    len = strlen(CmdLine);
    i = ((len+2)/2)*sizeof(PVOID) + sizeof(PVOID);

    argv = (PCHAR*)GlobalAlloc(GMEM_FIXED,
        i + (len+2)*sizeof(CHAR));

    _argv = (PCHAR)(((PUCHAR)argv)+i);

    argc = 0;
    argv[argc] = _argv;
    in_QM = FALSE;
    in_TEXT = FALSE;
    in_SPACE = TRUE;
    i = 0;
    j = 0;

    while( a = CmdLine[i] ) {
        if(in_QM) {
            if(a == '\"') {
                in_QM = FALSE;
            } else {
                _argv[j] = a;
                j++;
            }
        } else {
            switch(a) {
            case '\"':
                in_QM = TRUE;
                in_TEXT = TRUE;
                if(in_SPACE) {
                    argv[argc] = _argv+j;
                    argc++;
                }
                in_SPACE = FALSE;
                break;
            case ' ':
            case '\t':
            case '\n':
            case '\r':
                if(in_TEXT) {
                    _argv[j] = '\0';
                    j++;
                }
                in_TEXT = FALSE;
                in_SPACE = TRUE;
                break;
            default:
                in_TEXT = TRUE;
                if(in_SPACE) {
                    argv[argc] = _argv+j;
                    argc++;
                }
                _argv[j] = a;
                j++;
                in_SPACE = FALSE;
                break;
            }
        }
        i++;
    }
    _argv[j] = '\0';
    argv[argc] = NULL;

    (*_argc) = argc;
    return argv;
}


}