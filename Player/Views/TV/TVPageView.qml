import QtQuick 2.0

Rectangle {
	id: tvPage;
	color: '#ffffff'
	FontLoader { id: openSansLight; source: "qrc:///mp/Resources/txdJ2vM9.ttf" }

	// signals
	signal showGenres(int id);
	signal categoryChanged(int id);
	signal searchFilterChanged(string txt);
	signal showSearchContextMenu(bool hasSelectedText);
	signal showTooltip(string text);
	signal gotoTVSource(int id);
	signal closeCurrentTVSource();
	signal processBanner(int id);
	
	TVCategoriesView {
		id: categoriesView;
		model: tvCategoriesModel;
		anchors {
			top: parent.top;
			left: parent.left;
		}
	}
	
	TVFilter {
		id: tvFilter;
		currentCategoryId: categoriesView.currentCategoryId;
		
		anchors {
			top: categoriesView.bottom;
			left: parent.left;
			right: parent.right;
			leftMargin: 12;
			rightMargin: 12;
			topMargin: 6;
		}
		
		height: 45;
	}
	
	MoreCategoriesView {
		id: moreCategoriesView;
		model: tvCategoriesModel;

		anchors {
			left: parent.left
			right: parent.right
			top: tvFilter.bottom
			bottom: parent.bottom
		}
		
		visible: false;
	}
	
	TVSourcesView {
		id: currentGenreSourcesView;
		model: tvSourcesCurrentGenreModel;
		
		anchors {
			left: parent.left
			right: parent.right
			top: tvFilter.bottom
			bottom: parent.bottom
		}
		
		visible: false;
	}
	
	TVSourcesView {
		id: searchSourcesView;
		model: tvSourcesSearchResultodel;
		
		anchors {
			left: parent.left
			right: parent.right
			top: tvFilter.bottom
			bottom: parent.bottom
		}
		
		visible: false;
	}
	
	TVCurrentCategoryView {
		id: currentCategoryView;
		
		anchors {
			left: parent.left
			right: parent.right
			top: tvFilter.bottom
			bottom: parent.bottom
		}
		
		visible: false;
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
	
	function setCurrentGenre(id, name) {
		tvPage.closeCurrentTVSource();
		tvFilter.currentGenreName = name;
		currentGenreSourcesView.visible = true;
		moreCategoriesView.visible = false;
		searchSourcesView.visible = false;
		currentCategoryView.visible = false;
	}
	
	function setCurrentCategory(id, topVisible, bannerId, bannerLogo) {
		tvPage.closeCurrentTVSource();
		
		if(!topVisible){
			categoriesView.insertLastTopVisibleCategory(id);
		}
		
		tvFilter.currentGenreName = "";
		currentGenreSourcesView.visible = false;
		moreCategoriesView.visible = false;
		searchSourcesView.visible = false;
		currentCategoryView.visible = true;
		
		currentCategoryView.bannerId = bannerId;
		currentCategoryView.bannerLogo = bannerLogo;
		
		categoriesView.setCategory(id);
	}
	
	function showMoreCategories() {
		tvPage.closeCurrentTVSource();
		
		tvFilter.currentGenreName = "";
		currentGenreSourcesView.visible = false;
		currentCategoryView.visible = false;
		searchSourcesView.visible = false;
		moreCategoriesView.visible = true;
	}
	
	function showSearchResults() {
		tvPage.closeCurrentTVSource();
		
		tvFilter.currentGenreName = "";
		currentGenreSourcesView.visible = false;
		currentCategoryView.visible = false;
		searchSourcesView.visible = false;
		searchSourcesView.visible = true;
	}

	
	function copyFromSearchEdit() {
		tvFilter.copyFromSearchEdit();
	}
	
	function cutFromSearchEdit() {
		tvFilter.cutFromSearchEdit();
	}
	
	function pasteToSearchEdit() {
		tvFilter.pasteToSearchEdit();
	}
	/*
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
				topMargin: 15
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
	
	MoreCategoriesView {
		id: moreCategoriesView;
		model: categoriesModel;
		
		anchors {
			left: parent.left
			right: parent.right
			top: categoriesView.bottom
			bottom: parent.bottom
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
	
	*/
}