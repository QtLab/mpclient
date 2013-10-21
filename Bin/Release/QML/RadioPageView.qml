import QtQuick 1.0

Rectangle {
	width: 600
	height: 480
	color: '#ffffff'	

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


 /*
Item {
    id: root

    // default size, but scalable by user
    height: 300; width: 400

    Rectangle {
        id: leftPanel

        anchors {
            top: parent.top
            left: parent.left
            bottom: parent.bottom
        }
        width: root.width * 0.3
        color: "blue"
    }

    Rectangle {
        id: topPanel

        anchors {
            top: parent.top
            left: leftPanel.right
            right: parent.right
        }
        height: 40
        color: "green"
    }


    Rectangle {
        id: contentArea

        anchors {
            top: topPanel.bottom
            left: leftPanel.right
            right: parent.right
            bottom: root.bottom
        }
        color: "white"

        Text {
            text: "Hi, I'm scalable!"
            anchors.centerIn: parent
            font.pixelSize: root.width * 0.05
        }
    }
}
*/