#ifndef MP_TRACKS_DATA_PROVIDER_H
#define MP_TRACKS_DATA_PROVIDER_H

#include "Prerequirements.h"
#include "DownlaodManager.h"

namespace mp {

class TracksDataProvider : public QObject
{
	Q_OBJECT

public:
	TracksDataProvider(QObject* parent = NULL);

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