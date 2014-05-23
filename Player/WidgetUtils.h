#ifndef MP_WIDGET_UTILS_H
#define MP_WIDGET_UTILS_H

#include <QString>
#include <QObject>
#include <QMap>
#include <QMutex>

class QPushButton;
class QWidget;
class QLabel;
class QMainWindow;

namespace mp {

class WidgetUtils
{
public:
	// ������������� ������ ������� �� �������, ������� border
	static void SetButtonImage(QPushButton* btn, const QString& imgSrc, bool borderNone = true, int dx = 0, int dy = 0);
	static void LoadStyleSheets(QWidget* widget, const QString& qss = QString::null);
	static void UpdateWidgetStyleSheets(QWidget* widget);

private:
	WidgetUtils();
};

}

#endif