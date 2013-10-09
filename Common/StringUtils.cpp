#include "Common.h"

bool IsRussian()
{
	LANGID lngId = GetSystemDefaultUILanguage();
	if(lngId == 1049)
		return true;

	return false;;
}

bool IsEnglish()
{
	LANGID lngId = GetSystemDefaultUILanguage();
	if(lngId == 0x0c09)
		return true;

	return false;;
}

std::string WstringToString( const std::wstring& in)
{
	//std::locale loc = std::locale();
	std::string out;
	std::wstring::const_iterator i( in.begin() ), ie( in.end());

	for( ; i!=ie; ++i )
		out += std::use_facet<std::ctype<char> > ( std::locale() ).widen( *i );

	return out;
}

std::wstring StringToWstring( const std::string& in)
{
	//std::locale loc = std::locale();
	std::wstring out;
	std::string::const_iterator i( in.begin() ), ie( in.end() );

	for( ; i!=ie; ++i )
		out += std::use_facet<std::ctype<wchar_t> > ( std::locale() ).widen( *i );

	return out;
}

wchar_t* StringToWChar(const std::string& text)
{
	size_t size = text.size() + 1;
	wchar_t* wa = new wchar_t[size];
	mbstowcs(wa,text.c_str(),size);
	return wa;
}


std::wstring Replace(std::wstring& text, const std::wstring& s, const std::wstring& d)
{
	for(unsigned index=0; index=text.find(s, index), index!=std::string::npos;)
	{
		text.replace(index, s.length(), d);
		index+=d.length();
	}
	return text;
}
