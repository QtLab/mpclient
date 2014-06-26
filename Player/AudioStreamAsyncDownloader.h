#ifndef MP_AUDIO_STREAM_ASYNC_DOWNLOADER_H
#define MP_AUDIO_STREAM_ASYNC_DOWNLOADER_H

#include <QObject>
#include <QRunnable>
#include <QVariant>
#include <QFile>

#include "bass.h"

namespace mp {

class AudioStreamAsyncDownloader: public QObject, public QRunnable
{
     Q_OBJECT
public:
	AudioStreamAsyncDownloader(const QString& url, const QString& filePath, const QVariant& tag = QVariant());
	~AudioStreamAsyncDownloader();

	bool FileOpened() const;
	bool StreamOpened() const;
	const QVariant& Tag() const;

private:
	void run();
	void WriteFile(const void *buffer, DWORD length);
	static void CALLBACK DownloadProc(const void *buffer, DWORD length, void *user);

private slots:
	void EmitDonwlaodFinished();

signals:
    void Finished();

private:
	HSTREAM					m_hStream;
	QString					m_url;
	QVariant				m_tag;
	QFile					m_outputFile;
};

}

#endif