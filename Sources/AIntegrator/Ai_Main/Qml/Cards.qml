import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.1
import QtQuick.Layouts 1.2
import Material 0.2

import ai.main 1.0
import ai.state 1.0

import "Parameters"

Item {
	id: root
	property string workStation: ""
	property bool dragEnabled: false

	CardGroupModel {
		id: groupModel
		workStation: root.workStation

		Component.onCompleted: {
			core.registerObserver(this)
		}
	}

	Component {
		id: groupDelegate

		Rectangle {
			width: parent.width
			height: listView.height / count
			color: Theme.backgroundColor

			Label {
				id: header
				style: "headline"
				text: group
				anchors {
					top: parent.top
					left: parent.left
					right: parent.right
				}

				anchors.leftMargin: Units.dp(40)
				anchors.topMargin: Units.dp(4)
				color: Theme.lightDark(Theme.backgroundColor, Theme.light.textColor, Theme.dark.textColor)
			}

			DndGrid {
				anchors.top: header.bottom
				anchors.bottom: parent.bottom
				width: parent.width
				dragEnabled: root.dragEnabled

				myModel: CardModel {
					id: card
					workStation: root.workStation
					moduleGroup: group

					Component.onCompleted: {
						core.registerObserver(this)
					}

					onShowModuleParameters: {
						parameterModel.executionModuleId = moduleId
						rightSheet.open()
						rightSheet.title = moduleName
					}
				}
			}
		}
	}

	ListView {
		id: listView
		anchors.fill: parent
		interactive: false
		spacing: 2
		model: groupModel
		delegate: groupDelegate
	}

	RightParametersSheet {
		id: rightSheet
		model: ParameterModel {
			id: parameterModel
			Component.onCompleted: {
				core.registerObserver(this)
			}
		}

		onOpened: model.opened()
		onClosed: model.closed()
	}
}


