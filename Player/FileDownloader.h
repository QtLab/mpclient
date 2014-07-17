#ifndef MP_FILE_DOWNLOADER_H
#define MP_FILE_DOWNLOADER_H


#include <QUrl>
#include <QFile>
#include <QMutex>
#include <QVariant>
#include <QRunnable>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>

namespace mp {

// Downloads information on the link and stores it in a file specification
class FileDownloader: public QObject
{
	Q_OBJECT

public:
	FileDownloader(const QUrl& url, const QString& filePath, bool autoDelete = true);

	bool ContinueDownload() const;
	void SetContinueDownload(bool contiueDownload);

	void SetNetworkAccessManager(QNetworkAccessManager* manager);

	void SetTag(const QVariant& data) { m_tag = data; }
	const QVariant& Tag() { return m_tag; }

	bool HasError() { return m_errorString.isEmpty(); }
	const QString& ErrorString() { return m_errorString; }

	const QUrl& Url() const { return m_url; }

	void Do();
	void Abort();

private:
	void Cleanup();

private slots:
	void DownloadFinished();
	void DownloadProgress(qint64,qint64);
	void DownloadReadyRead();
	void ErrorSlot ( QNetworkReply::NetworkError code );

signals:
	void Finished(bool success, QVariant tag);
	void ProgressChanged(qint64 bytesReceived, qint64 bytesTotal);

private:
	const bool					m_autoDelete;
	const QString				m_filePath;
	const QUrl					m_url;

	bool						m_continueDownload;
	bool						m_aborted;
	QNetworkAccessManager*		m_manager;
	QFile *						m_file;
	QNetworkReply *				m_reply;
	QVariant					m_tag;
	QString						m_errorString;

	Q_DISABLE_COPY(FileDownloader)
};

}

#endif