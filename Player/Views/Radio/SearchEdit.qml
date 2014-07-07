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
			radioPageView.searchFilterChanged(searchField.text)
		}
		
		onTextChanged: {
			radioPageView.searchFilterChanged(searchField.text)
		}
		
		 MouseArea {
			 anchors.fill: parent
			 acceptedButtons: Qt.RightButton
			 onClicked: {
				var globalPos = mapToItem(null, mouseX, mouseY);
				radioPageView.showSearchContextMenu(globalPos.x, globalPos.y);
			 }
		 }
	 
		font.pixelSize: 10
		font.family: openSansLight.name
		width: 10;
		text: parent.filter;
		placeholderText: "Поиск"
		
        style: TextFieldStyle {
            textColor: "black"
            background: Rectangle {
                border.color: "#333"
                border.width: 0
            }
        }
	}
	
	/*
	MouseArea  {
		anchors.fill: parent
		hoverEnabled: true			
		onClicked: {
			for (var idx in searchField.children) {
				var child = searchField.children[idx];
				if(child.clicked) {
					child.clicked(mouse);
					break;
				}
			}	
		}
	}
*/

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
			topMargin: 1;
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
				radioPageView.searchFilterChanged(searchField.text)
			}
		}
	}
}