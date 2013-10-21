import QtQuick 1.0

Rectangle {
	id: radioPageViewId;
	width: 600
	height: 480
	color: '#ffffff'	

	//QML types also provide built-in property change 
	//signals that are emitted whenever a property value changes

	property string currentRadioId:""

	// signals
	signal playRadio(string id);
	signal pauseRadio();
	signal genreSelect(string id);

	LastStationsView {
		id: lastStationsView
		width: 100
		height: 120
		anchors.topMargin: 30
		anchors.leftMargin: 20
		anchors.left: parent.left
		anchors.top: genresView.bottom
	}

	TopStationsView {
		id: popularStationsView
		width: 100
		height: 120
		anchors.topMargin: 30
		anchors.leftMargin: 20
		anchors.left: parent.left
		anchors.top: lastStationsView.bottom
	}

	StationsView {
		width: 100
		height: parent.height - 150
		anchors.topMargin: 50
		anchors.right: parent.right
		anchors.top: genresView.bottom
	}

	GenresView {
		id: genresView
		anchors.topMargin: 5
	}
}