#ifndef MP_SATE_TOOLIP_H
#define MP_SATE_TOOLIP_H

#include "Prerequirements.h"
#include <QDialog>

namespace mp {
namespace view {

class StateTooltip : public QDialog
{
	Q_OBJECT

public:
	StateTooltip(QWidget* parent);
	void Show(QPoint pos);

private slots:
	void PlayStopClicked();

private:

	void leaveEvent(QEvent * e)
	{
		close();
	}
signals:
	void TVGenreChanged(int genreId);

private:
	LinkButton *		m_playStop;
	QVBoxLayout *		m_contentLayout;
};

} // end namespace view
} // end namespace mp

#endif