import QtQuick 2.0

Rectangle {	
	width: parent.width; height: parent.height
	property variant model;
	
	Component  {
		id: channelDelegate
		StationView {
			width: 120;
		}
	}
	
	ListView {
		id: channelsList
		width:parent.width;
		
		anchors {
			fill: parent;
		}
		
		delegate: channelDelegate
		model: parent.model;
	}
}


			
