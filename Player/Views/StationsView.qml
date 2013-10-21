import QtQuick 1.0
import Cursor 1.0

Rectangle {

CursorShapeArea {
	anchors.fill: parent
	cursorShape: Qt.PointingHandCursor

	id: stationsView
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
		width:parent.width; height: parent.height
		anchors.fill: parent
		model: radioChannels
		delegate: channelDelegate
	}

}
}
