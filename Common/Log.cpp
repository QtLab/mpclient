#include "Common.h"

void ClearLog()
{
	DeleteFileA("logfile.txt");
}

void LogErrStructed(const char* msg)
{
	Log((std::string(msg) + " unhanded strutted exception\r\n").c_str());
}

void Log(const char* msg)
{
	FILE *log = fopen("logfile.txt", "at");
	if (!log) log = fopen("logfile.txt", "wt");

	fputs  (msg, log);
	fclose (log);
}
