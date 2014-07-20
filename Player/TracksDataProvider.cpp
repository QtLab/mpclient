#include "TracksDataProvider.h"
#include "TrackModel.h"
#include "UrlBuilder.h"

#include <QNetworkReply>

namespace mp {

TracksDataProvider::TracksDataProvider(QObject* parent)
	:QObject(parent)
	,m_inProcess(false)
{
}

model::TrackModelPtr TracksDataProvider::Model() const
{
	return m_model;
}

void TracksDataProvider::SetModel(model::TrackModelPtr model)
{
	m_model = model;
}

void TracksDataProvider::Search(const QString& searchFilter)
{
	if(!m_inProcess)
	{
		m_searchFilter = searchFilter;
		m_searchFilter.replace("_", " ")
					.replace("//", " ")
					.replace("\\", " ");

		QUrl url  = UrlBuilder::CreateMp3Xml(m_searchFilter);
		m_networkAccess.Get(url, this, SLOT(Mp3XmlHttpRequestFinished()));

		m_inProcess = true;
	}
}

void TracksDataProvider::Mp3XmlHttpRequestFinished()
{
	bool success = false;

	QNetworkReply * reply = qobject_cast<QNetworkReply*>(sender());

	if(reply->error() != QNetworkReply::NoError)
	{
			qDebug() << "Network error, url: "
				<< reply->request().url().toString() 
				<<  ", error: " << reply->errorString()  << ", size" << reply->size();
	}
	else
	{
		QByteArray body = reply->readAll();
		if(m_model->LoadFrom(body, model::TrackModel::ST_Mp3xml))
		{
			if(m_model->rowCount() > 0)
			{
				success = true;
				m_inProcess = false;
				emit SearchFinished(true);
			}
		}
	}

	if(!success)
	{
		QUrl url = UrlBuilder::CreateInixMedia(m_searchFilter);
		m_networkAccess.Get(url, this, SLOT(InixmediaHttpRequestFinished()));
	}
}

void TracksDataProvider::InixmediaHttpRequestFinished()
{
	bool success = false;

	QNetworkReply * reply = qobject_cast<QNetworkReply*>(sender());

	if(reply->error() != QNetworkReply::NoError)
	{
			qDebug() << "Network error, url: "
				<< reply->request().url().toString() 
				<<  ", error: " << reply->errorString()  << ", size" << reply->size();
	}
	else
	{
		QByteArray body = reply->readAll();
		if(m_model->LoadFrom(body, model::TrackModel::ST_Inixmedia))
		{
			if(m_model->rowCount() > 0)
			{
				success = true;
			}
		}
	}

	m_inProcess = false;

	emit SearchFinished(success);
}

}