#ifndef MP_FILE_UTILS
#define MP_FILE_UTILS

#include <QObject>

namespace mp {

class FileUtils
{
public:
	static void Delete(const QString& filter);
	static QString ComputeFileMD5(const QString& filePath);
	static bool LoadFileToByteArray(const QString& filePath, QByteArray& fileBody);
	static bool SaveByteArrayToFile(const QString& filePath, const QByteArray& fileBody);

private:
	FileUtils();
};

}

#endif
