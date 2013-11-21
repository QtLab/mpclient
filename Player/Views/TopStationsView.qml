import QtQuick 1.0


Rectangle {
	
	property variant internalModel;

		Text  {
			id: topHeader
			text: 'TOP' //currentChannel.Name
			color: '#ff7e43'
			font.pixelSize: 14
			height: 30
		}

	Rectangle {



		Column {
			Repeater {
				model: internalModel
				Text  {
					id: stationText
					text: '  ' + Name + '  '
					color: '#a2a2a2'
					font.pixelSize: 12
					height: 30

					MouseArea  {
						anchors.fill: parent
						hoverEnabled: true
					}
				}
			}
		}
	}
}

/*
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
		id: topChannelsList
		width:parent.width; height: 150
		anchors.fill: parent
		anchors.topMargin: 50
		model: parent.internalModel
		delegate: channelDelegate
		snapMode: ListView.NoSnap
	}
}
*/