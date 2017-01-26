import QtQuick 2.4
import Material 0.2



PopupBase {
	id: rightSheet

	property int maxHeight: parent.height * 0.6

	default property alias content: containerView.data

	overlayLayer: "dialogOverlayLayer"
	overlayColor: Qt.rgba(0, 0, 0, 0.2)
	height: Math.min(maxHeight, implicitHeight)
	implicitHeight: containerView.childrenRect.height
	width: parent.width
	dismissOnTap: false

	visible: percentOpen > 0

	property real percentOpen: showing ? 1 : 0

	Behavior on percentOpen {

		NumberAnimation {
			duration: 200
			easing {
				type: Easing.OutCubic
			}
		}
	}

	anchors {
		bottom: parent.bottom
		right: parent.right
		rightMargin: (rightSheet.percentOpen - 1) * height
	}

	View {
		id:containerView

		anchors.fill: parent

		elevation: 2
		backgroundColor: Theme.backgroundColor
	}
}
