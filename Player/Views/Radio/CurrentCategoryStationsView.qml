import QtQuick 2.0

Rectangle {	
	LastStationsView {
		id: lastStationsView
		anchors {
			leftMargin:21
			topMargin: 20
			left: parent.left
			top: parent.top
		}
	}
	
	TopStationsView {
		id: topStationsView
		width: lastStationsView.width
		anchors {
			topMargin: 10
			leftMargin: 20
			left: parent.left
			top: lastStationsView.bottom
		}
	}
	
	StationsView {
		id: allStationsView
		anchors {
			topMargin: 30
			right: parent.right
			top: parent.top
		}
	}

	PlayStationView {
		id: playStationView
		stationName: stationName
		stationMetadata: stationMetadata
		anchors {
			centerIn: parent;
		}
	}
}