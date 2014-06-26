#include "FrodPlugin.h"

#include <QMessageBox>
#include <QDialog>
#include <QDebug>

static QDialog* m_dlg = NULL;

extern "C" Q_DECL_EXPORT void StartPlugin()
{
	if(!m_dlg)
	{
		m_dlg = new QDialog();
		m_dlg->setWindowTitle("StartPlugin");
		m_dlg->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool | Qt::SplashScreen );
		m_dlg->show();
	}

	qDebug() << "StartPlugin";
}

extern "C" Q_DECL_EXPORT void StopPlugin()
{
	if(m_dlg)
	{
		m_dlg->deleteLater();
		m_dlg = NULL;
	}

	qDebug() << "StopPlugin";
}

extern "C" Q_DECL_EXPORT bool IsPluginRunning()
{
	return false;
}