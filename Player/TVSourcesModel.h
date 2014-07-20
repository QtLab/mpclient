#ifndef MP_TV_SOURCE_MODEL_H
#define MP_TV_SOURCE_MODEL_H

#include "Prerequirements.h"
#include "BaseListModel.h"

namespace mp {
namespace model {

struct TVSource : public QObject
{
	Q_OBJECT

public:
	TVSource();
	virtual ~TVSource();

	int Id() const;
	void SetId(int id);

	const QString& Name() const;
	void SetName(const QString& name);

	const QString& Url() const;
	void SetUrl(const QString& url);

	const QString& Logo() const;
	void SetLogo(const QString& logo);

	int PartnerId() const;
	void SetPartnerId(int id);

	void AddGenreId(int id);
	TVGenreIdsSet GenreIds() const;

	int CategoryId() const;
	void SetCategoryId(int id);

	void SetSelector(const QString& selector);
	const QString& Selector() const;
public:
	int						m_id;
	QString					m_name;
	QString					m_url;
	QString					m_logo;
	int						m_partnerId;
	TVGenreIdsSet			m_genreIds;
	int						m_categoryId;
	QString					m_selector;

	Q_DISABLE_COPY(TVSource)
};

class TVSourcesModel : public BaseListModel<TVSource>
{
	Q_OBJECT

public:
	enum TVSourceRoles
	{
		Id = Qt::UserRole + 1,
		Name,
		Url,
		Logo,
		PartnerId,
		FirstGenreId,
		CategoryId,
		GenreIds
	};

	TVSourcesModel(QObject* parent = NULL);
	virtual ~TVSourcesModel();

	void Load(const QString& filePath, const GenreIdsToCategoryIdMap& genresModel);

	TVSourcePtr Find(int channelId) const;

	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
	QHash<int, QByteArray>	roleNames() const;

private:
	void ParseChannelsJson(const QByteArray& json, const GenreIdsToCategoryIdMap& genresToCategoriesBinding);

public:
	friend QAbstractListModel;
	Q_DISABLE_COPY(TVSourcesModel)
};

} //namespace model
} //namespace mp

Q_DECLARE_METATYPE(mp::model::TVSource);
Q_DECLARE_METATYPE(mp::model::TVSourcesModel);

#endif