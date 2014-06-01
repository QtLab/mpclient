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
	// Если useDownloadProgress == false -  сигнал DownloadProgressChanged вырабатыватсья не будет
	// и файл будет сохранен на диск по сигналу полного завершения скачивания
	FileDownloader(const QUrl& url, const QString& filePath, bool autoDelete = true);

	bool ContinueDownload() const;
	void SetContinueDownload(bool contiueDownload);

	void SetNetworkAccessManager(QNetworkAccessManager* manager);
	void SetData0(const QVariant& data) { m_data0 = data; }
	const QVariant& Data0() { return m_data0; }

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
	void Error(const QString& error);
	void Finished(const QString& filepath);
	void ProgressChanged(qint64 bytesReceived, qint64 bytesTotal);

private:
	bool						m_continueDownload;
	bool						m_aborted;
	bool						m_autoDelete;
	QUrl						m_url;
	QString						m_filePath;
	QNetworkAccessManager*		m_manager;
	QFile *						m_file;
	QNetworkReply *				m_reply;
	QVariant					m_data0;

	Q_DISABLE_COPY(FileDownloader)
};

}

#endif