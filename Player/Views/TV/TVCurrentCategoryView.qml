import QtQuick 2.0

Rectangle {
	property int bannerId;
	property string bannerLogo;

	Image {
		id: bannerImg

		anchors {
			left: parent.left;
			top: parent.top;
			leftMargin: 12;
		}

		width: 545; 
		height: 174;
		
		source: bannerLogo
	}
	
	StyledText {
		id: showAllTxt;
		text: "Показать все";
		color: "#3987C5";
		font.underline: true;
		font.pixelSize: 13;
		font.bold: true;
		font.weight: Font.DemiBold
		anchors {
			left: parent.left;
			top: bannerImg.bottom;
			topMargin: 14;
			leftMargin: 12;

		}
		
		MouseArea  {
			anchors.fill: parent
			hoverEnabled: true
			cursorShape: Qt.PointingHandCursor
			onClicked: {
			}
		}
	}
	
	TVSourcesView {
		id: searchSourcesView;
		model: topTvSourcesModel;

		anchors {
			left: parent.left
			top: showAllTxt.bottom
			topMargin: 8;
		}
		
		interactive: false;
		width: parent.width; 
		height: 174;
	}
}