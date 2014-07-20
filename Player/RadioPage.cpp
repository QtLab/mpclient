#include "RadioPage.h"
#include "Path.h"

#include <QDebug>
#include <QQuickView>
#include <QQmlContext>
#include <QQuickItem>
#include <QAbstractItemModel>

namespace mp {
namespace view {

RadioPage::RadioPage(QWidget* parent, QAbstractItemModel* categoriesModel, QAbstractItemModel* allStationsModel, 
					QAbstractItemModel* topStationsModel, QAbstractItemModel* lastStationsModel, QAbstractItemModel* searchStationsModel)
	:QuickPageBase(parent, "Radio\\RadioPageView.qml")
{
	RootContext()->setContextProperty("categoriesModel", categoriesModel);
	RootContext()->setContextProperty("allStationsModel", allStationsModel);
	RootContext()->setContextProperty("lastStationsModel", lastStationsModel);
	RootContext()->setContextProperty("topStationsModel", topStationsModel);
	RootContext()->setContextProperty("searchStationsModel", searchStationsModel);

	connect(RootQuickItem(), SIGNAL(categoryChanged(int)), this, SIGNAL(CategoryChanged(int)));
	connect(RootQuickItem(), SIGNAL(resumeRadio()), this, SIGNAL(ResumeRadio()));
	connect(RootQuickItem(), SIGNAL(playRadio(int)), this, SIGNAL(PlayRadio(int)));
	connect(RootQuickItem(), SIGNAL(volumeChanged(qreal)), this, SIGNAL(VolumeChanged(qreal)));
	connect(RootQuickItem(), SIGNAL(pauseRadio()), this, SIGNAL(PauseRadio()));
	connect(RootQuickItem(), SIGNAL(searchFilterChanged(QString)), this, SIGNAL(SearchFilterChanged(QString)));
	connect(RootQuickItem(), SIGNAL(searchTracks(QString)), this, SIGNAL(SearchTracks(QString)));
}

RadioPage::~RadioPage()
{
}

void RadioPage::Enter()
{
	QuickPageBase::Enter();
}

bool RadioPage::Leave()
{
	return QuickPageBase::Leave();
}

void RadioPage::RetranslateUI()
{
}

QString RadioPage::Name() const
{
	return tr("Radio");
}

QString RadioPage::ToolTip() const
{
	return tr("a collection of the best radio stations from around the world");
}

bool RadioPage::Resizable() const
{
	return false;
}

QSize RadioPage::RestoreSize() const
{
	return QSize(580, 351);
}

void RadioPage::SaveSize(const QSize& size)
{
}

void RadioPage::SetVolume(qreal val)
{
	QMetaObject::invokeMethod(RootQuickItem(), "setVolume", Q_ARG(QVariant, val));
}

void RadioPage::SetPlayingState(bool play)
{
	QMetaObject::invokeMethod(RootQuickItem(), "setPlayingState", Q_ARG(QVariant, play));
}

void RadioPage::SetMetadata(const QString& metadata)
{
	QMetaObject::invokeMethod(RootQuickItem(), "updateMetadata", Q_ARG(QVariant, metadata));
}

void RadioPage::SetPlayStationName(const QString& name)
{
	QMetaObject::invokeMethod(RootQuickItem(), "setPlayStationName", Q_ARG(QVariant, name));
}

void RadioPage::SetCategory(int id, bool topVisible)
{
	QMetaObject::invokeMethod(RootQuickItem(), "setCategory", Q_ARG(QVariant, id), Q_ARG(QVariant, topVisible));
}

void RadioPage::UpdateTopVisibleCategories()
{
	QMetaObject::invokeMethod(RootQuickItem(), "updateTopVisibleCategories");
}

} // end namespace view
} // end namespace mp