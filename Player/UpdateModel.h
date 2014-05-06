#ifndef MP_UPDATE_MODEL_H
#define MP_UPDATE_MODEL_H

#include "BaseListModel.h"
#include "Prerequirements.h"

namespace mp {

class FileToUpdate : public QObject
{
	Q_OBJECT
	Q_PROPERTY(QString N READ FileName WRITE SetFileName)
	Q_PROPERTY(QString M READ MD5 WRITE SetMD5)
	Q_PROPERTY(QString U READ Url WRITE SetUrl)
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

	FileToUpdateList Items() const;
	void Remove(FileToUpdatePtr fileInfo);
	bool RequiredPlayerUpdate() const;

	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	QHash<int, QByteArray>	roleNames() const;

private:
	void ParseJson(const QByteArray& json);
	QString ComputeFileMD5(const QString& filePath);

private:
	bool	m_requiredPlayerUpdate;

	friend class UpdateController;
	Q_DISABLE_COPY(UpdateModel)
};

}

#endif