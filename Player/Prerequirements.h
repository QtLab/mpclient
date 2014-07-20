#ifndef MP_MEDIA_PREREQUIRMENTS
#define MP_MEDIA_PREREQUIRMENTS

#include "Common.h"
#include <QObject>
#include <QSharedPointer>
#include <QList>
#include <QSet>
#include <QMap>
#include <QMultiMap>

#define PLAYER_VERSION "1.0.0.5"

// Forward declaration

QT_BEGIN_NAMESPACE
class QNetworkAccessManager;
class QNetworkReply;
class QNetworkRequest;
class QQuickView;
class QWebFrame;
class QWebPage;
class QContextMenuEvent;
class QMenu;
class QWebSettings;
class QAbstractListModel;
class QVBoxLayout;
class QHBoxLayout;
class QTimer;
class QAbstractItemModel;
class QMouseEvent;
class QLabel;
class QQuickView;
class QQuickItem;
class QQmlContext;
class QNetworkSession;
class QListView;
class QPropertyAnimation;
QT_END_NAMESPACE

class NcFramelessHelper;

namespace mp {

class ChannelMetadata;
class AudioStream;
class PluginManager;
class DownlaodManager;
class UserIdle;
class FlashUtils;
class TracksDataProvider;

typedef AudioStream * AudioStreamPtr;
typedef TracksDataProvider * TracksDataProviderPtr;

namespace view {

class WebView;
class RadioPage;
class PlayerPage;
class TVPage;
class LinkButton;
class Titlebar;
class TabWidget;
class MainWindow;
class SystemTray;
class TabPage;
class TVGenresPopupView;

typedef TVGenresPopupView * TVGenresPopupViewPtr;
typedef TabPage * TabPagePtr;

}

namespace controller {

class IPageController;
class TVPageController;
class RadioPageController;
class UpdateResult;
class UpdateController;
class PlayerPageController;

typedef IPageController* IPageControllerPtr;

}

namespace model {

class Track;
class TrackModel;
typedef QSharedPointer<Track> TrackPtr;
typedef TrackModel * TrackModelPtr;

class UpdateModel;
class FileToUpdate;
typedef QSharedPointer<FileToUpdate> FileToUpdatePtr;
typedef UpdateModel * UpdateModelPtr;
typedef QList<FileToUpdatePtr> FileToUpdateList;

class BannerInfo;
class BannersModel;
typedef QSharedPointer<BannerInfo> BannerInfoPtr;
typedef BannersModel * BannersModelPtr;
typedef QList<int> BannerInfoIds;

class RadioSource;
class RadioSourcesModel;
class RadioSourcesSortFilterProxyModel;
typedef QSharedPointer<RadioSource> RadioSourcePtr;
typedef RadioSourcesModel * RadioSourcesModelPtr;
typedef RadioSourcesSortFilterProxyModel * RadioSourcesSortFilterProxyModelPtr;
typedef QList<RadioSourcePtr> RadioSourceList;

class RadioCategory;
class RadioCategoriesModel;
typedef QSharedPointer<RadioCategory> RadioCategoryPtr;
typedef RadioCategoriesModel * RadioCategoriesModelPtr;
typedef QList<RadioCategoryPtr> RadioCategoryList;
typedef QSet<int> RadioCategoryIds;

class TVCategory;
class TVCategoriesModel;
typedef QSharedPointer<TVCategory> TVCategoryPtr;
typedef TVCategoriesModel * TVCategoriesModelPtr;
typedef QSet<int> TVCategoryIdsSet;

class TVGenre;
class TVGenresModel;
class TVGenresSortFilterProxyModel;
typedef QSharedPointer<TVGenre> TVGenrePtr;
typedef TVGenresModel * TVGenresModelPtr;
typedef TVGenresSortFilterProxyModel* TVGenresSortFilterProxyModelPtr;
typedef QSet<int> TVGenreIdsSet;

typedef QMap<int, int> GenreIdsToCategoryIdMap;

class TVSource;
class TVSourcesModel;
class TVSourcesSortFilterProxyModel;
typedef QSharedPointer<TVSource> TVSourcePtr;
typedef TVSourcesModel * TVSourcesModelPtr;
typedef TVSourcesSortFilterProxyModel* TVSourcesSortFilterProxyModelPtr;
typedef QSet<int> TVSourceIdsSet;
typedef QList<int> TVSourceIds;
}

}

#endif