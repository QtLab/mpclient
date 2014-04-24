import QtQuick 2.0

Rectangle {
	property string stationName;
	property string stationMetadata
	
	Text  {
		id: stationNameTxt
		text: parent.stationName
		
		color: '#7A7673'
		font.pixelSize: 12

		anchors {
			bottom: logoImg.top
			left: logoImg.left
		}
	}

	Image 
	{
		id: logoImg
		anchors {
			centerIn: parent;
			rightMargin: 40
		}
		source: "centralimg.png"
	}
	
	Text  {
		id: stationMetadata
		text: parent.stationMetadata
		
		color: '#71C5EA'
		font.pixelSize: 12

		anchors {
			top: logoImg.bottom
			left: logoImg.left
		}
	}
}