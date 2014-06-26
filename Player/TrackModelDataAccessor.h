#ifndef MP_TRACK_MODEL_DATA_ACCESSOR_H
#define MP_TRACK_MODEL_DATA_ACCESSOR_H

#include "Prerequirements.h"
#include "DownlaodManager.h"

namespace mp {

class TrackModelDataAccessor : public QObject
{
	Q_OBJECT

public:
	TrackModelDataAccessor();

	model::TrackModelPtr Model() const;
	void SetModel(model::TrackModelPtr model);

	void Search(const QString& searchFilter);

private slots:
	void Mp3XmlHttpRequestFinished();
	void InixmediaHttpRequestFinished();

signals:
	void SearchFinished(bool ok);

private:
	bool					m_inProcess;
	QString					m_searchFilter;
	DownlaodManager			m_networkAccess;
	model::TrackModelPtr	m_model;
};

}

#endif