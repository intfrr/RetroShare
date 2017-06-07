import QtQuick 2.7
import QtQuick.Layouts 1.2
import QtQuick.Controls 2.0


Item {

	id: chatBubbleDelegate
	height: bubble.height
	width: parent.width

	property int lMarginBubble: 10
	property int rMarginBubble: 10
	property int tMarginBubble: 5
	property int bMarginBubble: 10
	property int aditionalBubbleHeight: tMarginBubble * 2
	property int aditionalBubbleWidth: 30


	Rectangle {
		anchors.fill: parent
		width: parent.width
		height: parent.height

		Rectangle
		{
			id: bubble
			width: Math.max(mesageText.implicitWidth, sendersName.implicitWidth ) + timeText.implicitWidth + aditionalBubbleWidth
//			height: mesageText.height + mesageText.anchors.margins + 10
			height: mesageText.height + sendersName.height  + aditionalBubbleHeight


			anchors.left: (model.incoming)?  parent.left : undefined
			anchors.right: (!model.incoming)?  parent.right : undefined


			color: (!model.incoming)? "aliceblue" : "lightGreen"
			radius: 5


			Text {
				id: sendersName
				visible: model.incoming
				text: (model.incoming)? model.author_name + ":" : ""
				color: "cornflowerblue"
				font.bold: true

				anchors.leftMargin:lMarginBubble/2
				anchors.rightMargin: rMarginBubble
				anchors.topMargin: tMarginBubble
				anchors.top: bubble.top

				anchors.left: (model.incoming)?  parent.left : undefined
				anchors.right:(!model.incoming)?  parent.right : undefined

				height: (model.incoming)? implicitHeight : 0
			}

			Text {
				id: timeText
				text: getMessageTime()
				color: "grey"

				anchors.left: (!model.incoming)?  parent.left : undefined
				anchors.right:(model.incoming)?  parent.right : undefined

				anchors.bottom:  bubble.bottom
				anchors.leftMargin:lMarginBubble
				anchors.rightMargin: rMarginBubble
				anchors.topMargin: tMarginBubble
			}


			Text {
				id: mesageText
				text: model.msg

				anchors.left: (model.incoming)?  parent.left : undefined
				anchors.right:(!model.incoming)?  parent.right : undefined

				anchors.top: sendersName.bottom
//				anchors.bottomMargin: bMarginBubble
				anchors.leftMargin:lMarginBubble
				anchors.rightMargin: rMarginBubble

//				wrapMode: Text.Wrap
			}



		}

	}

	function getMessageTime(){

		var timeFormat = "hh:mm";
		var recvDate = new Date(model.recv_time*1000)

		var timeString = Qt.formatDateTime(recvDate, timeFormat)
		return timeString
	}

 }



