import QtQuick 2.0

Rectangle {
	id: radioPageView;
	color: '#ffffff'
	FontLoader { id: openSansLight; source: "txdJ2vM9.ttf" }

	// signals
	signal genreChanged(int id)
	signal pauseRadio
	signal playRadio(int id)
	signal searchFilterChanged(string txt);
	
	GenresView {
		id: genresView;
		anchors {
			top: radioPageView.top
		}
	}
	
	Rectangle {
		anchors {
			left: parent.left
			right: parent.right
			top: genresView.bottom
			bottom: parent.bottom
		}
		
		Loader {
			id: background

			anchors.fill: parent
			source: "CurrentCategoryStationsView.qml"
		}
	}
	
	onSearchFilterChanged: {
		if(txt.length == 0)
		{
			hideSearchResults();
		}
		else
		{
			showSearchResults();
		}
	}
	
	function showSearchResults() {
		background.source = "SearchResultView.qml";
	}
	
	function hideSearchResults() {
		background.source = "CurrentCategoryStationsView.qml";
	}
	
	/*
	CurrentGenreStationsView {
		id: currentGenreStationsView;
		visible: true;
		anchors {
			left: parent.left
			right: parent.right
			top: genresView.bottom
			bottom: parent.bottom
		}
	}
	
	SearchResultView {
		id: searchResultView
		visible: false;
		anchors {
			left: parent.left
			right: parent.right
			top: genresView.bottom
			bottom: parent.bottom
		}
	}

	function toogleVisible() {
		if(currentGenreStationsView.visible)
		{
			currentGenreStationsView.visible = false;
			searchResultView.visible = true;
		}
		else
		{
			currentGenreStationsView.visible = true;
			searchResultView.visible = false;
		}
		
	}
	*/
	
	/*	

*/
	/*
	Rectangle {

		anchors {	
			left: parent.left
			right: parent.right
			top: genresView.bottom
			bottom: parent.bottom
		}
	}
	*/
	/*
	Rectangle {
		color: 'black'
		border.width :5;
		id: testView;
		
		anchors {
			fill: parent;
			top: genresView.bottom
		}
		
		Text {
			text: 'asasdasasdasdasd'
		}
	}
	*/
	

}