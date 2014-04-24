import QtQuick 2.2

Rectangle {

	Component {
		id: channelDelegate
		StationView {
			width : 200
		}
	}

	Text  {
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
		height: 50
		
		anchors {
			top: topStationsText.bottom
			left: topStationsText.left
			topMargin: 16
		}
		model: topStations
		delegate: channelDelegate
		interactive: false
	}
}
