#include "Common.h"

bool IsRussian()
{
	LANGID lngId = GetSystemDefaultUILanguage();
	return lngId == 1049;
}