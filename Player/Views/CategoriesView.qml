import QtQuick 2.0

Rectangle {
	width: parent.width
	height: 43
	color: '#362F2A'
	property variant model;
	
    Component {
        id: contactDelegate
		
        Item {
			property int categoryId: Id;
            height: 43; width: categoryText.paintedWidth + 20; 
			
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
			
			Line {
				color: '#473f3c'
				anchors.right: parent.right
				anchors.top: parent.top
				height: categoriesView.height
				width: 1
			}
        }
    }

    ListView {
		id: categoriesList;
		orientation: Qt.Horizontal
		width: 430;
		height: 43;
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

