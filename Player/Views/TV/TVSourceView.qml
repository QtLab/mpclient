import QtQuick 2.0;

Rectangle {
	width: logoView.width;
	height: logoView.height + textView.height;
	
	Image {
		id: logoView

		anchors {
			left: parent.left;
			top: parent.top;
		}

		width: 95; 
		height: 123;
		
		source: Logo;
	}
	
	StyledText {
		id: textView;
		color: "#3987C5";
		font.bold: true;
		width: 95; 
		text: Name;
		elide: Text.ElideRight;
		font.pixelSize: 13;

		anchors {
			top: logoView.bottom;
			left: parent.left;
		}
	}
	
	MouseArea  {
		anchors.fill: parent
		cursorShape: Qt.PointingHandCursor
		onClicked: tvPage.gotoTVSource(Id);
	}
}
