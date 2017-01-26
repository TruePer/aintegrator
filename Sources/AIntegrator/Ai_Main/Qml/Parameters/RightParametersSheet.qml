import QtQuick 2.4
import Material 0.2
import Material.ListItems 0.1 as ListItem
import QtQuick.Layouts 1.2

RightSheet {
	id: root

	property string title
	property var model

	implicitHeight: listViewContainer.implicitHeight
	height: parent.height
	width: listView.contentWidth < 300 ? 300 : listView.contentWidth

	Column {
		id: column
		visible: root.model.valid

		anchors {
			top: parent.top
			right: parent.right
			left: parent.left
			bottom: buttons.top
		}

		ListItem.Subheader {
			id: header
			text: title
			visible: title !== ""
			height: Units.dp(56)
			style: "headline"
			backgroundColor: Theme.backgroundColor
			elevation: listView.atYBeginning ? 0 : 1
			fullWidth: true
			z: 2
		}

		Item {
			id: listViewContainer

			width: parent.width
			height: parent.height

			implicitHeight: listView.contentHeight + listView.topMargin + listView.bottomMargin

			Flickable {
				id: listView
				width: parent.width
				height: parent.height

				topMargin: 0
				bottomMargin: Units.dp(8)

				contentWidth: width
				contentHeight: subColumn.height + Units.dp(100)

				Column {
					id: subColumn
					width: parent.width
					spacing: Units.dp(24)

					Repeater {
						model: root.model

						delegate: ParameterItem {
								anchors.left: parent.left
								anchors.leftMargin: Units.dp(18)
								parameter: _parameter
						}
					}
				}
			}

			Scrollbar {
				flickableItem: listView
			}
		}
	}

	View {
		id: buttons
		width: parent.width
		height: btnApply.height + Units.dp(12)
		anchors.bottom: parent.bottom
		backgroundColor: Theme.backgroundColor
		z: 2
			Button {
				id: btnApply
				anchors.right: btnClose.left
				anchors.rightMargin: Units.dp(6)
				anchors.verticalCenter: parent.verticalCenter
				visible: root.model.valid

				text: qsTr("Apply") + tr.tr
				onClicked: {
					root.close()
					model.apllyParameters()
				}
			}
			Button {
				id: btnClose
				anchors.right: parent.right
				anchors.rightMargin: Units.dp(6)
				anchors.verticalCenter: parent.verticalCenter

				text: qsTr("Reject") + tr.tr
				onClicked: root.close()
			}

	}
}
