import QtQuick 1.0

Rectangle {
	id: radioPageViewId;
	width: 600
	height: 480
	color: '#ffffff'	

	//QML types also provide built-in property change 
	//signals that are emitted whenever a property value changes

	property string currentRadioId:"64"
	property string currentGenreId:"22"
	property variant test:"22"

	signal playRadio

	// signals
	signal pauseRadio

	signal genreChanged

	LastStationsView {
		id: lastStationsView
		width: 100
		height: 120
		anchors.topMargin: 30
		anchors.leftMargin: 20
		anchors.left: parent.left
		anchors.top: genresView.bottom
		internalModel: lastStations
	}

	TopStationsView {
		id: popularStationsView
		width: 100
		height: 120
		anchors.topMargin: 30
		anchors.leftMargin: 20
		anchors.left: parent.left
		anchors.top: lastStationsView.bottom
		internalModel: topStations
	}

	StationsView {
		id: stationsView
		width: 100
		height: parent.height - 150
		anchors.topMargin: 50
		anchors.right: parent.right
		anchors.top: genresView.bottom
		internalModel: currentStations
	}

	GenresView {
		id: genresView
		anchors.topMargin: 5
	}
}