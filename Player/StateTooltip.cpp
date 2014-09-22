#include "StateTooltip.h"
#include "WidgetUtils.h"
#include "Path.h"
#include "LinkButton.h"

#include <QVBoxLayout>

namespace mp {
namespace view {


StateTooltip::StateTooltip(QWidget* parent)
	:QDialog(parent)
{
	WidgetUtils::LoadStyleSheets(this, Path::CssFile("StateTooltip.qss"));
	setWindowFlags(Qt::FramelessWindowHint | Qt::Popup);
	setAttribute(Qt::WA_TranslucentBackground);

	QWidget * centralWidget = new QWidget(this);
	centralWidget->setObjectName("centralWidget");

	m_contentLayout = new QVBoxLayout(centralWidget);
	m_contentLayout->setContentsMargins(0, 0, 0, 0);
	
	m_playStop = new LinkButton();
	m_playStop->setObjectName("playStopLnk");
	connect(m_playStop, SIGNAL(clicked()), SLOT(PlayStopClicked()));
	m_contentLayout->addWidget(m_playStop);
}

void StateTooltip::Show(QPoint pos)
{
	move(pos);
	show();
}

void StateTooltip::PlayStopClicked()
{
}
//void TVGenresPopupView::GenreSelected(const QModelIndex & index)
//{
//	close();
//	int genreId = index.data(model::TVGenresModel::Id).toInt();
//	emit TVGenreChanged(genreId);
//}

//void TVGenresPopupView::paintEvent(QPaintEvent *e)
//{
//	QPainter painter(this);
//	painter.setRenderHint(QPainter::Antialiasing);
//	painter.setPen(QPen(QColor(0x31, 0x60, 0x94), 2, Qt::SolidLine, Qt::SquareCap, Qt::BevelJoin));

//	// Left bottom point
//	QPoint triangleAPoint = rect().topLeft();
//	triangleAPoint.setX(triangleAPoint.x() + TriangleLeftMargin);
//	triangleAPoint.setY(triangleAPoint.y() + TriangleHeight);

//	// Right bottom point
//	QPoint triangleBPoint((triangleAPoint.x() + TriangleBaseWidth) + 1, triangleAPoint.y());

//	// The top of the triangle
//	QPoint triangleCPoint((triangleAPoint.x() + TriangleBaseWidth / 2) + 1, 0);

//	painter.drawLine(triangleAPoint, triangleCPoint);
//	painter.drawLine(triangleBPoint, triangleCPoint);

//}

} // end namespace view
} // end namespace mp