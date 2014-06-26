import QtQuick 2.0;

Item {
    id: scrollbar;
	height : 5;
	width: flickable.width
	
    visible: (flickable.visibleArea.widthRatio < 1.0);
    anchors {
		left: flickable.left;
        //top: flickable.top;
        right: flickable.right;
        bottom: flickable.bottom;
    }

    property Flickable flickable               : null;
    property int       handleSize              : 5;


    Binding {
        target: flickable;
        property: "contentX";
        value: (handle.x * (flickable.contentWidth- flickable.width) / clicker.drag.maximumX);
        when: (clicker.drag.active || clicker.pressed);
    }

   Binding {
        target: handle;
        property: "x";
        value: (flickable.contentX * clicker.drag.maximumX / (flickable.contentWidth - flickable.width));
        when: (!clicker.drag.active);
    }
	
    Rectangle {
        id: backScrollbar;
		color: '#FFFFFF'
        anchors { fill: parent; }
        MouseArea {
            anchors.fill: parent;
            cursorShape: Qt.PointingHandCursor
        }
    }

    Item {
        id: groove;
        clip: true;
		
        anchors {
            fill: parent;
        }

        MouseArea {
            id: clicker;
            drag {
                target: handle;
                minimumX: 0;
                maximumX: (scrollbar.width - handle.width);
                axis: Drag.XAxis;
            }
			cursorShape: Qt.PointingHandCursor
            anchors { fill: parent; }
            onClicked: { flickable.contentX = (mouse.x / scrollbar.width * (flickable.contentWidth - flickable.width)); }
        }
		
        Item {
            id: handle;
            width: Math.max (20, (flickable.visibleArea.widthRatio * groove.width));
            anchors {
				top: parent.top;
				bottom: parent.bottom;
            }

            Rectangle {
                id: backHandle;
                color: '#372F2C'
                anchors { fill: parent; }
            }
        }
    }
}
