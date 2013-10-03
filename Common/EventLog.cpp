#include "Common.h"

void InstallEventLogSource(const std::string& a_name)
{
	const std::string key_path("SYSTEM\\CurrentControlSet\\Services\\"
		"EventLog\\Application\\" + a_name);

	HKEY key;

	DWORD last_error = RegCreateKeyExA(HKEY_LOCAL_MACHINE,
		key_path.c_str(),
		0,
		0,
		REG_OPTION_NON_VOLATILE,
		KEY_SET_VALUE,
		0,
		&key,
		0);

	if (ERROR_SUCCESS == last_error)
	{
		char currentPath[MAX_PATH];
		GetCurrentDirectoryA(MAX_PATH, currentPath);
		sprintf(currentPath, "%s\\ZaxarUpdater.exe");

		DWORD last_error;
		const DWORD types_supported = EVENTLOG_ERROR_TYPE   |
			EVENTLOG_WARNING_TYPE |
			EVENTLOG_INFORMATION_TYPE;

		last_error = RegSetValueExA(key,
			"EventMessageFile",
			0,
			REG_SZ,
			(BYTE*)currentPath,
			sizeof(currentPath));

		if (ERROR_SUCCESS == last_error)
		{
			last_error = RegSetValueExA(key,
				"TypesSupported",
				0,
				REG_DWORD,
				(LPBYTE) &types_supported,
				sizeof(types_supported));
		}

		if (ERROR_SUCCESS != last_error)
		{
			std::cerr << "Failed to install source values: "
				<< last_error << "\n";
		}

		RegCloseKey(key);
	}
	else
	{
		std::cerr << "Failed to install source: " << last_error << "\n";
	}
}

void LogEventLogMessage(const std::string& a_msg,
	const WORD         a_type,
	const std::string& a_name)
{
	DWORD event_id = 0x0;

	/*
	switch (a_type)
	{
	case EVENTLOG_ERROR_TYPE:
		event_id = MSG_ERROR_1;
		break;
	case EVENTLOG_WARNING_TYPE:
		event_id = MSG_WARNING_1;
		break;
	case EVENTLOG_INFORMATION_TYPE:
		event_id = MSG_INFO_1;
		break;
	default:
		std::cerr << "Unrecognised type: " << a_type << "\n";
		event_id = MSG_INFO_1;
		break;
	}
	*/

	HANDLE h_event_log = RegisterEventSourceA(0, a_name.c_str());

	if (0 == h_event_log)
	{
		std::cerr << "Failed open source '" << a_name << "': " <<
			GetLastError() << "\n";
	}
	else
	{
		LPCTSTR message = StringToWChar(a_msg);

		if (FALSE == ReportEvent(h_event_log,
			a_type,
			0,
			event_id,
			0,
			1,
			0,
			&message,
			0))
		{
			std::cerr << "Failed to write message: " <<
				GetLastError() << "\n";
		}

		DeregisterEventSource(h_event_log);
	}
}

void UninstallEventLogSource(const std::string& a_name)
{
	const std::string key_path("SYSTEM\\CurrentControlSet\\Services\\"
		"EventLog\\Application\\" + a_name);

	DWORD last_error = RegDeleteKeyA(HKEY_LOCAL_MACHINE,
		key_path.c_str());

	if (ERROR_SUCCESS != last_error)
	{
		std::cerr << "Failed to uninstall source: " << last_error << "\n";
	}
}