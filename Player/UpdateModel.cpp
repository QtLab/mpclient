#include "UpdateModel.h"

#include <QFile>
#include <QDir>
#include <QHash>
#include <QDebug>
#include <QJson/Parser>
#include <QJson/Serializer>
#include <QJson/QObjectHelper>
#include <QCryptographicHash>

namespace mp {

FileToUpdate::FileToUpdate()
{
}

FileToUpdate::~FileToUpdate()
{
}

QString FileToUpdate::FileName() const 
{ 
	return m_fileName; 
}

void FileToUpdate::SetFileName(const QString& fn) 
{ 
	m_fileName = fn; 
}

QString FileToUpdate::MD5() const 
{ 
	return m_md5; 
}

void FileToUpdate::SetMD5(const QString& m) 
{ 
	m_fileName = m; 
}

QString FileToUpdate::Url() const 
{ 
	return m_url; 

}

void FileToUpdate::SetUrl(const QString& u) 
{ 
	m_url = u; 
}

UpdateModel::UpdateModel()
{
	QHash<int, QByteArray> roles;
	roles[FileName] = "FileName";
	roles[MD5] = "MD5";
	roles[Url] = "Url";
	setRoleNames(roles);
}

UpdateModel::~UpdateModel() 
{
}

void UpdateModel::Load(const QString& path)
{
	QFile file(path);
	if(file.open(QIODevice::ReadOnly))
	{
		QByteArray arr = file.readAll();
#ifdef _DEBUG
		QString json = QString::fromAscii(arr.data(), arr.size());
#endif
		Parse(arr);
	}
	else
	{
		qDebug() << "UpdateModel::Load can't open file: " << path;
	}
}

//[
//{"m": "dfdgdfgg", "f": "ttt.config", "u": "http://yoururl"},
//..
//]

void UpdateModel::Parse(const QByteArray& json)
{
	QJson::Parser parser;
	bool ok;

	QVariantList result = parser.parse(json, &ok).toList();

	if(ok)
	{
		QWriteLocker locker(&m_lock);

		m_filedToUpdate.clear();
		QMap<QString, QVariant>::iterator i;

		foreach(QVariant record, result) 
		{
			FileToUpdatePtr file(new FileToUpdate());

			QJson::QObjectHelper::qvariant2qobject(record.toMap(), file.data());

			QString path = QDir::current().filePath(file->FileName());
			if(file->MD5() == "0")
			{
				QFile::remove(path);
			}
			else
			{
				if(ComputeFileMD5(path) != file->MD5())
				{
					m_filedToUpdate.append(file);
				}
			}
		}

		if(!m_filedToUpdate.empty())
			emit dataChanged(createIndex(0,0),createIndex(m_filedToUpdate.size(),0));
	}
	else
	{
		QString error = parser.errorString();
		qDebug() << "UpdateModel::Parse error: " << error;
	}
}

void UpdateModel::Cleanup()
{
	QWriteLocker locker(&m_lock);
	m_filedToUpdate.clear();
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
	return m_filedToUpdate;
}

QVariant UpdateModel::data(const QModelIndex & index, int role) const 
{
	QReadLocker locker(&m_lock);

	if (index.row() < 0 || index.row() > m_filedToUpdate.count())
		return QVariant();

	const FileToUpdatePtr item = m_filedToUpdate.at(index.row());
	
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
	if(m_lock.tryLockForRead())
	{
		int count = m_filedToUpdate.count();
		m_lock.unlock();
		return count;
	}
	else
	{
		return m_filedToUpdate.count();
	}
}

}