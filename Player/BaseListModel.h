#ifndef MP_BASE_LIST_MODELE_H
#define MP_BASE_LIST_MODELE_H

#include <QMetaType>
#include <QMetaObject>
#include <QMetaProperty>
#include <QSharedPointer>
#include <QAbstractListModel>
#include <QReadWriteLock>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QList>
#include <QDebug>
#include <QFile>
#include <QDateTime>
#include <QSet>

namespace mp {

template<typename T>
class BaseListModel : public QAbstractListModel
{
public:
	typedef QSet<QString> PropertiesSet;

	BaseListModel(){}
	virtual ~BaseListModel(){}

	virtual void Add(QSharedPointer<T> item, bool notifiChanged = false)
	{
		beginInsertRows(QModelIndex(), m_items.length(), m_items.length());
		m_items.append(item);
		endInsertRows();

		if(notifiChanged)
		{
			// notify view
			emit dataChanged(createIndex(0,0),createIndex(m_items.size(),0));
		}
	}

	virtual  void Load(const QString& filePath, const PropertiesSet& propertiesToLoad = PropertiesSet())
	{
		QFile file(filePath);
		if(file.open(QIODevice::ReadOnly))
		{
			QByteArray arr = file.readAll();
	#ifdef _DEBUG
			QString json = QString::fromUtf8(arr.data(), arr.size());
	#endif
			ParseJson(arr, propertiesToLoad);
		}
		else
		{
			const QMetaObject *metaObject = &T::staticMetaObject;
			qDebug() << metaObject->className() << " can't open file: " << filePath;
		}
	}

	virtual bool Save(const QString& filePath, const PropertiesSet& propertiesToSave = PropertiesSet())
	{
		const QMetaObject *metaObject = &T::staticMetaObject;
		int propertiesCount = metaObject->propertyCount();

		QJsonDocument document;
		QJsonArray jsonArray;

		ItemList::iterator iter;
		for (iter = m_items.begin(); iter != m_items.end(); ++iter)
		{
			QJsonObject jsonObj;
			ItemType item = (*iter);
			for (int i = 0; i < propertiesCount; ++i)
			{
				QMetaProperty property = metaObject->property(i);
				const char *propertyName = property.name();

				if(strcmp("objectName", propertyName) != 0)
				{
					if(propertiesToSave.empty() || propertiesToSave.contains(propertyName))
					{
						QVariant value = item->property(propertyName);
#ifdef _DEBUG
						QString str = value.toString();
#endif
						jsonObj.insert(propertyName, QJsonValue::fromVariant(value));
					}
				}
			}

			jsonArray.append(QJsonValue(jsonObj));
		}

		document.setArray(jsonArray);
		QByteArray json = document.toJson();

		QFile file(filePath);
		if(file.open(QIODevice::WriteOnly | QIODevice::Text))
		{
			qint64 written = file.write(json);
			int toWrite = json.size();

			return written == toWrite;
		}

		return false;
	}

	virtual void ParseJson(const QByteArray& json, const PropertiesSet& propertiesToLoad = PropertiesSet())
	{
		QJsonParseError parseResult;
		QJsonDocument d = QJsonDocument::fromJson(json, &parseResult);

		if(parseResult.error == QJsonParseError::NoError)
		{
			QJsonArray list = d.array();

			const QMetaObject *metaObject = &T::staticMetaObject;

			int count = metaObject->propertyCount();

			foreach(QJsonValue record, list) 
			{
				ItemType item(new T());

				QJsonObject object = record.toObject();

				for (int i = 0; i < count; ++i)
				{
					QMetaProperty property = metaObject->property(i);
					const char* propertyName = property.name();

					if(propertiesToLoad.empty() || propertiesToLoad.contains(propertyName))
					{
						QJsonValue value = object.value(propertyName);
					
						if(!value.isNull())
							item->setProperty(propertyName, value.toVariant());
					}
				}
		
				Add(item);
			}

			if(!m_items.empty())
				emit dataChanged(createIndex(0,0),createIndex(m_items.size(),0));
		}
		else
		{
			const QMetaObject *metaObject = &T::staticMetaObject;
			qDebug() << metaObject->className() << " json parse error: " << parseResult.errorString();
		}
	}
	
	void Cleanup()
	{
		beginResetModel();
		m_items.clear();
		endResetModel();
	}

protected:
	typedef QSharedPointer<T>	ItemType;
	typedef QList<ItemType>		ItemList;

	ItemList					m_items;
};

}

#endif