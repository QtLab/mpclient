import QtQuick 2.0

Rectangle {
	property variant model;
	height: 85;
	
	Component {
		id: channelDelegate
		StationView {
			width : 130
		}
	}

	StyledText  {
		id: lastStationsText
		text: 'ПОСЛЕДНИЕ'
		color: '#FF702A'
		font.pixelSize: 13
		
		anchors {
			top: parent.top
			left: parent.left
		}
	}
	
	ListView {
		id: channelsList
		height: 70
		width: 130
		
		anchors {
			top: lastStationsText.bottom
			topMargin: 12
		}
		
		delegate: channelDelegate
		interactive: false
		clip: true;
		model: parent.model
	}
}
