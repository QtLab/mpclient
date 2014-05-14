import QtQuick 2.0

Rectangle {
	property variant model;

	Component  {
		id: channelDelegate
		StationView {
			width: 120;
		}
	}
	
	ListView {
		id: channelsList
		height: 195
		width: 130
		clip: false
		delegate: channelDelegate
		model: parent.model;
	}
}


			
