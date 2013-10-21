#include "TabPage.h"

#include <QDebug>
#include <QEvent>
#include <QDeclarativeError>
#include <QtDeclarative/QDeclarativeView>
#include <QDeclarativeContext>

namespace mp {

TabPage::TabPage(QWidget* parent)
	:QWidget(parent)
{
}

TabPage::~TabPage()
{
}

void TabPage::Enter()
{
}

bool TabPage::Leave()
{
	return true;
}

void TabPage::RetranslateUI()
{
}

void TabPage::changeEvent(QEvent * evt)
{
	if (evt->type() == QEvent::LanguageChange) 
	{
		RetranslateUI();
	}
	else 
	{
		QWidget::changeEvent(evt);
	}
}

void TabPage::ChannelSourceModelUpdated(ChannelSourceModel* channels)
{
}

void TabPage::GenreModelUpdated(GenreModel* channels)
{
}

QMargins TabPage::Margins()
{
	return QMargins(1, 0, 1, 0);
}

QDeclarativeView* TabPage::CreateDeclarativeView(const QString& qmlFileName, const QString& key, QObject* obj)
{
    QDeclarativeView * ui = new QDeclarativeView(this);

	ui->rootContext()->setContextProperty("radioChannels", obj);

	ui->setSource(QUrl(qmlFileName));
    ui->setResizeMode(QDeclarativeView::SizeRootObjectToView);

	QDeclarativeError err;
	foreach(err, ui->errors())
	{
		qDebug() << "CreateDeclarativeView error: " << err.toString();
	}

	return ui;
}

}