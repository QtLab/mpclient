#ifndef MP_GENRE_SOURCE_H
#define MP_GENRE_SOURCE_H

#include <QMetaType>
#include <QSharedPointer>
#include <QAbstractListModel>
#include <QReadWriteLock>

namespace mp {

class GenreItem : public QObject
{
	Q_OBJECT
	Q_PROPERTY(QString	id READ Id WRITE SetId)
	Q_PROPERTY(QString name READ Name WRITE SetName)

public:
	GenreItem();
	~GenreItem();

	QString Id() const;
	void SetId(const QString& id);

	QString Name() const;
	void SetName(const QString& name);

private:
	QString					m_id;
	QString					m_name;
	Q_DISABLE_COPY(GenreItem)
};

typedef QSharedPointer<GenreItem> GenreItemPtr;
typedef QList<GenreItemPtr> GenreItemList;

class GenreModel : public QAbstractListModel
{
	Q_OBJECT

public:
	enum GenreItemRoles
	{
		Name = Qt::UserRole + 1,
		Id,
		Logo,
		Url
	};

	GenreModel();
	~GenreModel();

	// Don't use in prod. It isn't thread-safe
	GenreItemList Items() const;
	GenreItemPtr FindById(const QString& id);

	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
	int rowCount(const QModelIndex &parent = QModelIndex()) const;

private:
	void Add(GenreItemPtr contact, bool notifiChanged = false);
	void Load(const QString& filePath);
	//[
	//{"id": "1", "name": "PSYCHEDELIK"},
	//{"Id": "1", "name": "PSYCHEDELIK2"}
	//]
	void Parse(const QByteArray& json);
	
	void Cleanup();

private:
	friend class TabPagesController;
	friend class ChannelSourceModel;
	friend class RadioCompositeModel;
	friend class RadioPageController;

	GenreItemList				m_genres;
	mutable QReadWriteLock		m_lock;

	Q_DISABLE_COPY(GenreModel)
};

typedef QSharedPointer<GenreModel> GenreModelPtr;
//typedef GenreModel * GenreModelPtr;
//Q_DECLARE_METATYPE(GenreModelPtr);
//Q_DECLARE_METATYPE(GenreItemPtr);

}

#endif