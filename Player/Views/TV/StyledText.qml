import QtQuick 2.0
import QtQuick.Controls 1.0

Text {
	elide: Text.ElideRight;
	font.family: openSansLight.name
		
	Timer {
		id: toolTipTimer;
		interval: 500; running: false; repeat: false
		onTriggered: {
			if(mouseArea.containsMouse) {
				tvPage.showTooltip(parent.text);
			}
		}
	}
	
	MouseArea {
		id: mouseArea
		anchors.fill: parent
		hoverEnabled: true
		
		onEntered: {
			toolTipTimer.start();
		}
	}
}