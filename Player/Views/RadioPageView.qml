import QtQuick 2.0

Rectangle {
	id: radioPageView;
	color: '#ffffff'
	FontLoader { id: openSansLight; source: "qrc:///mp/Resources/txdJ2vM9.ttf" }

	// signals
	signal categoryChanged(int id)
	signal playRadio(int id)
	signal pauseRadio
	signal searchFilterChanged(string txt);
	signal volumeChanged(real volume);
	
	CategoriesView {
		id: categoriesView;
		model: categoriesModel;
		anchors {
			top: radioPageView.top
		}
	}
	
	Rectangle {
		id: currentCategoryStationsView;
		
		anchors {
			left: parent.left
			right: parent.right
			top: categoriesView.bottom
			bottom: parent.bottom
		}
		
		LastStationsView {
			id: lastStationsView
			width: 130
			model: lastStationsModel;
			
			anchors {
				leftMargin:20
				topMargin: 15
				left: parent.left
				top: parent.top
			}
		}
		
		TopStationsView {
			id: topStationsView
			width: 130
			model: topStationsModel
			
			anchors {
				leftMargin: 20
				bottomMargin: 20
				left: parent.left
				bottom: parent.bottom
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
		
		StationsView {
			id: allStationsView
			width: 130
			height: parent.height
			model: allStationsModel
			anchors {
				topMargin: 30
				right: parent.right
				top: parent.top
			}
		}
	}

	SearchResultView {
		id: searchResultView;
		
		anchors {
			left: parent.left
			right: parent.right
			top: categoriesView.bottom
			bottom: parent.bottom
		}
		
		visible: false;
	}
	
	onCategoryChanged: {
		hideSearchResults();
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
	
	onPlayRadio: {
		hideSearchResults();
	}
	
	function setPlayingState(isPlaying) {
		playStationView.isPlaying = isPlaying;
	}
	
	function updateMetadata(metaData) {
		playStationView.stationMetadata = metaData;
	}
	
	function setVolume(value) {
		playStationView.volume = value;
	}
	
	function showSearchResults() {
		currentCategoryStationsView.visible = false;
		searchResultView.visible = true;
	}
	
	function hideSearchResults() {
		currentCategoryStationsView.visible = true;
		searchResultView.visible = false;
	}

	

}