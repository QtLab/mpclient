#include "UpdateModel.h"
#include "Common.h"
#include "FileUtils.h"

#include <QFile>
#include <QDir>
#include <QHash>
#include <QDebug>
#include <QApplication>

namespace mp {
namespace model {

static const QString PlayerExeName(PLAYER_APP_EXE);
static const QString LoaderExeName(LAODER_APP_EXE);
static const QString LaucherExeName(LAUCNHER_APP_EXE);
static const QString FileNameKey("N");
static const QString FileUrlKey("U");
static const QString FileMd5Key("M");
static const QString Zero("0");

FileToUpdate::FileToUpdate()
	:m_downloadTries(0)
{
}

FileToUpdate::~FileToUpdate()
{
}

const QString& FileToUpdate::FileName() const 
{ 
	return m_fileName; 
}

void FileToUpdate::SetFileName(const QString& fn) 
{ 
	m_fileName = fn; 
}

const QString& FileToUpdate::MD5() const 
{ 
	return m_md5; 
}

void FileToUpdate::SetMD5(const QString& m) 
{ 
	m_md5 = m; 
}

const QString& FileToUpdate::Url() const 
{ 
	return m_url; 

}

void FileToUpdate::SetUrl(const QString& u) 
{ 
	m_url = u; 
}

QString FileToUpdate::FullPath() const
{
	QString path = QDir::current().filePath(m_fileName);
	return path;
}

bool FileToUpdate::Exists() const
{
	bool exists = false;

	QString md5 = FileUtils::ComputeFileMD5(FullPath());
	if(md5.compare(m_md5, Qt::CaseSensitivity::CaseInsensitive) == 0)
	{
		exists = true;
	}

	return exists;
}

bool FileToUpdate::IsPlayer() const
{
	if(m_fileName.compare(PlayerExeName, Qt::CaseSensitivity::CaseInsensitive) == 0)
	{
		return true;
	}

	return false;
}

bool FileToUpdate::IsLoader() const
{
	if(m_fileName.compare(LoaderExeName, Qt::CaseSensitivity::CaseInsensitive) == 0)
	{
		return true;
	}

	return false;
}

bool FileToUpdate::IsLauncer() const
{
	if(m_fileName.compare(LaucherExeName, Qt::CaseSensitivity::CaseInsensitive) == 0)
	{
		return true;
	}

	return false;
}

int FileToUpdate::DownloadTries() const
{
	return m_downloadTries;
}

void FileToUpdate::IncrementDownlaodTries()
{
	m_downloadTries++;
}

UpdateModel::UpdateModel()
	:m_requirePlayerUpdate(false)
{
}

UpdateModel::~UpdateModel() 
{
}


//[
//{"m": "dfdgdfgg", "f": "ttt.config", "u": "http://yoururl"},
//..
//]

void UpdateModel::ParseJson(const QByteArray& json)
{
	QJsonParseError parseResult;
	QJsonDocument d = QJsonDocument::fromJson(json, &parseResult);

	if(parseResult.error == QJsonParseError::NoError)
	{
		QList<QVariant> list = d.toVariant().toMap()["F"].toList();

		foreach(QVariant record, list) 
		{
			FileToUpdate file;

			QMap<QString, QVariant> map = record.toMap();

			file.SetFileName(map[FileNameKey].toString());
			file.SetUrl(map[FileUrlKey].toString());
			file.SetMD5(map[FileMd5Key].toString());
		
			if(file.MD5() == Zero)
			{
				QFile::remove(file.FullPath());
			}
			else
			{
				if(!file.Exists())
				{
					if(file.IsPlayer())
					{
						m_requirePlayerUpdate = true;
					}

					m_items.append(file);
				}

				QApplication::processEvents();
			}
		}
	}
}

bool UpdateModel::PopBack(FileToUpdate& file)
{
	if(m_items.count() > 0)
	{
		file= m_items.last();
		m_items.pop_back();

		return true;
	}

	return false;
}

bool UpdateModel::RequirePlayerUpdate() const
{
	return m_requirePlayerUpdate;
}

void UpdateModel::Cleanup()
{
	m_requirePlayerUpdate = false;
	m_items.clear();
}

int UpdateModel::Count() const
{
	return  m_items.count();
}

} //namespace model
} //namespace mp
