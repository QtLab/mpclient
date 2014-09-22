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
		id: tempText
		visible: false
		width: 95;
		wrapMode: Text.WordWrap
		
		function elideMultiline(text, font, linesCount) {
			tempText.font = font
			var l = text.length
			var s = "";
			for (var i = 0; i < linesCount; i++) {
				s += "W";
				if (i < linesCount - 1)
					s += "\n"
			}
			tempText.text = s
			var maxHeight = tempText.paintedHeight
			tempText.text = text
			while (tempText.paintedHeight > maxHeight) {
				tempText.text = text.substring(0, --l) + "..."
			}
			return tempText.text
		}
	}

	StyledText {
		id: textView;
		wrapMode: Text.WordWrap;
		horizontalAlignment: Text.AlignHCenter;
		color: "#3987C5";
		font.bold: true;
		width: 95; 

		font.pixelSize: 13;

		anchors {
			top: logoView.bottom;
			left: parent.left;
		}
		
		Component.onCompleted: {
			text = tempText.elideMultiline(Name, font, 2);
		}
	}
	
	MouseArea  {
		anchors.fill: parent
		cursorShape: Qt.PointingHandCursor
		onClicked: tvPage.gotoTVSource(Id);
	}
}
