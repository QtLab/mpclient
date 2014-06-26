import QtQuick 2.0
import QtQuick.Controls 1.0

Rectangle {
	radius: 3;
	border.color : '#888481'
	border.width : 1
	property string filter: "";
	
	TextField {
		id: searchField
		anchors {
			topMargin: 2
			leftMargin: 2
			top: parent.top
			left: parent.left
			right: searchIconSeparator.left
		}
		
		Keys.onReturnPressed: {
			playerPage.search(searchField.text)
		}
		
		font.pixelSize: 10
		font.family: openSansLight.name
		width: 10;
		placeholderText: "Поиск"
		text: parent.filter;
	}
	
	Line {
		id: searchIconSeparator
		color: '#888481'
		anchors {
			right: parent.right
			top : parent.top
			rightMargin: 30;
		}

		width: 1
		height: parent.height
	}
	
	Image {
		id: searchIcon
		anchors {
			topMargin: 2;
			top: parent.top
			right: parent.right 
		}
		smooth: true
		fillMode: Image.PreserveAspectFit
		source: "qrc:///mp/Resources/searchicon.png"
		
		MouseArea  {
			anchors.fill: parent
			hoverEnabled: true
			cursorShape: Qt.PointingHandCursor
			
			onClicked: {
				playerPage.search(searchField.text)
			}
		}
	}
}