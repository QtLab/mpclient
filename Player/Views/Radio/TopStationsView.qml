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
		id: topStationsText
		text: 'ПОПУЛЯРНЫЕ'
		color: '#FF702A'
		font.pixelSize: 13
		
		anchors {
			top: parent.top
			left: parent.left
		}
	}
	
	ListView {
		id: topChannelsList
		width:parent.width; 
		height: 70;
		
		model: parent.model;
		delegate: channelDelegate;
		interactive: false;
		clip: true;
		anchors {
			top: parent.top
			topMargin: 30
			left: topStationsText.left
		}
	}
}
