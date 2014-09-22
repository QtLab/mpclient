import QtQuick 2.0

Rectangle 
{
	width: parent.width;
	property int currentIndex: -1;
	property variant model: null;
	property alias header: tracksListView.header
	
	Component  {
		id: trackDelegate
		
		Rectangle {
			id: wrapper
			property int trackId: Id;
			property string trackName: Title;
			height: 45
			
			Rectangle {
				property bool containsMouse: false;
				
				id: trackSelection;
				height: parent.height;
				width: tracksListView.width;
				color: containsMouse ? "#F0F0F0" : tracksListView.currentIndex == index ? "#F2F7FB" : "#FFFFFF";
				
				Rectangle {
					anchors.top: parent.top;
					anchors.left: parent.left;
					height: 1;
					width: parent.width;
					color: "#E4E4E4";
				}
				
				Rectangle {
					width: parent.width - 28;
					anchors.top: parent.top;
					anchors.topMargin: 15;
					
					Image {
						id: playIcon;
						anchors.top: parent.top;
						anchors.left : parent.left;
						anchors.leftMargin: 23;
						source: tracksListView.currentIndex == index && playerPage.isPlaying ? 
																	"qrc:///mp/Resources/pl_trackpauseicon.png" :
																	"qrc:///mp/Resources/pl_trackplayicon.png";
					}
					
					StyledText  {
						id: trackDesk;
						text: Artist + " - " + Title;
						font.pixelSize: 13
						font.bold: true;
						width: parent.width;
						color: "black"
						anchors {
							top: parent.top
							left: playIcon.right;
							right: trackDuration.left;
							leftMargin: 10;
						}
					}
					
					StyledText {
						id: trackDuration;
						text: secondsToTime(Duration);
						anchors.top: parent.top
						anchors.right: parent.right
						font.pixelSize: 12
						visible: !Downloading && !trackSelection.containsMouse;
						color: "#888888"
					}
					
					AnimatedImage {
						id: downloadLnk
						anchors.top: parent.top
						anchors.right: parent.right
						anchors.leftMargin: 23
						visible: Downloading || trackSelection.containsMouse;
						source: {
							playing = true;
							var sou = Downloading ? "qrc:///mp/Resources/downloading.gif" : "qrc:///mp/Resources/download.png";
							return sou;
						}
					}
				}
				
				/*
				SliderProgressBar {
					id: positionSlider;
					anchors.bottom: parent.bottom;
					anchors.left: parent.left;
					maximumValue: 1000;
					minimumValue: 0;
					value: 0;
					width: 315
					height: 5;
					visible: tracksListView.currentIndex == index;
					progressBarStyle: positionSlider.defaultBlue;
				}
				*/
				
				MouseArea  {
					anchors.fill: parent;
					cursorShape: Qt.PointingHandCursor
					hoverEnabled: true;
					onEntered: trackSelection.containsMouse = true;
					onExited : trackSelection.containsMouse = false;
					onClicked: {
					
						if(!Downloading && coordsInElement(mouse.x, mouse.y, downloadLnk))
						{
							playerPage.downloadTrack(Id);
						}
						else
						{
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
			}
		}
	}
	
	function coordsInElement(x, y, elem) {
		if(!elem.visible) {
			return false;
		}
		
		if(x >= elem.x && x <= elem.x + elem.width
			&& y >= elem.y && y <= elem.x + elem.height)
		{
			return true;
		}
		
		return false;
	}
	
	function gotoNextTrack() {
		tracksListView.incrementCurrentIndex();
		playerPage.playNewTrack(tracksListView.currentItem.trackId, tracksListView.currentItem.trackName);
	}
	
	function gotoPrevTrack() {
		tracksListView.decrementCurrentIndex();
		playerPage.playNewTrack(tracksListView.currentItem.trackId, tracksListView.currentItem.trackName);
	}
	
	function secondsToTime(secs) {
		var t = new Date(1970,0,1);
		t.setSeconds(secs);
		var s = t.toTimeString().substr(0,8);
		if(secs > 86399)
			s = Math.floor((t - Date.parse("1/1/70")) / 3600000) + s.substr(2);
		if(s.substr(0, 2) == 00)
			return s.substr(3);
		return s;
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
	