#ifndef MP_TAB_MODEL_H
#define MP_TAB_MODEL_H

#include <QMetaType>
#include <QSharedPointer>
#include <QAbstractListModel>
#include <QReadWriteLock>
#include <QObject>

namespace mp {

class TabModelItem : public QObject
{
	Q_OBJECT
	Q_PROPERTY(QString id READ Id WRITE SetId)
	Q_PROPERTY(QString name READ Name WRITE SetName)
	Q_PROPERTY(QString args READ Arguments WRITE SetArguments)

public:

	TabModelItem();
	~TabModelItem();

	QString Id() const;
	void SetId(const QString& id);

	QString Name() const;
	void SetName(const QString& n);

	QString Arguments() const;
	void SetArguments(const QString& n);

private:
	QString				m_id;
	QString				m_name;
	QString				m_arguments;
	Q_DISABLE_COPY(TabModelItem)
};


typedef QSharedPointer<TabModelItem> TabModelItemPtr;
typedef QList<TabModelItemPtr> TabModelItemList;

class TabModel : public QAbstractListModel
{
	Q_OBJECT

public:
	enum TabModelItemRoles
	{
		Id = Qt::UserRole + 1,
		Name,
		Arguments
	};

	TabModel();
	~TabModel();

	// Don't use in prod. It isn't thread-safe
    // for (int row = 3; row < qrecap_model->rowCount(); row++)
    // {
    // QModelIndex idx = (qrecap_model->index(row, 0));
	// s = qrecap_model->data(idx).toString();
     //}
	TabModelItemList Items() const;

	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
	int rowCount(const QModelIndex &parent = QModelIndex()) const;

private:
	void Add(TabModelItemPtr contact, bool notifiChanged = false);
	void Load(const QString& filePath);
	//[
	//{"id": "radio", "name": "RADIO", args: "radio.json" },
	//{"id": "tv", "name": "RADIO", args: "tv.json" },
	//{"id": "webpage", "name": "WEB PAGE", "args": "www.youtube.ru" }
	//..
	//]
	void Parse(const QByteArray& json);

private:
	friend class TabPagesController;

	TabModelItemList				m_tabs;
	mutable QReadWriteLock			m_lock;

	Q_DISABLE_COPY(TabModel)
};

typedef TabModel * TabModelPtr;
//Q_DECLARE_METATYPE(UpdateModelPtr);
//Q_DECLARE_METATYPE(FileToUpdatePtr);

}

#endif