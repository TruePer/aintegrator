import QtQuick 2.0
import Material 0.2
import ai.main 1.0
import ".."

Item {
	id: root
	property var statusIcon
	property int animationDuration: 500

	width: statusIcon.isStandardIcon ? iconStandard.size : iconCustom.width
	height: statusIcon.isStandardIcon ? iconStandard.size : iconCustom.height

	Timer {
		id: timer
		interval: 10
		onTriggered: {
			icon.iconChanging = false
		}
	}

	Item {
		id: icon
		anchors.fill: parent
		property bool iconChanging: false

		Icon {
			id: iconStandard
			anchors.fill: parent
			name: statusIcon.standardIcon
			color: statusIcon.color
			visible: statusIcon.isStandardIcon

			onNameChanged: {
				icon.iconChanging = true
				timer.start()
			}

			onColorChanged: {
				icon.iconChanging = true
				timer.start()
			}
		}

		Image {
			id: iconCustom
			anchors.fill: parent
			source: statusIcon.isStandardIcon ? "" : "file://" + statusIcon.customIconPath
			visible: !statusIcon.isStandardIcon

			onSourceChanged: {
				icon.iconChanging = true
				timer.start()
			}
		}

		TooltipArea {
			text: statusIcon.description
		}
	}

	states: [
		State {
			name: "valid"
			when: statusIcon.isValid && !icon.iconChanging
		},
		State {
			name: "invalid"
			when: !statusIcon.isValid && !icon.iconChanging

			PropertyChanges { target: root; rotation: 90 }
			PropertyChanges { target: icon; opacity: 0 }
		},
		State {
			name: "changing"
			when: icon.iconChanging

			PropertyChanges { target: root; rotation: 90 }
			PropertyChanges { target: icon; opacity: 0 }
		}
	]

	transitions: [
		Transition {
			RotationAnimation { target: root; direction: RotationAnimation.Clockwise; duration: animationDuration; easing.type: Easing.InOutQuad }
			PropertyAnimation { target: icon; properties: "opacity "; duration: animationDuration; easing.type: Easing.InOutQuad }
		}
	]
}
