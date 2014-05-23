#ifndef MP_FLASH_INSTALLER_H
#define MP_FLASH_INSTALLER_H

#include "Prerequirements.h"

#include <QObject>
#include <QProcess>

namespace mp {

class FlashUtils : public QObject
{
	Q_OBJECT

public:	
	FlashUtils();

	static QByteArray CreateHtmlForCheckFlashInstalled();
	static QByteArray CreateHtmlFlashInstalling();

	void CheckForFlashInstalled();

private slots:
	void PageLoaded(bool ok); 
	void StartDownload();
	void Install(const QString& path);
	void InstallaFinished(int exitCode, QProcess::ExitStatus exitStatus);
	void DownlaodProgressChanged(qint64 bytesReceived, qint64 bytesTotal);

signals:
	void FlashInstallProgressChanged(int percent);
	void FlashInstallStarted();
	void FlashInstalled(bool success);

private:
	bool				m_silent;
	QProcess *			m_process;

};

}

#endif