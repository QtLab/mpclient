import QtQuick 2.0
import QtQuick.Controls 1.0

Text {
	elide: Text.ElideRight;
	font.family: openSansLight.name
		
	MouseArea {
		id: mouseArea
		anchors.fill: parent
	}
}