#ifndef MP_GENRE_SOURCE_H
#define MP_GENRE_SOURCE_H

#include "BaseListModel.h"

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

class GenreModel : public BaseListModel<GenreItem>
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
	virtual ~GenreModel();

	// Don't use in prod. It isn't thread-safe
	GenreItemList Items() const;
	GenreItemPtr FindById(const QString& id);

	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	QHash<int, QByteArray>	roleNames() const;

private:
	friend class TabPagesController;
	friend class ChannelSourceModel;
	friend class RadioCompositeModel;
	friend class RadioPageController;
};

typedef QSharedPointer<GenreModel> GenreModelPtr;
//Q_DECLARE_METATYPE(GenreModel);
//typedef GenreModel * GenreModelPtr;
//Q_DECLARE_METATYPE(GenreModelPtr);
//Q_DECLARE_METATYPE(GenreItemPtr);

}

#endif