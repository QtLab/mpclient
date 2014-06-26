import QtQuick 2.0

Rectangle 
{
	width: parent.width;
	property int currentIndex: -1;
	property variant model: null;
	
	Component  {
		id: trackDelegate
		
		Rectangle {
			id: wrapper
			property int trackId: Id;
			property string trackName: Title;
			
			height: 25
			
			Rectangle {
				id: trackSelection
				radius: 5;
				height: parent.height
				width: tracksListView.width - 35

				color: {
					if(tracksListView.currentIndex == index) return "#c4c4c3";
					if(mouseArea.containsMouse) return "#efefeb";
					return "#ffffff";
				}
				
				Rectangle {
					width: parent.width - 28
					anchors.top: parent.top
					anchors.topMargin: 4
					
					Image 
					{
						id: playIcon
						anchors.top: parent.top
						source: tracksListView.currentIndex == index && playerPage.isPlaying ? 
																	"qrc:///mp/Resources/pauseicon.png" :
																	"qrc:///mp/Resources/playicon.png"
						width: 16;
						height: 16;
					}
					
					StyledText  {
						text: Artist + " - " + Title
						color: '#a2a2a2'
						font.pixelSize: 12
						width: parent.width;
						
						anchors {
							top: parent.top
							left: playIcon.right;
							leftMargin: 10;
						}
					}
				}

				MouseArea  {
					id: mouseArea;
					hoverEnabled: true;
					anchors.fill: parent
					cursorShape: Qt.PointingHandCursor
					
					onClicked: {
						if(tracksListView.currentIndex != index) {
							tracksListView.currentIndex = index;
							playerPage.playNewTrack(Id, Title);
						}
						else {
							playerPage.tooglePlayingState();
						}
					}
				}
			}
			
			Image {
				id: addToFavorites
				anchors.top: parent.top
				anchors.left: trackSelection.right
				anchors.leftMargin: 3;
				anchors.topMargin: 3;
				visible: !Downloading
				
				source: {
					if(FileExists) {
						return "qrc:///mp/Resources/favoritesremove.png" ;
					}
					
					return "qrc:///mp/Resources/addtofavorits.png";
				}
				
				MouseArea  {
					hoverEnabled: true;
					anchors.fill: parent
					cursorShape: Qt.PointingHandCursor
					onClicked: {
						if(!FileExists) {
							playerPage.downloadTrack(Id);
						}
						else {
							playerPage.deleteTrack(Id);
						}
					}
				}
			}
			
			AnimatedImage {
				anchors.top: parent.top
				anchors.left: trackSelection.right
				anchors.leftMargin: 3;
				anchors.topMargin: 3;

				playing : true;
				visible: Downloading
				source: "qrc:///mp/Resources/ajax-loader.gif";
			}
		}
	}
	
	 function gotoNextTrack() {
		tracksListView.incrementCurrentIndex();
		playerPage.playNewTrack(tracksListView.currentItem.trackId, tracksListView.currentItem.trackName);
	}
	
	function gotoPrevTrack() {
		tracksListView.decrementCurrentIndex();
		playerPage.playNewTrack(tracksListView.currentItem.trackId, tracksListView.currentItem.trackName);
	}
	
	ListView {
		id: tracksListView;
		currentIndex: parent.currentIndex;

		anchors {
			top: parent.top;
			bottom: parent.bottom;
			left: parent.left;
			right: parent.right;
		}
		
		highlightMoveDuration: 150 ;
		width: parent.width;
		height: parent.width;
		
		clip: true
		delegate: trackDelegate
		maximumFlickVelocity: 900;
		
		model: parent.model;
		
		onCurrentItemChanged: {
		}
	}
	
	VerticalScrollBar 
	{
		flickable: tracksListView;
	}
}
	