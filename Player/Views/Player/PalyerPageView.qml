import QtQuick 2.0
import QtQuick.Controls 1.0

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
	signal tracksPathChangeRequest
	
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
			source: playerPage.isPlaying ? "qrc:///mp/Resources/pauseicon.png" :"qrc:///mp/Resources/playicon.png"
			
			anchors.topMargin: 10;
			anchors.leftMargin: 5;
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
			width: 18;
			height: 18;
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
			width: 18;
			height: 18;
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
			anchors.topMargin: 5;
			anchors.leftMargin: 10;
			width: 250
			color: '#a2a2a2'
			font.pixelSize: 12
		}
		
		Image {
			id: showDownloadTracks
			source: "qrc:///mp/Resources/w128h1281338911632musicfolder.png"
			anchors.top: parent.top
			anchors.left: trackName.right;
			anchors.topMargin: 5;
			
			MouseArea  {
				anchors.fill: parent
				cursorShape: Qt.PointingHandCursor
				onClicked: {
					if(tracksList.model == searchTracksModel) {
						tracksList.model = downloadedTracksModel;
					}
					else {
						tracksList.model = searchTracksModel;
					}
				}
			}
		}
		
		Image {
			id: changeTracksFolder
			source: "qrc:///mp/Resources/w128h1281338911586folder.png"
			anchors.top: parent.top
			anchors.left: showDownloadTracks.right;
			anchors.topMargin: 5;
			anchors.leftMargin: 10;
			
			MouseArea  {
				anchors.fill: parent
				cursorShape: Qt.PointingHandCursor
				onClicked: playerPage.tracksPathChangeRequest();
			}
		}
		
		Slider {
			id: positionSlider
			anchors.top: trackName.bottom
			anchors.left: trackName.left;
			anchors.topMargin: 5;
			width: 200
			height: 10;
			maximumValue: 1000
			minimumValue: 0
			value: 0
			stepSize: 0
			onValueChanged: {
				if(positionSlider.pressed) {
					playerPage.positionChanged(value);
				}
			}
		}
		
		StyledText  {
			id: timeLeft
			property bool showTimeLeft: true;
			anchors.top: trackName.bottom
			anchors.left: positionSlider.right;
			anchors.leftMargin: 10;
			width: 40
			text: "00:00"
			color: '#a2a2a2'
			font.pixelSize: 12
		}
		
		Slider {
			id: volumeSlider
			anchors.top: trackName.bottom
			anchors.left: timeLeft.right
			anchors.topMargin: 5;
			width: 50;
			height: 10;
			maximumValue: 1000
			minimumValue: 0
			value: 0
			stepSize: 1
			onValueChanged: {
				var volume = value / 1000;
				playerPage.volumeChanged(volume);
			}
		}
		
		SearchEdit {
			id: searchEdit
			anchors.top: parent.top;
			anchors.right: parent.right;
			anchors.topMargin: 10;
			anchors.rightMargin: 8;
			
			height: 24
			width: 160
		}
	}
	
	TrackList {
		id: tracksList
		width: parent.width
		height: parent.height - (playerContols.height + 15)
		model: searchTracksModel
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
	
	function updateTrackPosition(currentPos, trackLenght) {
		if(!positionSlider.pressed) {
			positionSlider.maximumValue = trackLenght;
			positionSlider.value = currentPos;
		}
		
		timeLeft.text = "-" + secondsToTime(trackLenght - currentPos);
		
		if(currentPos == trackLenght) {
			isPlaying = false;
			tracksList.gotoNextTrack();
		}
	}
	
	function setPlayingState(state) {
		isPlaying = state;
	}
	
	function setVolume(volume) {
		volumeSlider.value = volume * 1000;
	}
	
	function search(filter) {
		tracksList.model = searchTracksModel;
		searchFilterChanged(filter);
	}
}