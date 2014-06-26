#include "AudioStreamAsyncDownloader.h"

#include <QDebug>

namespace mp {

AudioStreamAsyncDownloader::AudioStreamAsyncDownloader(const QString& url, const QString& filePath, const QVariant& tag)
	:m_url(url)
	,m_tag(tag)
	,m_outputFile(filePath)
{
	setAutoDelete(false);

	m_outputFile.setPermissions(QFile::WriteOwner);

	if(!m_outputFile.open(QIODevice::WriteOnly))
	{
		qDebug() << "AudioStreamAsyncDownloader can't create file: " << filePath
			<< " error: " <<m_outputFile.errorString();
	}
}

AudioStreamAsyncDownloader::~AudioStreamAsyncDownloader()
{
	BASS_StreamFree(m_hStream);
}

bool AudioStreamAsyncDownloader::FileOpened() const
{
	return m_outputFile.isOpen();
}


bool AudioStreamAsyncDownloader::StreamOpened() const
{
	return m_hStream != 0;
}

const QVariant& AudioStreamAsyncDownloader::Tag() const
{ 
	return m_tag; 
}

void AudioStreamAsyncDownloader::run()
{
	if(m_outputFile.isOpen())
	{
		std::string url = m_url.toStdString();

		// try streaming the file/url
		m_hStream =  BASS_StreamCreateURL(url.c_str(), 0, BASS_STREAM_DECODE, AudioStreamAsyncDownloader::DownloadProc, this);

		if(m_hStream)
		{
			BASS_ChannelPlay(m_hStream, TRUE);
		}
		else
		{
			qDebug() << "BASS_StreamCreateURL: " << m_url << " error code: " << BASS_ErrorGetCode();

			m_outputFile.remove();
			EmitDonwlaodFinished();
		}
	}
}

void AudioStreamAsyncDownloader::WriteFile(const void *buffer, DWORD length)
{
	m_outputFile.write(reinterpret_cast<const char*>(buffer), length);
}

void AudioStreamAsyncDownloader::DownloadProc(const void *buffer, DWORD length, void *user)
{
	AudioStreamAsyncDownloader* streamDownlaoder = reinterpret_cast<AudioStreamAsyncDownloader*>(user);

	if(buffer)
	{
		streamDownlaoder->WriteFile(buffer, length);
	}
	else
	{
		QMetaObject::invokeMethod( streamDownlaoder, "EmitDonwlaodFinished", Qt::QueuedConnection);
	}
	
}

void AudioStreamAsyncDownloader::EmitDonwlaodFinished()
{
	emit Finished();

	deleteLater();
}

}