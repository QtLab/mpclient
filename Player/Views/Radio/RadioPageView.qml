import QtQuick 2.0

Rectangle {
	id: radioPageView;
	color: '#ffffff'
	FontLoader { id: openSansLight; source: "qrc:///mp/Resources/txdJ2vM9.ttf" }

	// signals
	signal categoryChanged(int id);
	signal playRadio(int id);
	signal resumeRadio;
	signal pauseRadio;
	signal searchFilterChanged(string txt);
	signal searchTracks(string txt);
	signal volumeChanged(real volume);
	signal showSearchContextMenu(bool hasSelectedText);
	signal showTooltip(string text);
	
	CategoriesView {
		id: categoriesView;
		model: categoriesModel;
		anchors {
			top: radioPageView.top;
		}
	}
	
	Rectangle {
		id: currentCategoryStationsView;
		
		anchors {
			left: parent.left;
			right: parent.right;
			top: categoriesView.bottom;
			bottom: parent.bottom;
		}
		
		LastStationsView {
			id: lastStationsView;
			width: 130;
			model: lastStationsModel;
			
			anchors {
				leftMargin:20;
				topMargin: 15;
				left: parent.left;
				top: parent.top;
			}
		}
		
		TopStationsView {
			id: topStationsView;
			width: 130;
			model: topStationsModel;
			
			anchors {
				leftMargin: 20;
				bottomMargin: 20;
				left: parent.left;
				bottom: parent.bottom;
			}
		}
		
		PlayStationView {
			id: playStationView;
			stationName: stationName;
			stationMetadata: stationMetadata;
			anchors {
				centerIn: parent;
			}
		}
		
		StationsView {
			id: allStationsView;
			width: 130;
			height: parent.height;
			model: allStationsModel;
			anchors {
				topMargin: 15;
				right: parent.right;
				top: parent.top;
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
	
	MoreCategoriesView {
		id: moreCategoriesView;
		model: categoriesModel;
		
		anchors {
			left: parent.left;
			right: parent.right;
			top: categoriesView.bottom;
			bottom: parent.bottom;
		}
		
		visible: false;
	}

	onCategoryChanged: {
		showCurrentCategory();
	}
	
	onSearchFilterChanged: {
		if(txt.length == 0)
		{
			showCurrentCategory();
		}
		else
		{
			showSearchResults();
		}
	}
	
	onPlayRadio: {
		showCurrentCategory();
	}
	
	function setPlayStationName(name) {
		playStationView.stationName = name;
		console.log("playStationView.stationName", playStationView.stationName);
	}
	
	function setPlayingState(isPlaying) {
		playStationView.isPlaying = isPlaying;
		console.log("setPlayingState:", isPlaying);
	}
	
	function updateMetadata(metaData) {
		playStationView.stationMetadata = metaData;
	}
	
	function setVolume(value) {
		playStationView.volume = value;
	}
	
	function showSearchResults() {
		currentCategoryStationsView.visible = false;
		moreCategoriesView.visible = false;
		searchResultView.visible = true;
	}
	
	function showMoreCategories() {
		currentCategoryStationsView.visible = false;
		searchResultView.visible = false;
		moreCategoriesView.visible = true;
	}
	
	function showCurrentCategory() {
		currentCategoryStationsView.visible = true;
		searchResultView.visible = false;
		moreCategoriesView.visible = false;
	}

	function setCategory(id, topVisible) {
		if(!topVisible){
			categoriesView.insertLastTopVisibleCategory(id);
		}
		
		categoriesView.setCategory(id);
	}
	
	function insertLastTopVisibleCategory(id) {
		categoriesView.insertLastTopVisibleCategory(id);
	}
	
	function insertFirstTopVisibleCategory(id) {
		categoriesView.insertFirstTopVisibleCategory(id);
	}

	function updateTopVisibleCategories() {
		categoriesView.updateTopVisibleCategories();
	}
	
	function copyFromSearchEdit() {
		categoriesView.copyFromSearchEdit();
	}
	
	function cutFromSearchEdit() {
		categoriesView.cutFromSearchEdit();
	}
	
	function pasteToSearchEdit() {
		categoriesView.pasteToSearchEdit();
	}
}