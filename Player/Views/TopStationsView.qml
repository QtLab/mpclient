import QtQuick 2.0

Rectangle {
	property variant model;
	
	Component {
		id: channelDelegate
		StationView {
			width : 130
		}
	}

	StyledText  {
		id: topStationsText
		text: 'ПОПУЛЯРНЫЕ'
		color: '#ff7e43'
		font.pixelSize: 13
		
		anchors {
			top: parent.top
			left: parent.left
		}
	}
	
	ListView {
		id: topChannelsList
		width:parent.width; 
		height: 35
		
		anchors {
			top: topStationsText.bottom
			left: topStationsText.left
			topMargin: 20
		}
		model: parent.model;
		delegate: channelDelegate;
		interactive: false;
	}
}
