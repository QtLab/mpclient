#include "WidgetUtils.h"

#include <QMutexLocker>
#include <QPixmap>
#include <QIcon>
#include <QPushButton>
#include <QLabel>
#include <QFile>
#include <QWidget>
#include <QDebug>
#include <QDesktopServices>
#include <QMainWindow>
#include <QStyle>
#include <QFileInfo>

namespace mp {

QMap<QString, QString> WidgetUtils::m_cachedStyles = QMap<QString, QString>();

void WidgetUtils::OnpenLinskInDefaultBrowser(const QString& url)
{

}

void WidgetUtils::SetButtonImage(QPushButton* btn, const QString& imgSrc, bool borderNone, int dx, int dy)
{
    QPixmap pixmap(imgSrc);
    QIcon icon(pixmap);
    QSize  iconSize(pixmap.width() - dx, pixmap.height() - dy);
    btn->setIconSize(iconSize);
    btn->setIcon(icon);

	if(borderNone)
		btn->setStyleSheet("QPushButton { border: none;}");

	btn->setCursor(Qt::PointingHandCursor);
}

void WidgetUtils::LoadStyleSheets(QWidget* widget, const QString& qss)
{
	QString filePath;

	if(qss.isEmpty())
	{
		QString className = widget->metaObject()->className();
		className.replace("::", "");
		filePath = "Styles/"+ className + ".qss";
	}
	else
	{
		filePath =  QFileInfo(qss).absoluteFilePath();
	}

	QMap<QString, QString>::const_iterator iter;
	iter = m_cachedStyles.find(filePath);

	QFile file(filePath);
	if(file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QString styleSheet = file.readAll();
		widget->setStyleSheet(styleSheet);	
		m_cachedStyles.insert(filePath, styleSheet);
	}
	else
	{
		qDebug() << "Stylesheet: " << filePath << "doesn't found";
	}
}

void WidgetUtils::UpdateWidgetStyleSheets(QWidget* widget)
{
	widget->style()->unpolish(widget);
	widget->style()->polish(widget);
	widget->update();
}

}