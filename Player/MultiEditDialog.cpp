#include "MultiEditDialog.h"
#include "WidgetUtils.h"

#include <QMouseEvent>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QFrame>
#include <QDialogButtonBox>
#include <QRadioButton>
#include <QGroupBox>
#include <QSpacerItem>
#include <QFrame>
#include <QTextEdit>

namespace mp {

MultiEditDialog::MultiEditDialog(const QString& text)
	:QDialog()
	//:QDialog(GBApp::Inst().View()->isVisible() ? GBApp::Inst().View() : NULL)
	,m_messageLbl(0)
	,m_headerLbl(0)
	,m_btnOk(0)
	,m_btnCancel(0)
{	
	setWindowFlags(Qt::FramelessWindowHint | Qt::Tool | Qt::SplashScreen );
	WidgetUtils::LoadStyleSheets(this);

	m_layout = new QVBoxLayout(this);
	m_headerLayout = new QHBoxLayout();
	m_contentLayout = new QHBoxLayout();
	m_footerLayout = new QHBoxLayout();

	m_layout->setContentsMargins(0, 0, 0, 0);
	m_layout->addLayout(m_headerLayout);
	m_layout->addLayout(m_contentLayout);
	m_layout->addLayout(m_footerLayout, 0);

	SetCaption(QString::null);
	
	m_contentLayout = new QHBoxLayout();
	m_edit = new QTextEdit(this);
	m_edit->setText(text);
	m_edit->setFixedWidth(500);
	m_edit->setFixedHeight(500);
	setFixedWidth(500);
	setFixedHeight(500);
	m_footerLayout->setSpacing(0);
	m_footerLayout->setContentsMargins(0, 0, 0, 0);

	m_btnOk = new QPushButton();
	m_btnOk->setObjectName("btnAccept");
	m_btnOk->setText(tr("Ok"));
	m_footerLayout->addWidget(m_btnOk, 0);

	m_btnCancel = new QPushButton(this);
	m_btnCancel->setObjectName("btnReject");
	m_btnCancel->setText(tr("Cancel"));
	m_footerLayout->addWidget(m_btnCancel, 0);

	connect(m_btnCancel, SIGNAL(clicked()), SLOT(reject()));
}

MultiEditDialog::~MultiEditDialog()
{
}

void MultiEditDialog::SetCaption(const QString& caption)
{
	if(!m_headerLbl)
	{
		m_headerLbl = new QLabel(this);
		m_headerLbl->setObjectName("headerLbl");
		m_headerLayout->addWidget(m_headerLbl, 1, Qt::AlignHCenter);
	}

	m_headerLbl->setText(caption);
}

void MultiEditDialog::SetInformativeText(const QString& message)
{
	if(!m_messageLbl)
	{
		m_messageLbl = new QLabel;
		m_contentLayout->addWidget(m_messageLbl, 1);
	}

	m_messageLbl->setTextFormat(Qt::RichText);
	m_messageLbl->setTextInteractionFlags(Qt::TextBrowserInteraction);
	m_messageLbl->setOpenExternalLinks(true);

	m_messageLbl->setText(message);
	m_messageLbl->setWordWrap(true);
	m_messageLbl->setObjectName("message");
}

QString MultiEditDialog::ShowModal(bool ontop)
{
	setModal(true);

	if(ontop)
		setWindowFlags(Qt::FramelessWindowHint | Qt::Tool | Qt::SplashScreen | Qt::WindowStaysOnTopHint);

	exec();
	return m_edit->toPlainText();
}

void MultiEditDialog::mousePressEvent(QMouseEvent *evt)
{
	QWidget::mousePressEvent(evt);

	if( evt->button() == Qt::LeftButton )
	{
		m_cursorPosition = evt->pos();
		
	}

	QWidget::setMouseTracking(true);
	update();
}

void MultiEditDialog::mouseMoveEvent(QMouseEvent *evt)
{
	if (evt->buttons() && Qt::LeftButton) 
	{
		QPoint diff = evt->pos() - m_cursorPosition;
		QPoint newpos = pos() + diff;
		move(newpos);
	}
}

}