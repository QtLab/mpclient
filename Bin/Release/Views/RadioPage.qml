 import QtQuick 1.1

 Rectangle 
 {
	anchors.fill: parent
    color: "gray"

	Component 
	{
		id: appDelegate
		Item 
		{
			width: 170; height: 170
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
				onClicked: {
					MediaController.Pause();
					gridView.currentIndex = index;
					MediaController.SetUrl(Url);
					MediaController.Play();
				}
			}
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
		cellWidth: 170; cellHeight: 170
		highlight: appHighlight
		focus: true
		model: radioChannels
		delegate: appDelegate
	}

 }