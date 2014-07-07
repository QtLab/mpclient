#ifndef MP_MEDIA_PREREQUIRMENTS
#define MP_MEDIA_PREREQUIRMENTS

#include "Common.h"
#include <QObject>
#include <QSharedPointer>
#include <QList>
#include <QSet>
#include <QMap>

#define PLAYER_VERSION "1.0.0.0"

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
QT_END_NAMESPACE

class NcFramelessHelper;

namespace mp {

class ChannelMetadata;
class AudioStream;
class PluginManager;
class DownlaodManager;
class UserIdle;
class FlashUtils;
class TrackModelDataAccessor;

typedef AudioStream * AudioStreamPtr;
typedef TrackModelDataAccessor * TrackModelDataAccessorPtr;

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

typedef TabPage * TabPagePtr;

}

namespace controller {

class IPageController;
class TVPageController;
class RadioPageController;
class UpdateController;
class PlayerPageController;

typedef IPageController* IPageControllerPtr;

}

namespace model {

class ChannelSourceSortFilterProxyModel;

class Track;
class TrackModel;
typedef QSharedPointer<Track> TrackPtr;
typedef TrackModel * TrackModelPtr;

class UpdateModel;
class FileToUpdate;
typedef QSharedPointer<FileToUpdate> FileToUpdatePtr;
typedef UpdateModel * UpdateModelPtr;
typedef QList<FileToUpdatePtr> FileToUpdateList;

class ChannelSource;
class ChannelSourceModel;
typedef QSharedPointer<ChannelSource> ChannelSourcePtr;
typedef ChannelSourceModel * ChannelSourceModelPtr;
typedef QList<ChannelSourcePtr> ChannelSourceList;

class RadioCategory;
class RadioCategoriesModel;
typedef QSharedPointer<RadioCategory> RadioCategoryPtr;
typedef RadioCategoriesModel * RadioCategoriesModelPtr;
typedef QList<RadioCategoryPtr> RadioCategoryList;
typedef QSet<uint> RadioCategoryIds;

}

}

#endif