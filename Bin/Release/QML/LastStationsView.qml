import QtQuick 2.0

Rectangle {

	property variant internalModel;

	Text  {
		text: 'LAST' //currentChannel.Name
		color: '#ff7e43'
		font.pixelSize: 14
	}

	Component {
		id: channelDelegate
		Item {
			
			Rectangle { 
				width: parent.width; height: parent.height
				//color: '#ffffff'

				Row {
					
					Text  {
						id: stationText
						text: '  ' + Name + '  '
						color: '#a2a2a2'
						font.pixelSize: 12

						MouseArea  {
							anchors.fill: parent
							hoverEnabled: true
						}
					}
				}
			}

			//width: stationText.paintedWidth; 
			height: 30
		}
	}

	ListView {
		id: channelsList
		width:parent.width; height: parent.height
		anchors.fill: parent
		anchors.topMargin: 50
		delegate: channelDelegate
		model: internalModel
	}
}
