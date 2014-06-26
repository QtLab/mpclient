#ifndef MP_FILE_UTILS
#define MP_FILE_UTILS

#include <QObject>

namespace mp {

class FileUtils
{
public:
	static bool Delete(const QString& filePath);
	static void Delete(const QStringList& filters);
	static QString ComputeFileMD5(const QString& filePath);
	static bool LoadFileToByteArray(const QString& filePath, QByteArray& fileBody);
	static bool SaveByteArrayToFile(const QString& filePath, const QByteArray& fileBody);

private:
	FileUtils();
};

}

#endif
