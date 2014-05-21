import QtQuick 2.0

Item 
{
	height: 22
	
	StyledText  {
		id: stationText
		text: Name
		color: '#a2a2a2'
		font.pixelSize: 12
		width: parent.width;
		
		MouseArea  {
			anchors.fill: parent
			hoverEnabled: true
			cursorShape: Qt.PointingHandCursor
			
			onClicked: {
				playStationView.stationName = Name;
				playStationView.stationMetadata = Url;
				playStationView.currentStationId = Id;
				playStationView.isPlaying = true;
				radioPageView.playRadio(Id);
			}
		}
	}
}