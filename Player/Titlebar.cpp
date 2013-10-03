#include "Titlebar.h"
#include "ChannelMetadataModel.h"

#include <QHBoxLayout>
#include <QMouseEvent>
#include <QSpacerItem>
#include <QDebug>

namespace mp {

Titlebar::Titlebar(QWidget *parent)
	: m_parent( parent )
{
	m_layout = new QHBoxLayout(this);
	setSizePolicy(QSizePolicy::Expanding , QSizePolicy::Fixed);

	setObjectName("titleBar");
	setAttribute(Qt::WA_StyledBackground);

	qDebug() << "Titlebar layouts created";

	m_logo = new QLabel(this);
	m_logo->setText("MP");
	connect(m_logo, SIGNAL(clicked()), this, SLOT(ShowMenuSlot()));
	m_layout->addWidget(m_logo, 0);

	qDebug() << "Titlebar zaxarLogo created";

	m_layout->addSpacerItem(new QSpacerItem(300, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));


	qDebug() << "Titlebar messages created";

	m_layout->addWidget(new QLabel(), 1);

	m_minimizeLnk = new QLabel(this);
	m_layout->addWidget(m_minimizeLnk);
	m_minimizeLnk->setText("_");

	m_closeLnk = new QLabel(this);
	m_layout->addWidget(m_closeLnk);
	m_closeLnk->setText("X");

	qDebug() << "Titlebar right buttons created";
}


void Titlebar::changeEvent(QEvent *event)
{
	if (event->type() == QEvent::LanguageChange) {
		RetranslateUI();
	}

	QWidget::changeEvent(event);
}

void Titlebar::RetranslateUI()
{
}

void Titlebar::mouseMoveEvent(QMouseEvent *evt)
{
	if(!m_parent->isMaximized())
	{
		QWidget::mouseMoveEvent(evt);
	}
}

void Titlebar::mouseDoubleClickEvent(QMouseEvent *evt)
{
	SwitchScreenMode();
}

void Titlebar::SwitchScreenMode()
{
	if(m_parent->isMaximized())
	{
		m_parent->showNormal();
	}
	else
	{
		m_parent->showMaximized();
	}
}

void Titlebar::MetadataUpdated(const ChannelMetadata& metadata)
{
	m_logo->setText(metadata.ToString());
}

}