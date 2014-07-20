#ifndef MP_GENRE_SOURCE_H
#define MP_GENRE_SOURCE_H

#include "BaseListModel.h"
#include "Prerequirements.h"

namespace mp {
namespace model {

class RadioCategory : public QObject
{
	Q_OBJECT
	Q_PROPERTY(int	id READ Id WRITE SetId)
	Q_PROPERTY(QString name READ Name WRITE SetName)

public:
	RadioCategory();
	~RadioCategory();

	int Id() const;
	void SetId(int id);

	QString Name() const;
	void SetName(const QString& name);

	bool TopVisible() const;
	void SetTopVisible(bool topVisible);

private:
	int						m_id;
	QString					m_name;
	bool					m_topVisible;
	Q_DISABLE_COPY(RadioCategory)
};


class RadioCategoriesModel : public BaseListModel<RadioCategory>
{
	Q_OBJECT

public:
	enum CategoryRoles
	{
		Name = Qt::UserRole + 1,
		Id,
		TopVisible
	};

	RadioCategoriesModel(QObject* parent = NULL);
	virtual ~RadioCategoriesModel();

	Q_INVOKABLE int UpdateTopVisibleCategories(int maxWidth, const QString& fontFamily, int pointSize, int spacing);
	Q_INVOKABLE void InsertFirstTopVisibleCategory(int id, int maxWidth, const QString& fontFamily, int pointSize, int spacing);
	Q_INVOKABLE void InsertLastTopVisibleCategory(int id, int maxWidth, const QString& fontFamily, int pointSize, int spacing);

	Q_INVOKABLE int RowIndexById(int id) const;
	RadioCategoryPtr FindById(int id) const;

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