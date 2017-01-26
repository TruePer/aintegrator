import QtQuick 2.0
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1
import Qt.labs.settings 1.0
import Material 0.2

Item {
	Rectangle {
		id: panel
		anchors.top: parent.top
		width: parent.width
		height: Units.dp(40)
		color: Theme.primaryColor

		Label {
			id: autoScrollTitle
			anchors.left: panel.left
			anchors.verticalCenter: panel.verticalCenter
			anchors.leftMargin: Units.dp(6)

			style: "subheading"
			text: qsTr("Autoscroll:")
			color: Theme.lightDark(panel.color, Theme.light.iconColor, Theme.dark.iconColor)
		}

		Switch {
			id: autoScroll
			anchors.left: autoScrollTitle.right
			anchors.verticalCenter: panel.verticalCenter
			anchors.leftMargin: Units.dp(6)

			checked: settings.autoscroll

			onClicked: {
				settings.autoscroll = checked
				if(checked) {
					consoleView.currentIndex = consoleView.count - 1
				} else {
					consoleView.currentIndex = -1
				}
			}
		}

		Settings {
			id: settings
			category: "Console"
			property bool autoscroll: true
		}
	}

	ListView {
		anchors.top: panel.bottom
		anchors.bottom: parent.bottom
		width: parent.width
		id: consoleView
		clip: true
		anchors.margins: 6

		model: core.state.logStorage
		delegate: Label {
			text: MessageTime + " " + ShortMessage
			color: MessageColor
			style: "dialog"
		}
	}

	Connections {
		target: consoleView.model
		onDataUpdated: {
			if(autoScroll.checked) {
				consoleView.currentIndex = count - 1
			}
		}
	}
}
