import QtQuick 2.0
import QtQuick.Controls 1.0

Rectangle {
	radius: 3;
	border.color : '#888481'
	border.width : 1
	
	TextEdit {
		anchors {
			topMargin: 4
			leftMargin: 2
			top: parent.top
			left: parent.left
			right: searchIconSeparator.left
		}
		
		onTextChanged: {
			radioPageView.searchFilterChanged(text)
		}
		
		//autoScroll: false;
		selectByMouse: true;
		width: 10;
		text: "Поиск"
		cursorVisible: false
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
		anchors {  right: parent.right }
		smooth: true
		fillMode: Image.PreserveAspectFit
		source: "searchicon.png"
	}
/*
	color: Qt.rgb(136, 132, 129)
	

    TextField {
		anchors {
			topMargin: 2;
			bottomMargin: 2;
			leftMargin: 2;
			rightMargin: 2;
			fill: parent
		}
		
		//platformRightMargin: 10;
		
		placeholderText: "Поиск"
			
		Line {
			color: '#473f3c'
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
			anchors {  right: parent.right }
			smooth: true
			fillMode: Image.PreserveAspectFit
			source: "searchicon.png"
		}
    }
*/
}