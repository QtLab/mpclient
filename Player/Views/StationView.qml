import QtQuick 2.2

Item 
{
	height: 25
	
	Text  {
		id: stationText
		text: Name
		color: '#a2a2a2'
		font.pixelSize: 12
		elide: Text.ElideRight;
		width: parent.width;
		MouseArea  {
			anchors.fill: parent
			hoverEnabled: true
			cursorShape: Qt.PointingHandCursor
		}
	}
}