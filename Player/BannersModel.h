#ifndef MP_BANNERS_MODEL_H
#define MP_BANNERS_MODEL_H

#include "BaseListModel.h"
#include "Prerequirements.h"

namespace mp {
namespace model {

class BannerInfo : public QObject
{
	Q_OBJECT
	Q_PROPERTY(int	id READ Id WRITE SetId)
	Q_PROPERTY(QString	logo READ Logo WRITE SetLogo)
	Q_PROPERTY(int	tvSourceId READ TVSourceId WRITE SetTVSourceId)
	Q_PROPERTY(QString externalUrl READ ExternalUrl WRITE SetExternalUrl)

public:
	BannerInfo();
	~BannerInfo();

	int Id() const;
	void SetId(int id);

	QString Logo() const;
	void SetLogo(const QString& logo);

	int TVSourceId() const;
	void SetTVSourceId(int id);

	QString ExternalUrl() const;
	void SetExternalUrl(const QString& name);
	bool HasExternalUrl() const;

private:
	int						m_id;
	QString					m_logo;
	int						m_tvSourceId;
	QString					m_externalUrl;
	Q_DISABLE_COPY(BannerInfo)
};


class BannersModel : public BaseListModel<BannerInfo>
{
	Q_OBJECT

public:
	enum CategoryRoles
	{
		Id = Qt::UserRole + 1,
		ExtrnalUrl,
		TSourceId
	};

	BannersModel(QObject* parent);
	virtual ~BannersModel();

	BannerInfoPtr FindById(int id) const;

	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	QHash<int, QByteArray>	roleNames() const;
};

} //namespace model
} //namespace mp

#endif