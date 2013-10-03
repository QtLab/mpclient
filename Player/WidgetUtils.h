#ifndef ZAXAR_WIDGET_UTILS_H
#define ZAXAR_WIDGET_UTILS_H

#include <QString>
#include <QObject>
#include <QMap>
#include <QMutex>

class QPushButton;
class QWidget;
class QLabel;
class QMainWindow;

namespace Zaxar {

class WidgetUtils
{
public:
	//static void DrawTextOnIcon();
	static void OnpenLinskInDefaultBrowser(const QString& url);
	// Устанавливает кнопке картику из ресуров, убирает border
	static void SetButtonImage(QPushButton* btn, const QString& imgSrc, bool borderNone = true, int dx = 0, int dy = 0);
	static void LoadStyleSheets(QWidget* widget, const QString& qss = QString::null);
	static void UpdateWidgetStyleSheets(QWidget* widget);

private:
	WidgetUtils();
	static QMap<QString, QString>	m_cachedStyles;
};

}

#endif