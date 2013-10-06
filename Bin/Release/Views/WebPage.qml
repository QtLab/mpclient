 import QtQuick 1.1

 Rectangle 
 {
     width: 800; height: 600
    color: "gray"

	Component 
	{
		id: appDelegate

		Item 
		{
			width: 200; height: 200

			//Scale {
				Image 
				{
					id: myIcon
					y: 20; anchors.horizontalCenter: parent.horizontalCenter
					source: Logo
				}
				Text 
				{
					anchors { top: myIcon.bottom; horizontalCenter: parent.horizontalCenter }
					text: Name
				}

				MouseArea 
				{
					anchors.fill: parent
					onClicked: gridView.currentIndex = index
				}
			//}

		}
	}

	Component 
	{
		id: appHighlight
		Rectangle { width: 80; height: 80; color: "lightsteelblue" }
	}

	GridView 
	{
		id: gridView
		anchors.fill: parent
		cellWidth: 200; cellHeight: 200
		highlight: appHighlight
		focus: true
		model: radioChannels
		delegate: appDelegate
	}

 }