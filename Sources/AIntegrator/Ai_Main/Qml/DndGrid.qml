import QtQuick 2.3
import QtQuick.Controls 1.2
import Material 0.2

Rectangle {
	id: root

	property int cellSize: Units.dp(300)
	property int spaceSize: Units.dp(10)
	property bool dragEnabled: true
	property var myModel: null

	color: Theme.backgroundColor

	Component {
		id: dndDelegate
		Item {
			id: wrapper
			width: dndGrid.cellWidth
			height: dndGrid.cellHeight

			Rectangle {
				id: card
				state: "inactive"
				anchors.centerIn: parent
				width: cellSize - spaceSize
				height: cellSize - spaceSize
				color: "transparent"
				smooth: true

				Rectangle {
					id: cardBorder
					anchors.centerIn: parent
					width: parent.width + 2*radius
					height: parent.height + 2*radius
					radius: 5
					color: "transparent"
					border.color: Theme.accentColor
					border.width: radius
					opacity: 0

					Behavior on opacity { NumberAnimation { duration: 300; easing.type: Easing.InOutQuad } }
				}

				Loader {
					id: loader
					property var size: root.cellSize
					anchors.fill: parent
					source: _card.qmlPath
				}

				states: [
					State {
						name: "greyedOut"
						when: (dndGrid.possibleDropIndex == index) && (dndGrid.draggedItemIndex != index) && (dndGrid.possibleDropIndex != -1)
						PropertyChanges { target: card; opacity: 0.7}
					},
					State {
						name: "inactive"
						when: (dndGrid.draggedItemIndex == -1) || (dndGrid.draggedItemIndex == index)
						PropertyChanges { target: card; opacity: 1.0}
					}
				]

				Behavior on width   { NumberAnimation { duration: 300; easing.type: Easing.InOutQuad } }
				Behavior on height  { NumberAnimation { duration: 300; easing.type: Easing.InOutQuad } }
				Behavior on opacity { NumberAnimation { duration: 300; easing.type: Easing.InOutQuad } }
			}

			Rectangle {
				id: circlePlaceholder
				width: 30; height: 30; radius: 30
				smooth: true
				anchors.centerIn: parent
				color: Theme.accentColor
				opacity: 0

				Behavior on opacity { NumberAnimation { duration: 300; easing.type: Easing.InOutQuad } }
			}

			states: [
				State {
					name: "inDrag"
					when: index == dndGrid.draggedItemIndex
					PropertyChanges { target: circlePlaceholder; opacity: 1 }
					PropertyChanges { target: cardBorder; opacity: 1 }
					PropertyChanges { target: card; parent: dndContainer }
					PropertyChanges { target: card; width: cellSize*0.8 }
					PropertyChanges { target: card; height: cellSize*0.8 }
					PropertyChanges { target: card; anchors.centerIn: undefined }
					PropertyChanges { target: card; x: drugMouseArea.mouseX - card.width / 2 }
					PropertyChanges { target: card; y: drugMouseArea.mouseY - card.height / 2 }
				}
			]
		}
	}

	ScrollView {
		id: scroller
		width: root.width
		height: root.height

		GridView {
			id: dndGrid
			anchors.fill: parent
			anchors.margins: spaceSize
			cellWidth: cellSize
			cellHeight: cellSize
			model: myModel
			delegate: dndDelegate

			interactive: false
			property int draggedItemIndex: -1
			property int possibleDropIndex: -1
			property int xCoordinateInPossibleDrop: -1
			property int yCoordinateInPossibleDrop: -1

			Item {
				id: dropPosIndicator
				visible: false
				height: dndGrid.cellHeight
				width: 10

				Rectangle {
					visible: parent.visible
					anchors.centerIn: parent
					width: 6
					radius: 3
					height: parent.height-spaceSize
					color: Theme.accentColor
					opacity: 0.5
				}

				states: [
					State {
						name: "shown"
						when: dndGrid.possibleDropIndex != -1
						PropertyChanges {
							target: dropPosIndicator
							visible: true
							x: dndGrid.xCoordinateInPossibleDrop
							y: dndGrid.yCoordinateInPossibleDrop
						}
					}
				]
			}

			Item {
				id: dndContainer
				anchors.fill: parent
			}

			MouseArea {
				id: drugMouseArea
				anchors.fill: parent
				enabled: dragEnabled
				onReleased: {
					if (dndGrid.draggedItemIndex != -1) {
						var draggedIndex = dndGrid.draggedItemIndex
						dndGrid.draggedItemIndex = -1
						dndGrid.possibleDropIndex = -1
						dndGrid.xCoordinateInPossibleDrop = -1
						dndGrid.yCoordinateInPossibleDrop = -1

						var min = draggedIndex
						var max = dndGrid.indexAt(mouseX, mouseY + scroller.flickableItem.contentY)

						if(max < 0 || min === max)
							return

						myModel.swap(min, max)
					}
				}

				onPressed: {
					dndGrid.draggedItemIndex = dndGrid.indexAt(mouseX, mouseY + scroller.flickableItem.contentY)
				}

				onPositionChanged: {
					var newPos = dndGrid.indexAt(mouseX, mouseY + scroller.flickableItem.contentY)
					if (newPos !== dndGrid.possibleDropIndex) {
						dndGrid.possibleDropIndex = newPos
						dndGrid.xCoordinateInPossibleDrop = Math.floor(mouseX/dndGrid.cellWidth) * dndGrid.cellWidth - 5
						var delta = scroller.flickableItem.contentY % dndGrid.cellHeight
						dndGrid.yCoordinateInPossibleDrop = Math.floor((mouseY + delta)/dndGrid.cellHeight) * dndGrid.cellHeight - delta
					}
				}
			}
		}
	}
}
