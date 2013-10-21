import QtQuick 1.0

Rectangle {

		Text  {
			id: currentStationText
			text: 'adasdas' //currentChannel.Name
			color: '#7b7775'
			font.pixelSize: 12

			MouseArea  {
				anchors.fill: parent
				hoverEnabled: true
			}
		}

		Image 
		{
			id: currentChannelImage
			y: 20; anchors.horizontalCenter: parent.horizontalCenter
			source: currentChannel.Logo
		}
}
