#include "FileUtils.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QDebug>
#include <QCryptographicHash>

namespace mp {

void FileUtils::Delete(const QString& filter)
{
	QFileInfoList files = QDir::current().entryInfoList(QStringList() << filter);
	QFileInfoList::Iterator iter(files.begin());

	while(iter != files.end())
	{
		QFile::remove(iter->filePath());
		iter++;
	}
}

QString FileUtils::ComputeFileMD5(const QString& filePath)
{
	QFile file(filePath);
	if (file.open(QIODevice::ReadOnly))
	{
		QByteArray hashData = QCryptographicHash::hash(file.readAll(), QCryptographicHash::Md5);
		QString hash(hashData.toHex());
		return hash;
	}

	return QString::null;
}

bool FileUtils::LoadFileToByteArray(const QString& filePath, QByteArray& fileBody)
{
	QFile file(filePath);
	if(file.open(QIODevice::ReadOnly))
	{
		fileBody = file.readAll();
		return true;
	}
	else
	{
		qDebug() << "FileUtils::LoadByteArray can't open file: " << filePath;
	}

	return false;
}

bool FileUtils::SaveByteArrayToFile(const QString& filePath, const QByteArray& fileBody)
{
	QFile file(filePath);
	if(file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		qint64 written = file.write(fileBody);
		int toWrite = fileBody.size();

		return written == toWrite;
	}

	return false;
}

}