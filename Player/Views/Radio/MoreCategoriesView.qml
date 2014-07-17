import QtQuick 2.0

Rectangle {
	property variant model;
	 
	Component  {
		id: moreCategoryDelegate
		
		Rectangle {
			width: textView.width;
			height: textView.height;
			
			StyledText {
				id: textView
				width: 96;
				text: Name
				elide: Text.ElideRight;
				font.pixelSize: 13;
				
				MouseArea  {
					anchors.fill: parent
					cursorShape: Qt.PointingHandCursor
					
					onClicked: {
						if(TopVisible)
						{
							radioPageView.setCategory(Id, true);
							radioPageView.showCurrentCategory();
						}
						else
						{
							var id = Id;
							radioPageView.setCategory(id, false);
							radioPageView.showCurrentCategory();
						}
					}
				}
			}
	
			Image {
				id: arrowImg
				anchors {
					left: textView.right;
					top: textView.top;
					leftMargin: 5;
					topMargin: 5;
				}
				
				source: "qrc:///mp/Resources/arrow.png"
			}

			Line {
				color: '#F8F8F8'
				anchors {
					left: textView.left
					right: textView.right
					top: textView.bottom
					topMargin: 5;
				}
				
				height: 2
				width: textView.width
			}
		}
	}
	
	GridView {
		id: moreCategoriesGrid

		anchors {
			top: parent.top;
			bottom: parent.bottom
			left: parent.left
			right: parent.right
			topMargin: 25
			leftMargin: 17
		}
		

		
		cellWidth: 136; cellHeight: 26
		
		Component.onCompleted: {
			console.log(parent.model);
			
		}
		
		model: parent.model
		delegate: moreCategoryDelegate
	}
	
	VerticalScrollBar {
		flickable: moreCategoriesGrid
	}
}