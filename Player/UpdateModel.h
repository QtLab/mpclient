#ifndef MP_UPDATE_MODEL_H
#define MP_UPDATE_MODEL_H

#include <QMetaType>
#include <QSharedPointer>
#include <QAbstractListModel>
#include <QReadWriteLock>
#include <QObject>

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


typedef QSharedPointer<FileToUpdate> FileToUpdatePtr;
typedef QList<FileToUpdatePtr> FileToUpdateList;

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

	// Don't use in prod. It isn't thread-safe
    // for (int row = 3; row < qrecap_model->rowCount(); row++)
    // {
    // QModelIndex idx = (qrecap_model->index(row, 0));
	// s = qrecap_model->data(idx).toString();
     //}
	FileToUpdateList Items() const;

	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
	int rowCount(const QModelIndex &parent = QModelIndex()) const;

private:
	void Add(FileToUpdatePtr contact, bool notifiChanged = false);
	void Load(const QString& filePath);
	//[
	//{"m": "dfdgdfgg", "f": "ttt.config", "u": "http://yoururl"},
	//..
	//]
	void Parse(const QByteArray& json);
	void Cleanup();
	QString ComputeFileMD5(const QString& filePath);

private:
	friend class UpdateControler;

	FileToUpdateList				m_filedToUpdate;
	mutable QReadWriteLock			m_lock;

	Q_DISABLE_COPY(UpdateModel)
};

typedef UpdateModel * UpdateModelPtr;
//Q_DECLARE_METATYPE(UpdateModelPtr);
//Q_DECLARE_METATYPE(FileToUpdatePtr);

}

#endif