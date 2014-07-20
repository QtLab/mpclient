#ifndef MP_GENRES_MODEL_H
#define MP_GENRES_MODEL_H

#include "BaseListModel.h"
#include "Prerequirements.h"

namespace mp {
namespace model {

class TVGenre : public QObject
{
	Q_OBJECT
	Q_PROPERTY(int	id READ Id WRITE SetId)
	Q_PROPERTY(int	categoryid READ CategoryId WRITE SetCategoryId)
	Q_PROPERTY(QString name READ Name WRITE SetName)

public:
	TVGenre();
	~TVGenre();

	int Id() const;
	void SetId(int id);

	int CategoryId() const;
	void SetCategoryId(int id);

	QString Name() const;
	void SetName(const QString& name);

private:
	int						m_id;
	int						m_categoryId;
	QString					m_name;
	Q_DISABLE_COPY(TVGenre)
};


class TVGenresModel : public BaseListModel<TVGenre>
{
	Q_OBJECT

public:
	enum CategoryRoles
	{
		Name = Qt::UserRole + 1,
		Id,
		CategoryId
	};

	TVGenresModel(QObject* parent);
	virtual ~TVGenresModel();

	Q_INVOKABLE int RowIndexById(int id) const;
	TVGenrePtr FindById(int id) const;
	const GenreIdsToCategoryIdMap& GenreIdsToCategoryIdBindingMap() const;
	void Cleanup();

	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
	QHash<int, QByteArray>	roleNames() const;

private:
	void ParseJson(const QByteArray& json, const PropertiesSet& propertiesToLoad = PropertiesSet());

private:
	GenreIdsToCategoryIdMap		m_genreIdsToCategoryIdToMap;
};

} //namespace model
} //namespace mp

#endif