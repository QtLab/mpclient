 import QtQuick 1.1

Rectangle 
{
	anchors.fill: parent
	width: 800; height: 600
	color: "white"

	Component {
		id: highlight
		Rectangle {
			width: 180; height: 40
			color: "lightsteelblue"; radius: 5
			y: list.currentItem.y
			Behavior on y {
				SpringAnimation {
					spring: 3
					damping: 0.2
				}
			}
		}
	}

	ListView {
		id: list
		width: 180; height: 200
		model: radioChannels
		delegate: Text { text: Name }

		highlight: highlight
		highlightFollowsCurrentItem: false
		focus: true
	}

}