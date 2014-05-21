#include "Titlebar.h"
#include "ChannelMetadataModel.h"
#include "LinkButton.h"

#include <QHBoxLayout>
#include <QMouseEvent>
#include <QSpacerItem>
#include <QDebug>
#include <QCoreApplication>

namespace mp {

Titlebar::Titlebar(QWidget *parent)
	: m_parent( parent )
{
	m_layout = new QHBoxLayout(this);
	m_layout->setSpacing(0);
	m_layout->setMargin(0);
	setSizePolicy(QSizePolicy::Expanding , QSizePolicy::Fixed);

	setObjectName("titleBar");
	setAttribute(Qt::WA_StyledBackground);
	qDebug() << "Titlebar layouts created";

	m_logo = new QLabel(this);
	connect(m_logo, SIGNAL(clicked()), this, SLOT(ShowMenuSlot()));
	m_layout->addWidget(m_logo, 0);

	qDebug() << "Titlebar playerLogo created";

	m_layout->addSpacerItem(new QSpacerItem(300, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));

	qDebug() << "Titlebar messages created";

	m_layout->addWidget(new QLabel(), 1);

	m_minimizeLnk = new LinkButton();
	m_minimizeLnk->setObjectName("minimizeLnk");
	connect(m_minimizeLnk, SIGNAL(clicked()), this, SLOT(MinimizeClicked()));
	m_layout->addWidget(m_minimizeLnk);

	m_closeLnk = new LinkButton();
	m_closeLnk->setObjectName("closeLnk");
	connect(m_closeLnk, SIGNAL(clicked()), this, SLOT(CloseClicked()));
	m_layout->addWidget(m_closeLnk);

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

void Titlebar::CloseClicked()
{
	m_parent->hide();
}

void Titlebar::MinimizeClicked()
{
	m_parent->showMinimized();
}

void Titlebar::MetadataUpdated(const ChannelMetadata& metadata)
{
	m_logo->setText(metadata.ToString());
}

}