import QtQuick 2.0
import QtQuick.Controls 1.1

Rectangle {
	color: 'red'
	property int currentStationId: 0;
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
		id: stationNameView
		text: parent.stationName
		width: centralIcon.width
		color: '#7A7673'
		font.pixelSize: 12

		anchors {
			bottomMargin: 18
			bottom: centralIcon.top
			left: centralIcon.left
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
						//radioPageView.updateStateionName
						radioPageView.playRadio(currentStationId);
					}
					
					isPlaying = !isPlaying;
				}
			}
		}
		
		Slider {
			id: volumeSlider
			y: 100; x: 40;
			width: 50;
			height: 10;
			maximumValue: 1000
			minimumValue: 0
			value: 50
			stepSize: 1
			onValueChanged: {
				var volume = value / 1000;
				radioPageView.volumeChanged(volume);
			}
		}
	}
	
	StyledText  {
		id: stationMetadataView
		text: parent.stationMetadata
		width: 220
		color: '#71C5EA'
		font.pixelSize: 12

		anchors {
			topMargin: 18
			top: centralIcon.bottom
			left: centralIcon.left
		}
	}
	
	onIsPlayingChanged: {
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