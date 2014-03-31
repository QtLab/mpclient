#ifndef MP_BASE_LIST_MODELE_H
#define MP_BASE_LIST_MODELE_H

#include <QMetaType>
#include <QMetaObject>
#include <QMetaProperty>
#include <QSharedPointer>
#include <QAbstractListModel>
#include <QReadWriteLock>
#include <QJsonDocument>
#include <QList>
#include <QDebug>
#include <QFile>

namespace mp {

template<typename T>
class BaseListModel : public QAbstractListModel
{
public:
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

	virtual  void Load(const QString& filePath)
	{
		QFile file(filePath);
		if(file.open(QIODevice::ReadOnly))
		{
			QByteArray arr = file.readAll();
	#ifdef _DEBUG
			QString json = QString::fromUtf8(arr.data(), arr.size());
	#endif
			ParseJson(arr);
		}
		else
		{
			const QMetaObject *metaObject = &T::staticMetaObject;
			qDebug() << metaObject->className() << " can't open file: " << filePath;
		}
	}

	virtual void ParseJson(const QByteArray& json)
	{
		QJsonParseError parseResult;
		QJsonDocument d = QJsonDocument::fromJson(json, &parseResult);

		if(parseResult.error == QJsonParseError::NoError)
		{
			QList<QVariant> list = d.toVariant().toList();

			const QMetaObject *metaObject = &T::staticMetaObject;

			int count = metaObject->propertyCount();

			foreach(QVariant record, list) 
			{
				QSharedPointer<T> item(new T());

				QMap<QString, QVariant> map = record.toMap();

				for (int i = 0; i < count; ++i)
				{
					QMetaProperty property = metaObject->property(i);
					const char *name = property.name();
					QVariant value = map[name];

					if(!value.isNull())
						item->setProperty(name, value);
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
		m_items.clear();
		emit dataChanged(createIndex(0, 0),createIndex(0, 0));
	}

protected:
	QList<QSharedPointer<T>>				m_items;
};

}

#endif