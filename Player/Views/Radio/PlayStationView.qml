import QtQuick 2.0
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.1

Rectangle {
	color: 'red'
	property bool isPlaying: false;
	property string stationName;
	property string stationMetadata;
	property real volume;
	
	Image 
	{
		id: leftDashedLine
		x: -130;
		y: -100;
		source: "qrc:///mp/Resources/dashedline.png"
	}
	
	Image 
	{
		id: rightDashedLine
		x: 130;
		y: -100;
		source: "qrc:///mp/Resources/dashedline.png"
	}
	
	StyledText  {
		text: parent.stationName
		width: centralIcon.width
		font.pixelSize: 12

		anchors {
			bottomMargin: 18
			bottom: centralIcon.top
			left: centralIcon.left
		}
		
		MouseArea  {
			anchors.fill: parent
			cursorShape: parent.text == null || parent.text.length == 0 ? Qt.ArrowCursor : Qt.PointingHandCursor
			onClicked:  {
				if(parent.text != null && parent.text.length > 0) {
					radioPageView.searchTracks(parent.text);
				}
			}
		}
	}
 
	Image 
	{
		id: centralIcon
		anchors {
			centerIn: parent;
			rightMargin: 40
		}
		
		source: "qrc:///mp/Resources/centralicon.png"
		
		Image 
		{
			y: 55; x: 55;
			id: playStateIcon
			source: "qrc:///mp/Resources/playicon.png"
			
			MouseArea  {
				anchors.fill: parent
				hoverEnabled: true
				cursorShape: Qt.PointingHandCursor
				onClicked:  {
					if(isPlaying)
					{
						radioPageView.pauseRadio();
					}
					else
					{
						radioPageView.resumeRadio();
					}
				}
			}
		}
		
		Slider {
			id: volumeSlider
			y: 100; x: 40;
			width: 50;
			height: 6;
			maximumValue: 1000
			minimumValue: 0
			value: 50
			stepSize: 1
			onValueChanged: {
				var volume = value / 1000;
				radioPageView.volumeChanged(volume);
			}
					
			style: SliderStyle {
				groove: Rectangle {
					implicitWidth: control.width
					implicitHeight: control.height
					color: "#120E0F"
					radius: 8
				}
				handle: Rectangle {
					anchors.centerIn: parent
					color: control.pressed ? "white" : "#E1E1E1"
					border.color: "gray";
					border.width: 0;
					implicitWidth: 22;
					implicitHeight: control.height;
					radius: 8
				}
			}
		}
	}
	
	StyledText  {
		text: parent.stationMetadata
		width: 220
		color: '#338CB2'
		font.pixelSize: 12

		anchors {
			topMargin: 18
			top: centralIcon.bottom
			left: centralIcon.left
		}
		
		MouseArea  {
			anchors.fill: parent
			cursorShape: parent.text == null || parent.text.length == 0 ? Qt.ArrowCursor : Qt.PointingHandCursor
			onClicked:  {
				if(parent.text != null && parent.text.length > 0) {
					radioPageView.searchTracks(parent.text);
				}
			}
		}
	}
	
	onIsPlayingChanged: {
		console.log("onIsPlayingChanged:", isPlaying);
		
		if(isPlaying)
		{
			playStateIcon.source = "qrc:///mp/Resources/pauseicon.png";
		}
		else
		{
			playStateIcon.source = "qrc:///mp/Resources/playicon.png";
		}
	}
	
	onVolumeChanged: {
		volumeSlider.value = volume * 1000;
	}
	
}