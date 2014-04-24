import QtQuick 2.2

Rectangle {

	property variant internalModel;
	height: 100

	Component {
		id: channelDelegate
		StationView {
			width : 200
		}
	}

	Text  {
		id: lastStationsText
		text: 'ПОСЛЕДНИЕ'
		color: '#ff7e43'
		font.pixelSize: 13
	}
	
	ListView {
		id: channelsList
		
		anchors {
			fill: parent
			topMargin: 30
		}
		
		interactive: false
		delegate: channelDelegate
		model: lastStations
		height: 75
	}
}
