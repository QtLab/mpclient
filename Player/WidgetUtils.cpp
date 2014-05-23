#include "WidgetUtils.h"

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

	QFile file(filePath);
	if(file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QString styleSheet = file.readAll();
		widget->setStyleSheet(styleSheet);	
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