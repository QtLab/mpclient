import QtQuick 2.0

Rectangle {

	property variant internalModel;
	
	//width: 180; height: 200
	
	width: parent.width; height: parent.height
	
	Component 
	{
		id: channelDelegate
		Item 
		{
			height: 40

			Text  {
				id: stationText
				text: ' ' + Name + '  '
				color: '#a2a2a2'
				font.pixelSize: 12
				
				MouseArea  {
					anchors.fill: parent
					hoverEnabled: true
					cursorShape: Qt.PointingHandCursor
				}
			}
		}
	}

	//Column {
		TextInput { 
			text: "Hello"; 
			font.weight: Font.DemiBold 
		}
		
		ListView {
			anchors.topMargin: 50
			id: channelsList
			width:parent.width; height: parent.height
			anchors.fill: parent
			delegate: channelDelegate
			model: internalModel
		}
	//}
}


			
