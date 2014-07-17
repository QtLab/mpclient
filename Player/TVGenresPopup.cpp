#include "TVGenresPopup.h"
#include "TVGenresModel.h"
#include "WidgetUtils.h"
#include "Config.h"
#include "Path.h"

#include <QPainter>
#include <QVBoxLayout>
#include <QListView>
#include <QStyledItemDelegate>
#include <QFontMetrics>
#include <QApplication>
#include <QDesktopWidget>
#include <QScrollBar>

namespace mp {
namespace view {

const int GenreItemWidth			= 156;
const int GenreItemHeight			= 30;
const int GenreItemLeftMargin		= 14;
const int GenreItemTopMargin		= 5;

const int TriangleHeight			= 8;
const int TriangleLeftMargin		= 22;
const int TriangleBaseWidth			= 15;

class GenreViewDelegate : public QStyledItemDelegate
{
public:
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
	QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

class CentralWidget : public QWidget
{
public:
	CentralWidget(QWidget* parent = NULL);
	void paintEvent(QPaintEvent *e);

};

TVGenresPopup::TVGenresPopup(QAbstractItemModel* genresModel, QWidget* parent)
	:QDialog(parent)
{
	WidgetUtils::LoadStyleSheets(this, Path::CssFile("TVGenresPopup.qss"));
	setWindowFlags(Qt::FramelessWindowHint | Qt::Popup);
	setAttribute(Qt::WA_TranslucentBackground);

	m_genresListView = new QListView();
	m_genresListView->setModel(genresModel);
	m_genresListView->setItemDelegate(new GenreViewDelegate());
	m_genresListView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	m_genresListView->setFixedWidth(GenreItemWidth);
	m_genresListView->setCursor(QCursor(Qt::PointingHandCursor));
	connect( m_genresListView, SIGNAL(clicked(const QModelIndex&)), SLOT(GenreSelected(const QModelIndex&)));

	m_wrapLayout = new QVBoxLayout(this);
	m_wrapLayout->setContentsMargins(0, TriangleHeight, 0, 0);

	m_centralWidget = new CentralWidget();
	m_centralWidget->setObjectName("centralWidget");
	m_wrapLayout->addWidget(m_centralWidget);

	m_contentLayout = new QVBoxLayout(m_centralWidget);
	m_contentLayout->setContentsMargins(2, 2, 0, 2);
	m_contentLayout->addWidget(m_genresListView, 0);
}

void TVGenresPopup::Show(QPoint pos)
{
	int rowCount = m_genresListView->model()->rowCount();

	if(rowCount > 0)
	{
		const int desktopBottom = QApplication::desktop()->rect().bottom();
		const int maxHeight = desktopBottom - (pos.y() + 5);

		int height = (rowCount * GenreItemHeight) + 8;

		if(height > maxHeight)
		{
			height = maxHeight;
		}

		m_centralWidget->setFixedHeight(height);

		const int topBottomMargins = m_contentLayout->contentsMargins().top() 
									+ m_contentLayout->contentsMargins().bottom();

		m_genresListView->setFixedHeight(height - topBottomMargins);
		m_genresListView->verticalScrollBar()->setValue(0);
		
		move(pos);
		show();
	}
}

void TVGenresPopup::GenreSelected(const QModelIndex & index)
{
	close();
	int genreId = index.data(model::TVGenresModel::Id).toInt();
	emit TVGenreChanged(genreId);
}

void TVGenresPopup::paintEvent(QPaintEvent *e)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setPen(QPen(QColor(0x31, 0x60, 0x94), 2, Qt::SolidLine, Qt::SquareCap, Qt::BevelJoin));
	
	// Left bottom point
	QPoint triangleAPoint = rect().topLeft();
	triangleAPoint.setX(triangleAPoint.x() + TriangleLeftMargin);
	triangleAPoint.setY(triangleAPoint.y() + TriangleHeight);

	// Right bottom point
	QPoint triangleBPoint((triangleAPoint.x() + TriangleBaseWidth) + 1, triangleAPoint.y());

	// The top of the triangle
	QPoint triangleCPoint((triangleAPoint.x() + TriangleBaseWidth /2) + 1, 0);

	painter.drawLine(triangleAPoint, triangleCPoint);
	painter.drawLine(triangleBPoint, triangleCPoint);

}

CentralWidget::CentralWidget(QWidget* parent)
	:QWidget(parent)
{
}

void CentralWidget::paintEvent(QPaintEvent *e)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setPen(QPen(QColor(0xFF, 0xFF, 0xFF), 4, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
	float x1 = float(TriangleLeftMargin) + 2.0f;
	float x2 = float(TriangleLeftMargin + TriangleBaseWidth) - 1.5f;
	painter.drawLine(QPointF(x1, 0.0f), QPointF(x2, 0.0f));
}

void GenreViewDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	QString name = index.data(model::TVGenresModel::Name).toString();

	QFont font = Config::Inst().DefaultFont();
	font.setBold(true);
	painter->setFont(font);

	QFontMetrics fm(option.font);

	int textWidth = GenreItemWidth - (GenreItemLeftMargin + 20);
	name = fm.elidedText(name, Qt::TextElideMode::ElideRight, textWidth);

	QRect textRect(option.rect);
	textRect.setLeft(textRect.left() + GenreItemLeftMargin);
	textRect.setTop(textRect.top() + GenreItemTopMargin);

	QColor textColor = QColor(0x37, 0x2F, 0x2C);

	if((option.state & QStyle::State_MouseOver) == QStyle::State_MouseOver)
	{
		textColor = QColor(0x3C, 0x86, 0xC5);
	}

	painter->save();
	painter->setPen(textColor);
	painter->drawText(textRect, name);
	painter->restore();

	painter->save();
	painter->setPen(QColor(0xE5, 0xEF, 0xF8));
	painter->drawLine(option.rect.bottomLeft(), option.rect.bottomRight());
	painter->restore();
}

QSize GenreViewDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	return QSize(GenreItemWidth, GenreItemHeight);
}

} // end namespace view
} // end namespace mp