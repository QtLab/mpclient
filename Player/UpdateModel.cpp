#include "UpdateModel.h"

#include <QFile>
#include <QDir>
#include <QHash>
#include <QDebug>
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
	QJsonDocument d = QJsonDocument::fromJson(json);
	QList<QVariant> list = d.toVariant().toList();

	const QMetaObject *metaObject = &FileToUpdate::staticMetaObject;
	int count = metaObject->propertyCount();

	foreach(QVariant record, list) 
	{
		QSharedPointer<FileToUpdate> file(new FileToUpdate());

		QMap<QString, QVariant> map = record.toMap();

		for (int i=0; i<count; ++i)
		{
			QMetaProperty property = metaObject->property(i);
			const char *name = property.name();
			QVariant value = map[name];
			file->setProperty(name, value);
		}
		
		QString path = QDir::current().filePath(file->FileName());
		if(file->MD5() == "0")
		{
			QFile::remove(path);
		}
		else
		{
			if(ComputeFileMD5(path) != file->MD5())
			{
				Add(file);
			}
		}
	}

	if(!m_items.empty())
		emit dataChanged(createIndex(0,0), createIndex(m_items.size(), 0));
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