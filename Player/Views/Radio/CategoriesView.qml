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
            height: 43; width: categoryText.paintedWidth + 20; 
			visible: TopVisible;
			
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
        delegate: contactDelegate
		
		highlight: Rectangle { color: "white";}
        focus: true;
		
		keyNavigationWraps: false;
		highlightMoveDuration: 150 ;
		
		interactive: false;
		clip: true;
		
		onCurrentItemChanged: {
			if(currentItem) {
				radioPageView.categoryChanged(currentItem.categoryId);
			}
		}
    }
	
    Connections {
        target: categoriesList.model
        onDataChanged: {
			updateTopVisibleCategories();
        }
    }
	
	Rectangle {
		id: moreTextRect
		anchors {
			right: searchEdit.left;
			rightMargin: 10
			top: parent.top	
		}
		
		color: categoriesList.currentIndex == -1 ?  'white' : 'transparent'
		
		StyledText {
			id: moreText;
			text: "Еще"

			anchors {
				left: parent.left
				top: parent.top
				leftMargin: 10
				topMargin: 10	
			}
			
			color: categoriesList.currentIndex == -1 ?  'black' : 'white'
			font.pixelSize: 13;
		}
		
		MouseArea  {
			anchors.fill: parent
			hoverEnabled: true
			cursorShape: Qt.PointingHandCursor
			onClicked: {
				categoriesList.currentIndex = -1;
				radioPageView.showMoreCategories();
			}
		}
			
		width: moreText.width + moreText.anchors.leftMargin * 2
		height: categoriesList.height
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
	
	function updateTopVisibleCategories() {
		categoriesList.model.UpdateTopVisibleCategories(parent.width - (parent.width - moreTextRect.x), openSansLight.name, 10, 21);
	}
	
	function insertLastTopVisibleCategory(id) {
		categoriesView.model.InsertLastTopVisibleCategory(id, categoriesView.width - (categoriesView.width - moreTextRect.x), openSansLight.name, 10, 21);
	}
	
	function insertFirstTopVisibleCategory(id) {
		categoriesView.model.InsertFirstTopVisibleCategory(id, categoriesView.width - (categoriesView.width - moreTextRect.x), openSansLight.name, 10, 21);
	}
	
	function copyFromSearchEdit() {
		searchEdit.copyFromSearchEdit();
	}
	
	function cutFromSearchEdit() {
		searchEdit.cutFromSearchEdit();
	}
	
	function pasteToSearchEdit() {
		searchEdit.pasteToSearchEdit();
	}
}

