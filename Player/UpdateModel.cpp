#include "UpdateModel.h"
#include "Common.h"

#include <QFile>
#include <QDir>
#include <QHash>
#include <QDebug>
#include <QCryptographicHash>

namespace mp {

static const QString PlayerExeName(PLAYER_APP_EXE);
static const QString FileNameKey("N");
static const QString FileUrlKey("U");
static const QString FileMd5Key("M");
static const QString Zero("0");

FileToUpdate::FileToUpdate()
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

QString FileToUpdate::FullPath() const
{
	QString path = QDir::current().filePath(m_fileName);
	return path;
}

void FileToUpdate::SetUrl(const QString& u) 
{ 
	m_url = u; 
}

UpdateModel::UpdateModel()
	:m_requiredPlayerUpdate(false)
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

		const QMetaObject *metaObject = &FileToUpdate::staticMetaObject;
		int count = metaObject->propertyCount();

		foreach(QVariant record, list) 
		{
			QSharedPointer<FileToUpdate> file(new FileToUpdate());

			QMap<QString, QVariant> map = record.toMap();

			file->SetFileName(map[FileNameKey].toString());
			file->SetUrl(map[FileUrlKey].toString());
			file->SetMD5(map[FileMd5Key].toString());
		
			
			if(file->MD5() == Zero)
			{
				QFile::remove(file->FullPath());
			}
			else
			{
				QString md5 = ComputeFileMD5(file->FullPath());
				if(md5.compare(file->MD5(), Qt::CaseSensitivity::CaseInsensitive) != 0)
				{
					if(file->FileName().compare(PlayerExeName, Qt::CaseSensitivity::CaseInsensitive) == 0)
					{
						m_requiredPlayerUpdate = true;
					}

					Add(file);
				}
			}
		}

		if(!m_items.empty())
			emit dataChanged(createIndex(0,0), createIndex(m_items.size(), 0));
	}
}

QString UpdateModel::ComputeFileMD5(const QString& filePath)
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

FileToUpdateList UpdateModel::Items() const
{
	return m_items;
}

void UpdateModel::Remove(FileToUpdatePtr fileInfo)
{
	int index = m_items.indexOf(fileInfo);
	if(index > -1)
	{
		m_items.removeAt(index);
	}

	emit dataChanged(createIndex(0,0), createIndex(m_items.size(), 0));
}

bool UpdateModel::RequiredPlayerUpdate() const
{
	return m_requiredPlayerUpdate;
}

QVariant UpdateModel::data(const QModelIndex & index, int role) const 
{
	if (index.row() < 0 || index.row() > m_items.count())
		return QVariant();

	const FileToUpdatePtr item = m_items.at(index.row());
	
	QVariant result;

	switch (role) {
	case FileName:
		result = QVariant(item->FileName());
		break;
	case MD5:
		result = QVariant(item->MD5());
		break;
	case Url:
		result = QVariant(item->Url());
		break;
	}

	return result;
}

int UpdateModel::rowCount(const QModelIndex &parent) const
{
	//files to update
	return m_items.count();
}

QHash<int, QByteArray>	UpdateModel::roleNames() const
{
	QHash<int, QByteArray> roles;
	roles[FileName] = "FileName";
	roles[MD5] = "MD5";
	roles[Url] = "Url";

	return roles;
}

}