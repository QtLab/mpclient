#ifndef MP_TV_GENRES_POPUP_H
#define MP_TV_GENRES_POPUP_H

#include "Prerequirements.h"
#include <QDialog>

namespace mp {
namespace view {

class TVGenresPopupView : public QDialog
{
	Q_OBJECT

public:
	TVGenresPopupView(QAbstractItemModel* genresModel, QWidget* parent = NULL);
	void Show(QPoint pos);

private slots:
	void GenreSelected(const QModelIndex & index);

private:
	void paintEvent(QPaintEvent *e);
	void leaveEvent(QEvent * e)
	{
		close();
	}
signals:
	void TVGenreChanged(int genreId);
	
private:
	QVBoxLayout *	m_wrapLayout;
	QWidget *		m_centralWidget;
	QVBoxLayout *	m_contentLayout;
	QListView *		m_genresListView;
};

} // end namespace view
} // end namespace mp

#endif