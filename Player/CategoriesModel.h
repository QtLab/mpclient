#ifndef MP_GENRE_SOURCE_H
#define MP_GENRE_SOURCE_H

#include "BaseListModel.h"
#include "Prerequirements.h"

namespace mp {

class Category : public QObject
{
	Q_OBJECT
	Q_PROPERTY(int	id READ Id WRITE SetId)
	Q_PROPERTY(QString name READ Name WRITE SetName)

public:
	Category();
	~Category();

	int Id() const;
	void SetId(int id);

	QString Name() const;
	void SetName(const QString& name);

private:
	int						m_id;
	QString					m_name;
	Q_DISABLE_COPY(Category)
};


class CategoriesModel : public BaseListModel<Category>
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

	CategoriesModel();
	virtual ~CategoriesModel();

	// Don't use in prod. It isn't thread-safe
	CategoryList Items() const;
	CategoryPtr FindById(int id);

	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	QHash<int, QByteArray>	roleNames() const;
};


//Q_DECLARE_METATYPE(mp::GenreItem);
//Q_DECLARE_METATYPE(mp::GenreModel);


}

#endif