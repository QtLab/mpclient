import QtQuick 2.0

Rectangle {
	width: parent.width;
	height: 43;
	color: '#372F2C';
	property variant model;
	property int currentCategoryId: -1;
	
    Component {
        id: categoryDelegate
		
        Item {
			property int categoryId: Id;
            height: 43; width: categoryText.paintedWidth + 20; 
			visible: TopVisible;
			
			StyledText {
				id: categoryText;
				text: Name
				anchors.horizontalCenter: parent.horizontalCenter;
				color: categoriesList.currentIndex == index ?  'black' : 'white'
				font.pixelSize: 13;
				anchors {
					top: parent.top
					topMargin: 12;
				}
			}
			
			MouseArea  {
				anchors.fill: parent;
				hoverEnabled: true;
				cursorShape: Qt.PointingHandCursor;
				
				onClicked: {
					categoryText.forceActiveFocus()
					categoriesList.currentIndex = index;
				}
			}
        }
    }
	
    ListView {
		id: categoriesList;
		orientation: Qt.Horizontal
		
		height: parent.height;
		width: {
			categoriesList.model.UpdateTopVisibleCategories(parent.width - (parent.width - moreTextRect.x), openSansLight.name, 10, 21);
			return parent.width - (parent.width - moreTextRect.x);
		}
		
		anchors {
			top: parent.top;
			left: parent.left;
		}
		
        model: parent.model
        delegate: categoryDelegate
        focus: true;
		
		keyNavigationWraps: false;
		highlightMoveDuration: 150 ;
		
		interactive: false;
		clip: true;
		
		highlight: Rectangle { color: "white";}
		
		onCurrentItemChanged: {
			var currentCategoryId = -1;
			if(currentItem) {
				currentCategoryId = currentItem.categoryId;
			}
			
			parent.currentCategoryId = currentCategoryId;
			tvPage.categoryChanged(currentCategoryId);
		}
    }
	
    Connections {
        target: categoriesList.model
        onDataChanged: {
			updateTopVisibleCategories();
        }
    }
	
	Rectangle {
		id: moreTextRect;
		color: categoriesList.currentIndex == -1 ?  'white' : 'transparent';
		
		anchors {
			right: parent.right;
			top: parent.top;
			rightMargin: 1;
		}
		
		StyledText {
			id: moreText;
			text: "Еще"
			color: categoriesList.currentIndex == -1 ?  'black' : 'white'
			font.pixelSize: 13;
			
			anchors {
				right: parent.right;
				top: parent.top
				rightMargin: 10;
				leftMargin: 10;
				topMargin: 12;
			}
			
			MouseArea  {
				anchors.fill: parent
				hoverEnabled: true
				cursorShape: Qt.PointingHandCursor
				onClicked: {
					categoriesList.currentIndex = -1;
					tvPage.showMoreCategories();
				}
			}
		}
	}
	
	function setCategory(id) {		
		var rowIndex = model.RowIndexById(id);
		categoriesList.currentIndex = rowIndex;
	}
	
	function updateTopVisibleCategories() {
		categoriesList.model.UpdateTopVisibleCategories(parent.width - (parent.width - moreTextRect.x), openSansLight.name, 10, 21);
	}
	
	function insertLastTopVisibleCategory(id) {
		categoriesView.model.InsertLastTopVisibleCategory(id, parent.width - (parent.width - moreTextRect.x), openSansLight.name, 10, 21);
	}
	
}

