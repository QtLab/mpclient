#include "MessageBoxView.h"
#include "Config.h"
#include <QMessageBox>

namespace mp {
namespace view {

void MessageBoxView::ShowAbout(QWidget* parent)
{
	QMessageBox::information(parent, tr("UnisonBox"), tr("UnisonBox version: %0").arg(Config::Inst().Version()));
}

void MessageBoxView::ShowNewVersionFound(QWidget* parent)
{
	QMessageBox::information(parent, tr("UnisonBox"), tr("Found a new version"));
}

void MessageBoxView::ShowNewVersionNotFound(QWidget* parent)
{
	QMessageBox::information(parent, tr("UnisonBox"), tr("A new version isn't found"));
}

}
}