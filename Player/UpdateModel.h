#ifndef MP_UPDATE_MODEL_H
#define MP_UPDATE_MODEL_H

#include "BaseListModel.h"
#include "Prerequirements.h"

namespace mp {
namespace model {

struct FileToUpdate// : public QObject
{
public:
	FileToUpdate();
	~FileToUpdate();

	const QString& FileName() const;
	void SetFileName(const QString& fn);

	const QString& MD5() const;
	void SetMD5(const QString& m);

	const QString& Url() const;
	void SetUrl(const QString& u);

	QString FullPath() const;
	bool Exists() const;
	bool IsPlayer() const;
	bool IsLoader() const;
	bool IsLauncer() const;

	int DownloadTries() const;
	void IncrementDownlaodTries();

private:
	QString		m_fileName;
	QString		m_md5;
	QString		m_url;
	int			m_downloadTries;
};

class UpdateModel : public QAbstractListModel
{
	Q_OBJECT

public:
	enum FileToUpdateRoles
	{
		FileName = Qt::UserRole + 1,
		MD5,
		Url
	};

	UpdateModel();
	~UpdateModel();

	FileToUpdatePtr PopBack();
	bool RequirePlayerUpdate() const;
	void Cleanup();
	void ParseJson(const QByteArray& json);

	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	QHash<int, QByteArray>	roleNames() const;

private:
	bool							m_requirePlayerUpdate;
	QList<FileToUpdatePtr>			m_items;

	Q_DISABLE_COPY(UpdateModel)
};

} //namespace model
} //namespace mp

Q_DECLARE_METATYPE(mp::model::FileToUpdate)


#endif