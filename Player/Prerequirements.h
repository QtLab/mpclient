#ifndef MP_MEDIA_PREREQUIRMENTS
#define MP_MEDIA_PREREQUIRMENTS

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
class GenreItem;
class GenreModel;
class ChannelSource;
class ChannelSourceModel;
class FileToUpdate;
class UpdateModel;
class MainWindow;
class SystemTray;
class TVPageController;
class RadioPageController;
class UpdateController;
class RequestModel;
class DownlaodManager;
class UserIdle;
class WebView;

typedef TabPage * TabPagePtr;

typedef QSharedPointer<AudioStream> AudioStreamPtr;
typedef QSharedPointer<ChannelMetadata> ChannelMetadataPtr;

typedef QSharedPointer<ChannelSource> ChannelSourcePtr;
typedef ChannelSourceModel * ChannelSourceModelPtr;
typedef QList<ChannelSourcePtr> ChannelSourceList;

typedef QSharedPointer<GenreItem> GenreItemPtr;
typedef GenreModel * GenreModelPtr;
typedef QList<GenreItemPtr> GenreItemList;

typedef QSharedPointer<FileToUpdate> FileToUpdatePtr;
typedef UpdateModel * UpdateModelPtr;
typedef QList<FileToUpdatePtr> FileToUpdateList;

}

#endif