import QtQuick 2.0

Rectangle {

	StyledText { 
		id: searchText
		text: "Результаты поиска"
		font.pixelSize: 13;
	}
	
	Component  {
		id: searchResultDelegate
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
					hoverEnabled: true
					cursorShape: Qt.PointingHandCursor
					
					onClicked: {
						playStationView.stationName = Name;
						playStationView.stationMetadata = Url;
						playStationView.isPlaying = true;
						categoriesView.setCategory(FirstCategoryId);
						
						radioPageView.playRadio(Id);
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
		id: searchResultGrid

		anchors {
			top: searchText.bottom;
			bottom: parent.bottom
			left: parent.left
			right: parent.right
			topMargin: 25
			leftMargin: 17
		}
		
		cellWidth: 136; cellHeight: 26
		
		model: searchStationsModel
		delegate: searchResultDelegate
	}
	
	VerticalScrollBar {
		flickable: searchResultGrid
	}
}