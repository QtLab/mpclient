import QtQuick 1.0

Rectangle {

	id: stationsView
	//width: parent.width
	//height: 40

	//color: '#ffffff'

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

			//width: stationText.paintedWidth; 
			height: 30
		}
	}

	ListView {
		id: channelsList
		width:parent.width; height: parent.height
		anchors.fill: parent
		model: radioChannels
		delegate: channelDelegate
	}

}
