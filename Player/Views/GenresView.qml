import QtQuick 2.2
import QtQuick.Controls 1.1

Rectangle {

	width: parent.width
	height: 43
	color: '#362F2A'

	
	Component {
		id: genreDelegate
		Rectangle {
			
			id: wrapper
			width: genreInfo.paintedWidth + 10;
			height: genreInfo.height
			color: '#372f2b'
			
			//visible: index <=5
			
			Text {
				id: genreInfo
				anchors {
					horizontalCenter: wrapper.horizontalCenter;
					verticalCenter: parent.genresList
					leftMargin: 50;
				}

				font.family: openSansLight.name
				font.pixelSize: 16;
				text: Name
				color: genresList.currentIndex == index ?  'yellow' : 'white'
				
				MouseArea  {
					anchors.fill: parent
					hoverEnabled: true
					cursorShape: Qt.PointingHandCursor
					onClicked: {
						genresList.currentIndex = index;
						radioPageView.genreChanged(Id);
						
						toogleVisible();
					}
				}
			}
				
			Line {
				color: '#473f3c'
				anchors.right: parent.right
				anchors.top: parent.top
				width: 1
				height: parent.parent.height
			}
			
			ListView.onAdd: console.log(Name);
		}

	}
	
	ListView {
		id: genresList
		//width: parent.width; height: parent.height;
		snapMode: ListView.NoSnap
		interactive: false
		width: 250
		model: radioGenres
		delegate: genreDelegate
		orientation: Qt.Horizontal
		
		highlight: Rectangle { color: "lightsteelblue"; radius: 5 }
		focus: true
		highlightFollowsCurrentItem: true
	}
	

	SearchEdit {
		anchors {
			top: parent.top;
			right: parent.right;
			topMargin: 10;
			rightMargin: 8;
		}
		height: 24
		width: 116
	}
}
