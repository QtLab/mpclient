import QtQuick 2.2

Rectangle {	
	width: parent.width; height: parent.height
	
	Component  {
		id: channelDelegate
		StationView {
			width: channelsList.width;
		}
	}
	
	ListView {
		id: channelsList
		width:parent.width;
		
		anchors {
			fill: parent;
		}
		
		delegate: channelDelegate
		model: allStations
	}
}


			
