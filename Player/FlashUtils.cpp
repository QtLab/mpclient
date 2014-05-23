#include "FlashUtils.h"
#include "FileDownloader.h"
#include "Path.h"

#include <QProcess>
#include <QDebug>
#include <QFile>
#include <QWebFrame>
#include <QWebPage>

namespace mp {

FlashUtils::FlashUtils()
	:m_silent(true)
{
}

QByteArray FlashUtils::CreateHtmlForCheckFlashInstalled()
{
	QFile file(":/mp/Resources/CheckFlashInstalled.html");
	bool opened = file.open(QIODevice::ReadOnly);
	QByteArray html = file.readAll();
	return html;
}

QByteArray FlashUtils::CreateHtmlFlashInstalling()
{
	QFile file(":/mp/Resources/FlashInstall.html");
	bool opened = file.open(QIODevice::ReadOnly);
	QByteArray html = file.readAll();
	return html;
}


void FlashUtils::CheckForFlashInstalled()
{
	QWebPage * testPage = new QWebPage(this);
	connect(testPage, SIGNAL(loadFinished(bool)), SLOT(PageLoaded(bool)));
	testPage->mainFrame()->setContent(CreateHtmlForCheckFlashInstalled());
}

void FlashUtils::PageLoaded(bool ok)
{
	QWebPage* testPage = qobject_cast<QWebPage*>(sender());
	QVariant result = testPage->mainFrame()->evaluateJavaScript("swfobject.hasFlashPlayerVersion('11')");
	
	qDebug() << "swfobject.hasFlashPlayerVersion('11') returns: " << result;

	bool flashInstalled = result.toBool();

	if(!flashInstalled)
	{
		StartDownload();

		emit FlashInstallStarted();
	}
	else
	{
		emit FlashInstalled(true);
		deleteLater();
	}
}

void FlashUtils::StartDownload()
{
	QUrl url("http://download.macromedia.com/get/flashplayer/current/licensing/win/install_flash_player_11_plugin.msi");

	FileDownloader * downloader = new FileDownloader(url, FlashMSIPath());
	connect(downloader, SIGNAL(Finished(const QString&)), SLOT(Install(const QString&)));
	connect(downloader, SIGNAL(ProgressChanged(qint64, qint64)), SLOT(DownlaodProgressChanged(qint64, qint64)));
	downloader->Do();
}

void FlashUtils::Install(const QString& path)
{
	static const QString silentCmdTemplate("msiexec.exe /qn /norestart /i \"%0\" /L installflash.log");
	static const QString cmdTemplate("msiexec.exe /norestart /i \"%0\" /L installflash.log");
	
	QString cmd = m_silent ? silentCmdTemplate.arg(path) : cmdTemplate.arg(path);

	qDebug() << "Start process with arguments: "<< cmd;

	m_process = new QProcess(this);
	connect(m_process, SIGNAL(finished (int, QProcess::ExitStatus)), SLOT(InstallaFinished(int, QProcess::ExitStatus)));
	m_process->start(cmd);

	emit FlashInstallProgressChanged(99);
}

void FlashUtils::InstallaFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
	qDebug() << "Flash installation finished with status: " << exitCode;

	if(exitCode != 0)
	{
		if(m_silent)
		{
			m_silent = false;
			Install(FlashMSIPath());
		}
	}
	else
	{
		emit FlashInstalled(exitCode == 0);
		deleteLater();
	}
}

void FlashUtils::DownlaodProgressChanged(qint64 bytesReceived, qint64 bytesTotal)
{
	float t = static_cast<float>(bytesReceived) / static_cast<float>(bytesTotal);
	float percents = static_cast<int>(t * 100) - 1;
	emit FlashInstallProgressChanged(percents < 0 ? 0 : percents);
}

}