#include "Common.h"

DWORD GetMinutesDifference( SYSTEMTIME &st1)
{
	try
	{
		FILETIME        ft1, ft2;
		LARGE_INTEGER   li1, li2, liDiff;
		DWORD dwDiff;

		SYSTEMTIME st2;
		GetSystemTime(&st2);

		SystemTimeToFileTime( &st1, &ft1 );
		SystemTimeToFileTime( &st2, &ft2 );

		li1.LowPart = ft1.dwLowDateTime;
		li1.HighPart = ft1.dwHighDateTime;
		li2.LowPart = ft2.dwLowDateTime;
		li2.HighPart = ft2.dwHighDateTime;

		if ( CompareFileTime(&ft1, &ft2) < 0 ) {
			liDiff.LowPart  = li2.LowPart  - li1.LowPart;
			liDiff.HighPart = li2.HighPart - li1.HighPart;
		}
		else {
			liDiff.LowPart  = li1.LowPart  - li2.LowPart;
			liDiff.HighPart = li1.HighPart - li2.HighPart;
		}

		dwDiff = ((__int64)liDiff.HighPart << 32) + liDiff.LowPart;
		dwDiff /= ((__int64)10*1000*1000*60); // Convert from n x 100ns intervals into minutes

		return dwDiff;
	}
	catch(std::exception e)
	{
		Log((std::string(__FUNCDNAME__) + " unhanded exception: " + e.what()).c_str());
	}
}