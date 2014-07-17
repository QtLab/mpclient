import QtQuick 2.0

Rectangle {
	property string currentGenreName;
	property int currentCategoryId;
	
	StyledText {
		id: genresText;
		text: "ЖАНРЫ";
		color: 'black'
		font.underline: true;
		font.pixelSize: 13;
		anchors {
			left: parent.left;
			top: parent.top;

		}
		
		MouseArea  {
			anchors.fill: parent
			hoverEnabled: true
			cursorShape: Qt.PointingHandCursor
			onClicked: {
					tvPage.showGenres(parent.currentCategoryId);
			}
		}
	}
		
	StyledText {
		id: currentGenre;
		color: '#FF702A'
		text: parent.currentGenreName;
		font.pixelSize: 13;
		anchors {
			leftMargin: 15;
			left: genresText.right;
			top: parent.top;

		}
	
	}
	SearchEdit {
		id: searchEdit
		anchors {
			top: parent.top;
			right: parent.right;
		}
		
		height: 24
		width: 116
	}
	
	Image {
		anchors {
			topMargin: 8
			left: parent.left;
			top: searchEdit.bottom;
		}
		
		source: "qrc:/mp/Resources/verticaldashedline.png"
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