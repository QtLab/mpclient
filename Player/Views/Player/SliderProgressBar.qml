import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.1

Rectangle {

	property alias progressBarStyle: progressBar.style;
	property alias maximumValue: progressBar.maximumValue;
	property alias minimumValue: progressBar.minimumValue;
	property alias value: progressBar.value;
	property int wheelStep: 0;
	property bool pressed: mouseArea.pressed;
	signal valueChangedByUser(int value);
	
	property Component defaultStyle: ProgressBarStyle {
		background: Rectangle {
			color: "#605855"
			border.color: "#372F2C"
			border.width: (control.height - 2) / 2;
			width: conrol.width;
			implicitHeight: control.height;
		}

		progress: Rectangle {
			color: "#F5F5F5"
			border.width: 0
			implicitHeight: 5
		}
	}
	
	property Component defaultBlue: ProgressBarStyle {
		background: Rectangle {
			color: "transparent"
			border.color: "transparent"
			border.width: (control.height - 2) / 2;
			width: conrol.width;
			implicitHeight: control.height;
		}

		progress: Rectangle {
			color: "#3B87C5"
			//border.color: "#82B2DA"
			border.width: 1
			implicitHeight: 5
		}
	}
	
	property Component divisionStyle: ProgressBarStyle {			
		background: Rectangle {
			color: "#605855"
			border.color: "#372F2C"
			border.width: 0;
			width: conrol.width;
			implicitHeight: control.height;
			
			Item {
				anchors.fill: parent
				clip: true
				Row {
					Repeater {
						Rectangle {
							color: index % 2 ? "#372F2C" : "#27221F" 
							width: index % 2 ? 1 : 3;
							height: control.height;
						}
						
						model: control.width / 2 + 3
					}
				}
			}
		}
		
		progress: Rectangle {
			border.color: "#372F2C"
			border.width: 0;
			
			Item {
				anchors.fill: parent
				clip: true
				Row {
					Repeater {
						Column {
							Rectangle { height: 1; color: index % 2 ? "#372F2C" : "#9D9998"; width: index % 2 ? 1 : 3;}
							Rectangle {
								color: index % 2 ? "#372F2C" : "#F9F9F9"
								width: index % 2 ? 1 : 3;
								height: control.height;
							}
							Rectangle { height: 1; color: index % 2 ? "#372F2C" : "#9D9998"; width: index % 2 ? 1 : 3;}
						}
						
						model: control.width / 2 + 3
					}
				}
			}
		}
	}
	
	ProgressBar {
		id: progressBar;
		anchors.bottom: parent.bottom;
		anchors.left: parent.left;
		width: parent.width;
		height: parent.height;
		value: 0
	}
	
	MouseArea {
		id: mouseArea; 
		anchors.fill: parent;
		hoverEnabled: true;
		cursorShape: Qt.PointingHandCursor
		
		onClicked: {
			updateValueWithMousePosition(mouse.x);
			parent.valueChangedByUser(progressBar.value);
		}
		
		onWheel : { //(WheelEvent wheel)
			if (parent.wheelStep > 0 && wheel.angleDelta.y > 0) {
				progressBar.value = progressBar.value  + wheelStep;
			}
			else {
				progressBar.value = progressBar.value - wheelStep;
			}
			
			parent.valueChangedByUser(progressBar.value);
		}
		
		onPositionChanged: {
			if(mouseArea.pressed) {
				updateValueWithMousePosition(mouse.x);
				parent.valueChangedByUser(progressBar.value);
			}
		}
		
		function updateValueWithMousePosition(mouseX) {
			progressBar.value = progressBar.minimumValue + (progressBar.maximumValue - progressBar.minimumValue) * mouseX / progressBar.width;
		}
	}
}