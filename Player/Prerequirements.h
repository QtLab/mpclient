#ifndef MP_MEDIA_PREREQUIRMENTS
#define MP_MEDIA_PREREQUIRMENTS

#include "Common.h"
#include <QSharedPointer>
#include <QList>

QT_BEGIN_NAMESPACE
class QNetworkAccessManager;
class QNetworkReply;
class QNetworkRequest;
class QQuickView;
QT_END_NAMESPACE

namespace mp {

// Forward declaration
class ChannelMetadata;
class AudioStream;
class TabPage;
class RadioPage;
class TVPage;
class Category;
class CategoriesModel;
class ChannelSource;
class ChannelSourceModel;
class FileToUpdate;
class UpdateModel;
class MainWindow;
class SystemTray;
class TVPageController;
class RadioPageController;
class UpdateController;
class MPRequest;
class DownlaodManager;
class UserIdle;
class WebView;
class ChannelSourceSortFilterProxyModel;

typedef TabPage * TabPagePtr;

typedef QSharedPointer<AudioStream> AudioStreamPtr;
typedef QSharedPointer<ChannelMetadata> ChannelMetadataPtr;

typedef QSharedPointer<ChannelSource> ChannelSourcePtr;
typedef ChannelSourceModel * ChannelSourceModelPtr;
typedef QList<ChannelSourcePtr> ChannelSourceList;

typedef QSharedPointer<Category> CategoryPtr;
typedef CategoriesModel * CategoriesModelPtr;
typedef QList<CategoryPtr> CategoryList;

typedef QSharedPointer<FileToUpdate> FileToUpdatePtr;
typedef UpdateModel * UpdateModelPtr;
typedef QList<FileToUpdatePtr> FileToUpdateList;

}

#endif