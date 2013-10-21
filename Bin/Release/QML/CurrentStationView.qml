Rectangle {

		Text  {
			id: lastStationsView
			text: 'asdadsads' //currentChannel.Name
			color: '#ff7e43'
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