#ifndef MP_MULTI_EDIT_DIALOG_H
#define MP_MULTI_EDIT_DIALOG_H

#include <QDialog>
#include <QTabWidget>
#include <QWidget>

class QTextEdit;
class QGridLayout;
class QVBoxLayout;
class QHBoxLayout;
class QPushButton;
class QLabel;
class QFrame;

namespace mp {

class MultiEditDialog : public QDialog
{
	Q_OBJECT

public:
	MultiEditDialog(const QString& text);
	virtual ~MultiEditDialog();
	void SetCaption(const QString& caption);
	void SetInformativeText(const QString& message);
	QString ShowModal(bool ontop = false);

protected:
	void mousePressEvent(QMouseEvent *evt);
	void mouseMoveEvent(QMouseEvent *evt);

protected:
	QVBoxLayout*	m_layout;
	QHBoxLayout *	m_headerLayout;
	QHBoxLayout *	m_contentLayout;
	QHBoxLayout *	m_footerLayout;

	QLabel *		m_headerLbl;
	QLabel *		m_messageLbl;
	QFrame *		m_okPlaceholder;
	QPushButton *	m_btnOk;
	QFrame *		m_cancelPlaceholder;
	QPushButton *	m_btnCancel;

	QTextEdit *		m_edit;
	QPoint			m_cursorPosition;
};

}

#endif