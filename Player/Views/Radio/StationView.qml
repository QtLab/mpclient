import QtQuick 2.0

Item 
{
	height: 22
	
	StyledText  {
		id: stationText
		text: Name
		font.pixelSize: 12
		width: parent.width;
		
		MouseArea  {
			anchors.fill: parent
			hoverEnabled: true
			cursorShape: Qt.PointingHandCursor
			
			onClicked: {
				playStationView.stationName = Name;
				playStationView.isPlaying = true;
				radioPageView.playRadio(Id);
			}
		}
	}
}