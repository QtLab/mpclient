#include "Common.h"
#include "StackWalker.h"

class CommonStackWalker : public StackWalker
{
public:
	CommonStackWalker() : StackWalker() {}
protected:
	virtual void OnOutput(LPCSTR szText)
	{
		Log(szText);
	}
};


LONG WINAPI SEHProcessor(struct _EXCEPTION_POINTERS* ExceptionInfo) 
{
	try
	{
		Log("*******************************CommonStackWalker*******************************");
		CommonStackWalker sw;
		sw.ShowCallstack(GetCurrentThread(), ExceptionInfo->ContextRecord);
		Log("*************************************************************************");
		return EXCEPTION_EXECUTE_HANDLER;
	}
	CATCH_ALL_EXCEPTIONS();
}

LONG WINAPI SEHProcessor(unsigned int code, struct _EXCEPTION_POINTERS *ExceptionInfo)
{
	try
	{
		Log("*******************************CommonStackWalker with code*******************************");
		CommonStackWalker sw;
		sw.ShowCallstack(GetCurrentThread(), ExceptionInfo->ContextRecord);
		Log("*************************************************************************");
		return EXCEPTION_EXECUTE_HANDLER;
	}
	CATCH_ALL_EXCEPTIONS();
}

void InstallUnhandleSEHProcessor()
{
	SetUnhandledExceptionFilter(SEHProcessor);
}

