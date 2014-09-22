import QtQuick 2.0

Rectangle {
	property int bannerId;
	property string bannerLogo;

	AnimatedImage {
		id: bannerImg;

		anchors {
			left: parent.left;
			top: parent.top;
			leftMargin: 12;
		}

		width: 545; 
		height: 174;
		
		source: bannerLogo;
		
		MouseArea  {
			anchors.fill: parent;
			hoverEnabled: true;
			cursorShape: Qt.PointingHandCursor;
			onClicked: tvPage.processBanner(bannerId);
		}
	}
	
	TVSourcesView {
		id: searchSourcesView;
		model: topTvSourcesModel;

		anchors {
			left: parent.left;
			top: bannerImg.bottom;
			topMargin: 28;
		}
		
		interactive: false;
		width: parent.width; 
		height: 174;
	}
}