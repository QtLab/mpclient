import QtQuick 2.0

Rectangle {
	property variant model;
	property bool interactive: true;
	
	Component  {
		id: tvSourceDelegate
		
		TVSourceView {
		}
	}
	

	GridView {
		id: currentGenreGrid

		anchors {
			top: parent.top;
			bottom: parent.bottom
			left: parent.left
			topMargin: 0
			leftMargin: 12
		}

		width: parent.width + 35;
		
		cellWidth: 150; cellHeight: 170
		interactive: parent.interactive;
		clip: true;
		Component.onCompleted: {
			console.log(parent.model);
		}
		
		model: parent.model
		delegate: tvSourceDelegate
	}

	VerticalScrollBar {
		anchors.rightMargin: 46;
		flickable: currentGenreGrid
	}
}