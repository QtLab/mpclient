#ifndef MP_UPDATE_MODEL_H
#define MP_UPDATE_MODEL_H

#include "BaseListModel.h"
#include "Prerequirements.h"

namespace mp {

class FileToUpdate : public QObject
{
	Q_OBJECT
	Q_PROPERTY(QString f READ FileName WRITE SetFileName)
	Q_PROPERTY(QString m READ MD5 WRITE SetMD5)
	Q_PROPERTY(QString u READ Url WRITE SetUrl)
public:

	FileToUpdate();
	~FileToUpdate();

	QString FileName() const;
	void SetFileName(const QString& fn);

	QString MD5() const;
	void SetMD5(const QString& m);

	QString Url() const;
	void SetUrl(const QString& u);

private:
	QString		m_fileName;
	QString		m_md5;
	QString		m_url;
	Q_DISABLE_COPY(FileToUpdate)
};

class UpdateModel : public BaseListModel<FileToUpdate>
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

	// Don't use in prod. It isn't thread-safe
    // for (int row = 3; row < qrecap_model->rowCount(); row++)
    // {
    // QModelIndex idx = (qrecap_model->index(row, 0));
	// s = qrecap_model->data(idx).toString();
     //}
	FileToUpdateList Items() const;
	void Remove(FileToUpdatePtr fileInfo);

	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	QHash<int, QByteArray>	roleNames() const;

private:
	void ParseJson(const QByteArray& json);
	QString ComputeFileMD5(const QString& filePath);

private:
	friend class UpdateController;
	Q_DISABLE_COPY(UpdateModel)
};

}

#endif