import QtQuick 2.0
import QtQuick.Controls 1.0
import QtQuick.Controls.Styles 1.0

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
		
		font.pixelSize: 11
		font.family: openSansLight.name
		width: 10;
		height: 25;
		placeholderText: focus? "" : "Поиск"
		text: parent.filter;
		
        style: TextFieldStyle {
            textColor: "black"
            background: Rectangle {
                border.color: "#333"
                border.width: 0
            }
        }
		
		 MouseArea {
			 anchors.fill: parent
			 acceptedButtons: Qt.RightButton
			 onClicked: {
				playerPage.showSearchContextMenu(parent.selectedText.length > 0);
			 }
		 }
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
			topMargin: 4;
			rightMargin: 1;
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
	
	function copyFromSearchEdit() {
		searchField.copy();
	}
	
	function cutFromSearchEdit() {
		searchField.cut();
	}
	
	function pasteToSearchEdit() {
		searchField.paste();
	}
}