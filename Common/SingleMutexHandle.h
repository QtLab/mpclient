#include <Windows.h>
#include <Tchar.h>

namespace cmn {

class SingleMutexHandle
{
public:
	SingleMutexHandle(TCHAR * mutexName)
		:m_alreadyExists(false)
	{
		m_handle = ::CreateMutex( NULL, TRUE, mutexName );

		if( ERROR_ALREADY_EXISTS == GetLastError())
		{
			m_alreadyExists = true;
		}
	}

	~SingleMutexHandle()
	{
		::ReleaseMutex( m_handle ); // Explicitly release mutex
		::CloseHandle( m_handle ); // close handle before terminating
	}

	bool AlreadyExists() const
	{
		return m_alreadyExists;
	}

private:
	bool		m_alreadyExists;	
	HANDLE		m_handle;
};

}
