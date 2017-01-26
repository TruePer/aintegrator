////////////////////////////////////////////////////////////////////////////////////////////////////
/// A-Integrator
/// (c) 2016 - Denis Fatkulin - denis.fatkulin@gmail.com
////////////////////////////////////////////////////////////////////////////////////////////////////

import QtQuick 2.3
import QtQuick.Controls 1.2

import QtQuick.Layouts 1.1

import Material 0.2
import Material.Extras 0.1

import ai.common 1.0
import ai.main 1.0
import ai.state 1.0

ApplicationWindow {
	id: mainWindow
	visible: true
	title: core.state.applicationTitle() + tr.tr

	property var sections: [ "Cards", "Log" ]
	property string selectedComponent: sections[0]

	initialPage: page

	WindowStateSaver {
		window: mainWindow
		defaultWidth: 1000
		defaultHeight: 700
	}

	TabbedPage {
		id: page

		actionBar {
			customContent: Rectangle {
				id: panel
				anchors.top: parent.top
				anchors.left: parent.left
				anchors.right: parent.right
				color: Theme.primaryColor

				MenuField {
					id: workStationSelector
					textRole: "_workStation"
					width: Units.dp(200)
					anchors.left: parent.left
					textColor: Theme.lightDark(panel.color, Theme.light.iconColor, Theme.dark.iconColor)
					showMenuInCenter: false

					model: WorkStationModel {
						Component.onCompleted: {
							core.registerObserver(this)
						}
					}
				}

				IconButton {
					id: start
					anchors.left: workStationSelector.right
					anchors.verticalCenter:  workStationSelector.verticalCenter
					anchors.leftMargin: Units.dp(24)
					iconName: "av/play_circle_outline"
					color: Theme.lightDark(page.actionBar.backgroundColor, Theme.light.iconColor, Theme.dark.iconColor)
					size: Units.dp(40)

					onClicked: {
						controller.launchWorkStation(workStationSelector.selectedText)
					}
				}

				IconButton {
					id: stop
					anchors.left: start.right
					anchors.verticalCenter:  start.verticalCenter
					anchors.leftMargin: Units.dp(6)
					iconName: "av/pause_circle_outline"
					color: Theme.lightDark(page.actionBar.backgroundColor, Theme.light.iconColor, Theme.dark.iconColor)
					size: Units.dp(40)

					onClicked: {
						controller.stopWorkStation(workStationSelector.selectedText)
					}
				}

				IconButton {
					id: languageButton
					anchors.right: panel.right
					anchors.verticalCenter:  start.verticalCenter
					iconName: "action/language"
					color: Theme.lightDark(page.actionBar.backgroundColor, Theme.light.iconColor, Theme.dark.iconColor)
					size: Units.dp(24)
					onClicked: languageDialog.show()
				}

				IconButton {
					id: colorButton
					anchors.right: languageButton.left
					anchors.verticalCenter:  start.verticalCenter
					anchors.rightMargin: Units.dp(24)
					iconName: "image/color_lens"
					color: Theme.lightDark(page.actionBar.backgroundColor, Theme.light.iconColor, Theme.dark.iconColor)
					size: Units.dp(24)
					onClicked: colorDialog.show()
				}

				IconButton {
					id: dragButton
					property bool checked: false

					anchors.right: colorButton.left
					anchors.verticalCenter:  start.verticalCenter
					anchors.rightMargin: Units.dp(24)
					iconName: checked ? "action/picture_in_picture_alt" : "action/picture_in_picture"
					color: checked ? Theme.accentColor : Theme.lightDark(page.actionBar.backgroundColor, Theme.light.iconColor, Theme.dark.iconColor)
					size: Units.dp(24)
					onClicked: checked = !checked
				}
			}
		}

		Tab {
			title: qsTr("Modules")  + tr.tr
			iconName: "action/home"
			Cards {
				anchors.fill: parent
				workStation: workStationSelector.selectedText
				dragEnabled: dragButton.checked
			}
		}

		Tab {
			title: qsTr("Log")  + tr.tr
			iconName: "editor/insert_comment"

			LogConsole {
				anchors.fill: parent
			}
		}
	}

	Dialog {
		id: colorDialog
		title: qsTr("Pick color") + tr.tr

		positiveButtonText: qsTr("Done") + tr.tr
		negativeButtonText: qsTr("Cancel") + tr.tr

		property var primaryColor
		property var accentColor
		property var backgroundColor

		MenuField {
			id: colorSelection
			model: [qsTr("Primary color") + tr.tr, qsTr("Accent color") + tr.tr, qsTr("Background color") + tr.tr]
			width: Units.dp(160)
		}

		Grid {
			columns: 7
			spacing: Units.dp(8)

			Repeater {
				model: [
					"red", "pink", "purple", "deepPurple", "indigo",
					"blue", "lightBlue", "cyan", "teal", "green",
					"lightGreen", "lime", "yellow", "amber", "orange",
					"deepOrange", "grey", "blueGrey", "brown", "black",
					"white"
				]

				Rectangle {
					width: Units.dp(30)
					height: Units.dp(30)
					radius: Units.dp(2)
					color: Palette.colors[modelData]["500"]
					border.width: modelData === "white" ? Units.dp(2) : 0
					border.color: Theme.alpha("#000", 0.26)

					Ink {
						anchors.fill: parent

						onPressed: {
							switch(colorSelection.selectedIndex) {
							case 0:
								theme.primaryColor = parent.color
								break;
							case 1:
								theme.accentColor = parent.color
								break;
							case 2:
								theme.backgroundColor = parent.color
								break;
							}
						}
					}
				}
			}
		}

		onShowingChanged: {
			if(showing) {
				primaryColor = theme.primaryColor
				accentColor = theme.accentColor
				backgroundColor = theme.backgroundColor
			}
		}

		onRejected: {
			theme.primaryColor = primaryColor
			theme.accentColor = accentColor
			theme.backgroundColor = backgroundColor
		}
	}

	Dialog {
		id: languageDialog
		title: qsTr("Language select") + tr.tr
		positiveButtonText: qsTr("Done") + tr.tr
		negativeButtonText: qsTr("Cancel") + tr.tr

		MenuField {
			id: languageSelection
			model: [qsTr("English") + tr.tr, qsTr("Russian") + tr.tr]
			width: Units.dp(160)
		}

		onAccepted: {
			tr.selectLanguage(languageSelection.selectedIndex)
		}
	}

}
