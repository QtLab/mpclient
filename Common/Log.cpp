#include "Common.h"

void ClearLog()
{
	DeleteFileA("logfile.txt");
}

void LogErrStructed(const std::string& mess)
{
	Log(mess + " unhanded strutted exception\r\n");
}

void Log(const std::string& mess)
{
	FILE *log = fopen("logfile.txt", "at");
	if (!log) log = fopen("logfile.txt", "wt");

	fputs  (mess.c_str(), log);
	fclose (log);
}
