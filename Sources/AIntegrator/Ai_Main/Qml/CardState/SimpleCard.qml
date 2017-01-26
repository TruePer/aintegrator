import QtQuick 2.0
import QtQuick.Layouts 1.2
import Material 0.2

Item {
	id: root
	anchors.centerIn: parent
	property bool pressed: false
	property int cellSize: width

	Rectangle {
		id: rect
		anchors.fill: parent
		color: Palette.colors[_card.bgColor]["400"]
		property var textColor: Theme.isDarkColor(color) ? Theme.dark.textColor : Theme.light.textColor
		property bool clicked: false

		MouseArea {
			id: mouseArea
			anchors.fill: parent

			onClicked:  controller.launchExecutionModule(_card.executionModuleId, _card.cardId)

			onPressed: {
				root.cellSize = width
				root.width  = cellSize * 0.98
				root.height = cellSize * 0.98
			}

			onReleased: {
				root.width  = cellSize
				root.height = cellSize
				root.cellSize = width
			}
		}

		ColumnLayout {
			anchors.fill: parent
			anchors.margins: Units.dp(10)

			RowLayout {
				anchors.margins: Units.dp(10)
				StatusIcon {
					statusIcon: _card.mainStatus
				}

				Label {
					text: _card.name
					style: "dialog"
					color: rect.textColor
					Layout.fillWidth: true
				}

				IconButton {
					iconName: "navigation/menu"
					color: rect.textColor
					visible: _card.hasParameters && _card.isValid
					onClicked: {
						_card.showParameters()
					}
				}
			}

			RowLayout {
				StatusIcon {
					statusIcon: _card.status1
				}
				StatusIcon {
					statusIcon: _card.status2
				}
				StatusIcon {
					statusIcon: _card.status3
				}
				StatusIcon {
					statusIcon: _card.status4
				}
				StatusIcon {
					statusIcon: _card.status5
				}
				StatusIcon {
					statusIcon: _card.status6
				}
				StatusIcon {
					statusIcon: _card.status7
				}
				StatusIcon {
					statusIcon: _card.status8
				}
				StatusIcon {
					statusIcon: _card.status9
				}
			}

			ColumnLayout {
				Layout.fillHeight: true
				Label {
					style: "dialog"
					text:  _card.textStatus1
					color: rect.textColor
				}
				Label {
					style: "dialog"
					text:  _card.textStatus2
					color: rect.textColor
				}
				Label {
					style: "dialog"
					text:  _card.textStatus3
					color: rect.textColor
				}
			}

			RowLayout {
				MenuField {
					Layout.fillWidth: true
					model: _card.workModes
					selectedIndex: _card.selectedIndex
					visible: _card.selectedIndex > -1
					property string workModeId: _card.selectedWorkModeId
					property int index: _card.selectedIndex
					textColor: Theme.lightDark(rect.color, Theme.light.iconColor, Theme.dark.iconColor)

					onIndexChanged: {
						selectedIndex = _card.selectedIndex
						selectedIndex = Qt.binding(function() { return _card.selectedIndex })
					}

					onSelectedIndexChanged: {
						_card.selectedIndex = selectedIndex
						selectedIndex = Qt.binding(function() { return _card.selectedIndex })
					}

					onItemSelected: {
						if(selectedIndex != index) {
							_card.startUpdateTimer()
							_card.selectedIndex = index
							controller.setWorkMode(_card.executionModuleId, _card.cardId, _card.selectedWorkModeId)
						}
					}
				}

				IconButton {
					id: actionButton
					iconName: _card.workModeActionIcon
					size: Units.dp(40)
					color:  rect.textColor
					visible: _card.selectedIndex > -1

					onClicked: {
						controller.workModeAction(_card.executionModuleId, _card.cardId)
						_card.startUpdateTimer()
					}

					states: [
						State {
							name: "updating"
							when: _card.isUpdating
							PropertyChanges { target: actionButton; color: Theme.accentColor }
							PropertyChanges { target: actionButton; iconName: "action/autorenew" }
						}
					]

					transitions: [
						Transition {
							from: "updating"
							to: "*"
							ColorAnimation { target: actionButton; duration: 1000}
						}
					]
				}
			}
		}

		states: [
			State {
				name: "clicked"
				when: rect.clicked
				PropertyChanges { target: root; width:  cellSize*0.98 }
				PropertyChanges { target: root; height: cellSize*0.98 }
			}
		]

		Behavior on color { ColorAnimation { duration: 500 } }
	}

	Behavior on width  { NumberAnimation { duration: 50; easing.type: Easing.InOutQuad } }
	Behavior on height { NumberAnimation { duration: 50; easing.type: Easing.InOutQuad } }
}
