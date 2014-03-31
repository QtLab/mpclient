#include <BaseListModel.h>

namespace mp {

/*
template<typename T>
void BaseListModel<T>::Add(QSharedPointer<T> item, bool notifiChanged)
{
	beginInsertRows(QModelIndex(), m_items.length(), m_items.length());
	m_items.append(channel);
	endInsertRows();

	if(notifiChanged)
	{
		// notify view
		emit dataChanged(createIndex(0,0),createIndex(m_genres.size(),0));
	}
}

template<typename T>
void BaseListModel<T>::Load(const QString& filePath)
{
	QFile file(path);
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
		
		qDebug() << typeid(this).name() << " can't open file: " << path;
	}
}

template<typename T>
void BaseListModel<T>::ParseJson(const QByteArray& json)
{
	QJsonDocument d = QJsonDocument::fromJson(json);
	QList<QVariant> list = d.toVariant().toList();

	const QMetaObject *metaObject = &T::staticMetaObject;
	int count = metaObject->propertyCount();

	foreach(QVariant record, list) 
	{
		QSharedPointer<T> item(new T());

		QMap<QString, QVariant> map = record.toMap();

		for (int i=0; i<count; ++i)
		{
			QMetaProperty property = metaObject->property(i);
			const char *name = property.name();
			QVariant value = map[name];
			genre->setProperty(name, value);
		}

		
		Add(GenreItemPtr(genre));
	}
}

template<typename T>
void BaseListModel<T>::Cleanup()
{
	m_items.clear();
}
*/

}