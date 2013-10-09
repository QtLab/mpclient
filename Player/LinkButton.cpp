#include "LinkButton.h"

namespace  mp {
LinkButton::LinkButton(QWidget *parent, const QString& name) 
	: QLabel(parent)
{
	setCursor(Qt::PointingHandCursor);
	setAttribute(Qt::WA_StyledBackground);

	if(!name.isEmpty())
		setObjectName(name);
};

void LinkButton::SetTag(const QVariant& tag)
{
	m_tag = tag;
}

QVariant LinkButton::Tag()
{
	return m_tag;
}


void LinkButton::mouseReleaseEvent(QMouseEvent *e)
{
	if(e->button() == Qt::LeftButton)
	{
		emit clicked();
	}
}
}