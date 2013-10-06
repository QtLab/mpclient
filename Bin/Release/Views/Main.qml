 import QtQuick 1.0

 TabWidget {
     id: tabs
     width: 800; height: 600

     Rectangle {
         property string title: "Radio"
         color: "#e3e3e3"

         Rectangle {
			RadioPage{}
         }
     }

     Rectangle {
         property string title: "TV"
         anchors.fill: parent
         color: "#e3e3e3"

         Rectangle {
             anchors.fill: parent; anchors.margins: 20
             color: "#7fff7f"
             Text {
                 width: parent.width - 20
                 anchors.centerIn: parent; horizontalAlignment: Qt.AlignHCenter
                 text: "Flower stems are green"
                 font.pixelSize: 20
                 wrapMode: Text.WordWrap
             }
         }
     }

     Rectangle {
         property string title: "News"
         anchors.fill: parent; color: "#e3e3e3"

         Rectangle {
             anchors.fill: parent; anchors.margins: 20
             color: "#7f7fff"
             Text {
                 width: parent.width - 20
                 anchors.centerIn: parent; horizontalAlignment: Qt.AlignHCenter
                 text: "Violets are blue"
                 font.pixelSize: 20
                 wrapMode: Text.WordWrap
             }
         }
     }
 }