import QtQuick 2.0

Rectangle {

	width: parent.width
	height: 43
	color: '#362F2A'

	Component {
		id: genreDelegate
		Rectangle {
			
			visible: parseInt(Id) < 5 
			
			id: wrapper
			width: genreInfo.paintedWidth + 10;
			height: genreInfo.height
			color: '#372f2b'
			Text {
				id: genreInfo
				anchors.horizontalCenter: wrapper.horizontalCenter
				anchors.top: wrapper.bottom
				anchors.topMargin: -16
				font.family: openSansLight.name
				font.pixelSize: 16;
				text: Name
				color: 'white'
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

	ListView {
		id: genresList
		width: parent.width; height: parent.height;
		snapMode: ListView.SnapOneItem
		interactive: false

		model: radioGenres
		delegate: genreDelegate
		orientation: Qt.Horizontal
	}
}
