#ifndef MP_GENRE_SOURCE_H
#define MP_GENRE_SOURCE_H

#include "BaseListModel.h"
#include "Prerequirements.h"

namespace mp {
namespace model {

class TVCategory : public QObject
{
	Q_OBJECT

public:
	TVCategory();
	~TVCategory();

	int Id() const;
	void SetId(int id);

	QString Name() const;
	void SetName(const QString& name);

	bool TopVisible() const;
	void SetTopVisible(bool topVisible);

	TVSourceIdsSet TVTopSourceIds() const;
	void AddTVTopSourceId(int id);

	int NextBannerId() const;
	BannerInfoIds BannerIds() const;
	void AddBannerId(int id);
private:
	int						m_id;
	QString					m_name;
	bool					m_topVisible;
	TVSourceIdsSet				m_tvTopSourceIds;
	BannerInfoIds			m_bannerIds;
	Q_DISABLE_COPY(TVCategory)
};


class TVCategoriesModel : public BaseListModel<TVCategory>
{
	Q_OBJECT

public:
	enum CategoryRoles
	{
		Name = Qt::UserRole + 1,
		Id,
		TopVisible,
		TVTopSourceIds,
		BannerIds
	};

	TVCategoriesModel(QObject* parent);
	virtual ~TVCategoriesModel();

	void Load(const QString& filePath);
	void ParseCatygoriesJson(const QByteArray& json);

	Q_INVOKABLE int UpdateTopVisibleCategories(int maxWidth, const QString& fontFamily, int pointSize, int spacing);
	Q_INVOKABLE void InsertFirstTopVisibleCategory(int id, int maxWidth, const QString& fontFamily, int pointSize, int spacing);
	Q_INVOKABLE void InsertLastTopVisibleCategory(int id, int maxWidth, const QString& fontFamily, int pointSize, int spacing);

	Q_INVOKABLE int RowIndexById(int id) const;
	TVCategoryPtr FindById(int id) const;

	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	QHash<int, QByteArray>	roleNames() const;

	void Cleanup();

private:
	int			m_lastTopVIsibleIndex;
};

} //namespace model
} //namespace mp

#endif