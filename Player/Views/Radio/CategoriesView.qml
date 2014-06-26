import QtQuick 2.0

Rectangle {
	width: parent.width
	height: 43
	color: '#372F2C'
	property variant model;
	
    Component {
        id: contactDelegate
		
        Item {
			property int categoryId: Id;
            height: categoriesList.width; width: categoryText.paintedWidth + 20; 
			
			StyledText {
				id: categoryText;
				text: Name
				anchors.horizontalCenter: parent.horizontalCenter
				color: categoriesList.currentIndex == index ?  'black' : 'white'
				font.pixelSize: 13;
				anchors {
					top: parent.top
					topMargin: 10;
				}
				
				MouseArea  {
					anchors.fill: parent
					hoverEnabled: true
					cursorShape: Qt.PointingHandCursor
					onClicked: {
						categoryText.forceActiveFocus()
						categoriesList.currentIndex = index;
					}
				}
			}
        }
    }
	
    ListView {
		id: categoriesList;
		orientation: Qt.Horizontal
		
		height: parent.height;
		
		anchors {
			top: parent.top;
			left: leftArrowLine.right;
			right: rightArrowLine.left
		}
		
        model: parent.model
        delegate: contactDelegate
        
		highlight: Rectangle { color: "white";}
        focus: true;
		
		keyNavigationWraps: false;
		highlightMoveDuration: 150 ;
		
		clip: true;
		
		onCurrentItemChanged: {
			radioPageView.categoryChanged(currentItem.categoryId);
		}
    }
	
	SearchEdit {
		id: searchEdit
		
		anchors {
			top: parent.top;
			right: parent.right;
			topMargin: 10;
			rightMargin: 8;
		}
		
		height: 24
		width: 116
	}
	
	function setCategory(id) {		
		var rowIndex = model.RowIndexById(id);
		categoriesList.currentIndex = rowIndex;
	}
	
	// Arrows
	Rectangle {
		id: leftArrow
		color: "#372F2C"
		width: 10
		height: parent.height;
		
		anchors.top: parent.top;
		anchors.left: parent.left;
		
		Image 
		{
			anchors.top: parent.top
			anchors.topMargin: 15		
			source: "qrc:///mp/Resources/arrow_left.png"
		}
			
		MouseArea {
			Timer {
				id: leftArrowTimer
				interval: 50; running: false; repeat: true
				onTriggered: {
					if(!categoriesList.atXBeginning)
						categoriesList.flick(2000, 0);
				}
			}
			anchors.fill: parent
			cursorShape: Qt.PointingHandCursor
			onPressed: leftArrowTimer.start();
			onReleased: leftArrowTimer.stop();
		}
	}
	
	Line {
		id: leftArrowLine
		color: '#473f3c'
		anchors.leftMargin: 2;
		anchors.top: parent.top;
		anchors.left: leftArrow.right;
		height: categoriesView.height
		width: 1
	}
		
	Rectangle {
		id: rightArrow
		color: "#372F2C"
		width: 10
		height: parent.height;
		anchors.top: parent.top;
		anchors.right: searchEdit.left;
		
		Image 
		{
			anchors.top: parent.top
			anchors.topMargin: 15
			source: "qrc:///mp/Resources/arrow_right.png"
		}
		
		MouseArea {
			Timer {
				id: rightArrowTimer
				interval: 50; running: false; repeat: true;
				onTriggered: {
					if(!categoriesList.atXEnd)
						categoriesList.flick(-2000, 0);
				}	
			}
			anchors.fill: parent
			cursorShape: Qt.PointingHandCursor
			onPressed: rightArrowTimer.start();
			onReleased: rightArrowTimer.stop();
		}
	}
	
	Line {
		id: rightArrowLine
		color: '#473f3c'
		anchors.rightMargin: 4
		anchors.top: parent.top;
		anchors.right: rightArrow.left;
		height: categoriesView.height
		width: 1
	}
}

