#ifndef MP_LINK_BUTTON_H
#define MP_LINK_BUTTON_H

#include <QLabel>
#include <QMouseEvent>

namespace mp {
namespace view {

class LinkButton : public QLabel
{
	Q_OBJECT

public:
	LinkButton(QWidget *parent = 0, const QString& name = QString());

	void SetTag(const QVariant& tag);
	QVariant Tag();

signals:
    void clicked();

protected:
	void mouseReleaseEvent(QMouseEvent *e);

private:
	QVariant m_tag;
};

}
}

#endif