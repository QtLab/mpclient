#include "Common.h"

//Указатели на функции
PMD5Init MD5Init = NULL;
PMD5Update MD5Update = NULL;
PMD5Final MD5Final = NULL;

//Динамическое подключение функций
bool InitMD5()
{
	try
	{
		HMODULE hLib = LoadLibrary(TEXT("Cryptdll.dll"));
		MD5Init = PMD5Init ( GetProcAddress(hLib, "MD5Init") );
		MD5Update = PMD5Update( GetProcAddress(hLib, "MD5Update") );
		MD5Final = PMD5Final ( GetProcAddress(hLib, "MD5Final"));
		return true;
	}
	catch(std::exception e)
	{
		Log((std::string(__FUNCDNAME__) + " unhanded exception: " + e.what()).c_str());
	}
}

TMD5 _ComputeMD5(PUCHAR Buffer, DWORD dwSize)
{
	try
	{
		const char HEX_CHAR_MAP[] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
		MD5_CTX context; //Контекст вычислений
		TMD5 result;
		MD5Init(&context);//Инициализация контекста

		//Обновляем контекст, добавляя к нему блоки данных в 64 байта
		for (; dwSize >= 64; Buffer += 64, dwSize -= 64)
			MD5Update(&context, Buffer, 64);
		//Обновляем контекст для последнего блока, размер которого меньше 64 байт
		MD5Update(&context, Buffer, dwSize);
		//Финализируем контекст. Теперь он содержит результат
		MD5Final(&context);

		//Получаем строковое представление MD5 хеша
		for (int i=0; i<16; i++)
		{
			result.hash[i*2] = HEX_CHAR_MAP[context.digest[i] >> 4];
			result.hash[i*2+1] = HEX_CHAR_MAP[context.digest[i] & 15];
		}

		result.hash[32] = NULL;
		memcpy(result.digest, context.digest, 16);
		return result;
	}
	catch(std::exception e)
	{
		Log((std::string(__FUNCDNAME__) + " unhanded exception: " + e.what()).c_str());
	}
}