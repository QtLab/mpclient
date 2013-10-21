import QtQuick 1.0

Rectangle {

	width: parent.width
	height: 50
	color: '#362F2A'

	Component {
		id: genreDelegate
		Item {
			
			Rectangle { 
				width: parent.width; height: parent.height
				color: '#372f2b'

				Column {
					
					Text  {
						id: genreText
						verticalAlignment: Text.AlignHCenter
						text: '  ' + Name + '  '
						color: 'white'
						font.pixelSize: 18
						
						MouseArea  {
							anchors.fill: parent
							hoverEnabled: true
							onClicked: console.log("area clicked")
							onDoubleClicked: console.log("area double clicked")
							onEntered: console.log("mouse entered the area")
							onExited: console.log("mouse left the area")
						}
					}

	
					Line {
						color: '#473f3c'
						anchors.right: parent.right
						anchors.top: parent.top
						width: 1
						height: parent.parent.height
					}
				}
			}

			width: genreText.paintedWidth; 
			height: 40
		}
	}

	ListView {
		id: genresList
		width:parent.width; height: parent.height
		anchors.fill: parent
		model: radioGenres
		delegate: genreDelegate
		orientation: Qt.Horizontal
	}

}
