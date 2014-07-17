import QtQuick 2.0;
import QtGraphicalEffects 1.0

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
	
    DropShadow {

        horizontalOffset: 3
        verticalOffset: 3
        radius: 8.0
        samples: 16
        color: "#80000000"
        source: logoView
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
