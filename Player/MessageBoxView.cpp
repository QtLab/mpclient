#include "MessageBoxView.h"
#include "Config.h"
#include "WidgetUtils.h"
#include "LinkButton.h"
#include <QMessageBox>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

namespace mp {
namespace view {

const int MinDialogWidht = 200;

class MessageBoxPopup : public QDialog
{
public:
	MessageBoxPopup(QWidget* parent, const QString& captionTxt, const QString& contentTxt, 
					const QString& okTxt = tr("Ok"), const QString& cancelTxt = QString() )
		:QDialog(parent)
	{
		WidgetUtils::LoadStyleSheets(this, "Styles//MessageBoxPoup.qss");

		QWidget * centralWidget = new QWidget(this);
		centralWidget->setObjectName("centralWidget");
		QVBoxLayout * layout = new QVBoxLayout(centralWidget);
		layout->setContentsMargins(0, 0, 0, 0);
		layout->setSpacing(0);
		setLayout(layout);

		// Header
		QWidget* headerWidget = new QWidget();
		headerWidget->setObjectName("headerWidget");
		QHBoxLayout * headerLyout = new QHBoxLayout(headerWidget);
		QLabel * captionLbl = new QLabel();
		captionLbl->setObjectName("captionLabel");
		captionLbl->setText(captionTxt);
		headerLyout->addWidget(captionLbl, 1, Qt::AlignLeft);
		LinkButton * closeLnk = new LinkButton();
		closeLnk->setObjectName("closeLnk");
		connect(closeLnk, SIGNAL(clicked()), SLOT(reject()));
		headerLyout->addWidget(closeLnk, 1, Qt::AlignRight);
		headerLyout->setContentsMargins(22, 0, 5, 0);

		// Content
		QWidget* contentWidget = new QWidget();
		contentWidget->setObjectName("contentWidget");
		QHBoxLayout * contentLayout = new QHBoxLayout(contentWidget);
		QLabel * contentLbl = new QLabel();
		contentLbl->setObjectName("contentLbl");
		contentLbl->setText(contentTxt);
		contentLayout->addWidget(contentLbl, 1,  Qt::AlignLeft);
		contentLayout->setContentsMargins(20, 0, 5, 0);

		int w = contentLbl->fontMetrics().width(contentTxt);

		bool hasSpacers = false;// contentLbl->fontMetrics().width(contentTxt) <= MinDialogWidht;

		// Footer
		QWidget* footerWiget = new QWidget();
		footerWiget->setObjectName("footerWiget");
		QHBoxLayout * footerLayout = new QHBoxLayout(footerWiget);
		footerLayout->setContentsMargins(0, 0, 10, 0);
		
		// Reject button
		if (!cancelTxt.isEmpty())
		{
			if (hasSpacers)
				footerLayout->addStretch(1);

			LinkButton * btnCancel = new LinkButton();
			btnCancel->setText(cancelTxt);
			btnCancel->setObjectName("lnkCancel");
			connect(btnCancel, SIGNAL(clicked()), SLOT(reject()));
			footerLayout->addWidget(btnCancel, 1, Qt::AlignRight);

			//if (hasSpacers)
				//footerLayout->addStretch(1);
		}

		// Accept button
		LinkButton * btnOk = new LinkButton();
		btnOk->setText(okTxt);
		btnOk->setObjectName("lnkOk");
		connect(btnOk, SIGNAL(clicked()), SLOT(accept()));
		footerLayout->addWidget(btnOk, 1, Qt::AlignRight);

		if (hasSpacers)
			footerLayout->addStretch(1);

		QWidget * contectAndFooterWidget = new QWidget();
		centralWidget->setObjectName("contectAndFooterWidget");
		QVBoxLayout * contectAndFooterLayout = new QVBoxLayout(contectAndFooterWidget);
		contectAndFooterLayout->setContentsMargins(0, 0, 0, 0);
		contectAndFooterLayout->setSpacing(0);

		layout->addWidget(headerWidget, 1);
		contectAndFooterLayout->addWidget(contentWidget, 1);
		contectAndFooterLayout->addWidget(footerWiget, 1);
		layout->addWidget(contectAndFooterWidget, 1);

		setWindowFlags(Qt::Widget | Qt::FramelessWindowHint);
		setParent(0); // Create TopLevel-Widget
		setAttribute(Qt::WA_TranslucentBackground, true);
		setAttribute(Qt::WA_NoSystemBackground, true);
		setAttribute(Qt::WA_PaintOnScreen);
	}

	virtual ~MessageBoxPopup()
	{
	}

private:
	void mousePressEvent(QMouseEvent *evt)
	{
		if (evt->button() == Qt::LeftButton)
		{
			m_cursorPosition = evt->pos();
		}

		QWidget::setMouseTracking(true);
		update();
	}
	
	void mouseMoveEvent(QMouseEvent *evt)
	{
		if (evt->buttons() && Qt::LeftButton)
		{
			QPoint diff = evt->pos() - m_cursorPosition;
			QPoint newpos = pos() + diff;

			move(newpos);
		}

		update();
	}

private:
	QPoint		m_cursorPosition;
};

void MessageBoxView::ShowAbout(QWidget* parent)
{
	MessageBoxPopup msgBox(parent, tr("UnisonBox"), tr("UnisonBox version: %0").arg(Config::Inst().Version()));
	msgBox.setModal(true);
	msgBox.exec();
}

void MessageBoxView::ShowNewVersionFound(QWidget* parent)
{
	MessageBoxPopup msgBox(parent, tr("UnisonBox"), tr("Found a new version"));
	msgBox.setModal(true);
	msgBox.exec();
}

void MessageBoxView::ShowNewVersionNotFound(QWidget* parent)
{
	MessageBoxPopup msgBox(parent, tr("UnisonBox"), tr("A new version isn't found"));
	msgBox.setModal(true);
	msgBox.exec();
}

} // end namespace view
} // end namespace mp