import QtQuick 2.0

Rectangle {
	
	property variant internalModel;

	Text  {
		text: 'TOP'
		color: '#ff7e43'
		font.pixelSize: 14
	}

	Component {
		id: channelDelegate
		Item {
			

			height: 40
			
			Text  {
				id: stationText
				font.family: openSansLight.name
				text: Name
				color: '#a2a2a2'
				font.pixelSize: 12

				MouseArea  {
					anchors.fill: parent
					hoverEnabled: true
				}
			}
		}
	}

	ListView {
		id: topChannelsList
		width:parent.width; height: 150
		anchors.fill: parent
		anchors.topMargin: 50
		model: parent.internalModel
		delegate: channelDelegate
		interactive: false
	}
}
