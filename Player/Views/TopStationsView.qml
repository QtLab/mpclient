import QtQuick 1.0

Rectangle {

	Text  {
		text: 'TOP'
		color: '#ff7e43'
		font.pixelSize: 14
	}

	Component {
		id: channelDelegate
		Item {
			
			Rectangle { 
				width: parent.width; height: parent.height

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

			height: 30
		}
	}

	ListView {
		id: channelsList
		width:parent.width; height: 150
		anchors.fill: parent
		anchors.topMargin: 50
		model: radioChannels
		delegate: channelDelegate
	}
}