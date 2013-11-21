import QtQuick 1.0
import Cursor 1.0

Rectangle {

	property variant internalModel;

//CursorShapeArea {
	//anchors.fill: parent
	//cursorShape: Qt.PointingHandCursor

	Component {
		id: channelDelegate
		Item {
			

			Rectangle { 
				width: parent.width; height: parent.height

				Row {

					//MouseArea  {
						//anchors.fill: parent
						//hoverEnabled: true

						Text  {
							id: stationText
							text: ' ' + Name + '  '
							color: '#a2a2a2'
							font.pixelSize: 12
						}

						MouseArea  {

							onClicked: {
								console.log("onClicked");
								radioPageViewId.currentRadioId = Id;
								
								RadioPage.PlayCurrentRadio();
							} 

							anchors.fill: parent
							hoverEnabled: true
 						}

					//}
				}
			}
			 
			height: 30
		}
	}

	ListView {
		id: channelsList
		width:parent.width; height: parent.height
		anchors.fill: parent
		delegate: channelDelegate
		model: currentStations
	}

//}
}
