import QtQuick 2.0
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.1

Rectangle {
	id: playerPage;
	color: '#ffffff'
	FontLoader { id: openSansLight; source: "qrc:///mp/Resources/txdJ2vM9.ttf" }

	// signals
	signal playTrack(int id)
	signal resumeTrack
	signal pauseTrack
	signal searchFilterChanged(string txt);
	signal volumeChanged(real volume);
	signal positionChanged(int volume);
	signal downloadTrack(int id);
	signal deleteTrack(int id);
	signal tracksPathChangeRequest;
	signal showSearchContextMenu(bool hasSelectedText);
	signal showTooltip(string text);
	
	// properties
	property bool isPlaying: false;		
	property int currentTrackId: 0;
	
	Rectangle {
		id: playerContols
		width: parent.width
		height: 43
		color: '#372F2C'
		anchors.top: parent.top;
		anchors.right: parent.right;
		
		Image {
			id: playIcon
			source: playerPage.isPlaying ? "qrc:///mp/Resources/pl_pauseicon.png" : "qrc:///mp/Resources/pl_playicon.png"
			
			anchors.topMargin: 15;
			anchors.leftMargin: 15;
			anchors.top: parent.top;
			anchors.left: parent.left
			
			MouseArea  {
				anchors.fill: parent
				cursorShape: Qt.PointingHandCursor
				onClicked: {
					playerPage.tooglePlayingState();
				}
			}
		}
		
		Image {
			id: playPrevIcon
			source: "qrc:///mp/Resources/playprevicon.png"
			anchors.topMargin: 14;
			anchors.leftMargin: 5;
			anchors.top: parent.top;
			anchors.left: playIcon.right
			
			MouseArea  {
				anchors.fill: parent
				cursorShape: Qt.PointingHandCursor
				onClicked: tracksList.gotoPrevTrack();
			}
		}
		
		Image {
			id: playNextIcon
			source: "qrc:///mp/Resources/playnexticon.png"
			anchors.topMargin: 14;
			anchors.leftMargin: 5;
			anchors.top: parent.top;
			anchors.left: playPrevIcon.right
			
			MouseArea  {
				anchors.fill: parent
				cursorShape: Qt.PointingHandCursor
				onClicked: tracksList.gotoNextTrack();
			}
		}

		StyledText {
			id: trackName
			anchors.top: parent.top
			anchors.left: playNextIcon.right;
			anchors.topMargin: 10;
			anchors.leftMargin: 10;
			width: 315
			color: '#a2a2a2'
			font.pixelSize: 12
		}
		
		SliderProgressBar {
			id: positionSlider;
			anchors.top: trackName.bottom;
			anchors.left: trackName.left;
			anchors.topMargin: 2;
			maximumValue: 1000;
			minimumValue: 0;
			value: 0;
			width: 315
			height: 5;
			progressBarStyle: positionSlider.defaultStyle;
			
			onValueChangedByUser: {
				playerPage.positionChanged(value);
			}
		}
		
		Image {
			id: muteIcon
			source: "qrc:///mp/Resources/volumeicon.png";
			anchors.topMargin: 15;
			anchors.leftMargin: 15;
			anchors.top: parent.top;
			anchors.left: positionSlider.right
		}

		SliderProgressBar {
			id: volumeSlider;
			anchors.top: muteIcon.top;
			anchors.left: muteIcon.right;
			anchors.leftMargin: 6;
			anchors.topMargin: 4;
			width: 40
			height: 11;
			maximumValue: 100
			minimumValue: 0
			wheelStep: 10;
			progressBarStyle: positionSlider.divisionStyle;
			
			onValueChangedByUser: {
				var volume = value / maximumValue;
				playerPage.volumeChanged(volume);
			}
		}		
	}
	
	TrackList {
		id: tracksList

		height: parent.height - (playerContols.height + 15)
		model: searchTracksModel
		width: 544;
		
		property string searchLine: "";
		
		header: Rectangle {
			id: trackListHeader;
			anchors.top: parent.top;
			anchors.left: parent.left;
			height: 50;
			
			SearchEdit {
				id: searchEdit
				anchors.top: parent.top;
				anchors.left: parent.left;
				anchors.topMargin: 10;
				filter: tracksList.searchLine;
				height: 30
				width: 440
			}
		}
		
		anchors {
			top: playerContols.bottom;
			left: parent.left;
			right: parent.right;
			topMargin: 15;
			rightMargin: 5;
			leftMargin: 10;
		}
	}
	
	function playNewTrack(id, title) {
		currentTrackId = id;
		trackName.text = title;
		playerPage.playTrack(id);
	}
	
	function tooglePlayingState() {
		if(currentTrackId == 0) {
			tracksList.gotoNextTrack();
		}
		
		if(playerPage.isPlaying) {
			playerPage.pauseTrack();
		}
		else {
			playerPage.resumeTrack();
		}
	}

	function updateTrackPosition(currentPos, trackLenght) {
		if(!positionSlider.pressed) {
			positionSlider.maximumValue = trackLenght;
			positionSlider.value = currentPos;
		}

		if(currentPos == trackLenght) {
			isPlaying = false;
			tracksList.gotoNextTrack();
		}
	}
	
	function setPlayingState(state) {
		isPlaying = state;
	}
	
	function setVolume(volume) {
		volumeSlider.value = volume * volumeSlider.maximumValue;
	}
	
	function setSearchFilter(filter) {
		tracksList.searchLine = filter;
	}
	
	function search(filter) {
		tracksList.model = searchTracksModel;
		searchFilterChanged(filter);
	}
	
	function copyFromSearchEdit() {
		searchEdit.copyFromSearchEdit();
	}
	
	function cutFromSearchEdit() {
		searchEdit.cutFromSearchEdit();
	}
	
	function pasteToSearchEdit() {
		searchEdit.pasteToSearchEdit();
	}
}