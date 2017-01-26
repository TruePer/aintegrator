import QtQuick 2.0
import Material 0.2
import Material.ListItems 0.1 as ListItem
import QtQuick.Controls 1.3 as QuickControls
import ai.main 1.0

import ".."

Item {
	id: root
	property var parameter

	width: parent.width
	height: checkBox.height + lineEdit.height + selectList.height + interval.height + group.height
	property int itemsWidth: width * 0.75

	// Group Name -----------------
	Item {
		id: group
		height: visible ? groupName.height : 0
		visible: parameter.type === Parameter.Group

		Label {
			id: groupName
			style: "title"
			color: Theme.accentColor
			text: parameter.name
		}
	}

	// CheckBox -----------------
	Item {
		id: checkBox
		height: visible ? checkBoxSwitcher.height : 0
		visible: parameter.type === Parameter.CheckBox

		Row {
			spacing: Units.dp(10)
			Switch {
				id: checkBoxSwitcher
				checked: parameter.checkBox.checked

				onCheckedChanged: {
					parameter.checkBox.setChecked(checked)
					checked = Qt.binding(function() { return parameter.checkBox.checked })
				}

			}
			Label {
				style: "subheading"
				text: parameter.name
			}
		}
	}

	// LineEdit -----------------
	Item {
		id: lineEdit
		height: visible ? lineEditText.height: 0
		visible:parameter.type === Parameter.LineEdit

		TextField {
			id: lineEditText
			helperText: parameter.name
			width: root.itemsWidth
			text: parameter.lineEdit.value
			validator: parameter.lineEdit.validator

			onTextChanged: parameter.lineEdit.setValue(text)
		}
	}

	// SelectList ----------------
	Item {
		id: selectList
		height: visible ? ( selectListMenu.visible ? selectListMenu.height : selectListRadioButtons.height): 0
		visible:parameter.type === Parameter.SelectList

		MenuField {
			helperText: parameter.name
			id: selectListMenu
			visible: parameter.selectList.type === SelectListParameter.ComboBox
			width: root.itemsWidth
			textColor: Theme.lightDark(Theme.backgroundColor, Theme.light.iconColor, Theme.dark.iconColor)
			model: parameter.selectList.items
			property int index: parameter.selectList.selectedIndex
			selectedIndex: parameter.selectList.selectedIndex

			onItemSelected: {
				if(selectedIndex !== index) {
					parameter.selectList.setSelectedIndex(index)
					selectedIndex = Qt.binding(function() { return parameter.selectList.selectedIndex })
				}
			}

			onIndexChanged: {
				selectedIndex = parameter.selectList.selectedIndex
				selectedIndex = Qt.binding(function() { return parameter.selectList.selectedIndex })
			}
		}

		Column {
			id: selectListRadioButtons
			spacing: Units.dp(-20)
			height: selectListSubHeader.height + selectListSubColumn.height
			visible: parameter.selectList.type === SelectListParameter.RadioButtons

			ListItem.Subheader {
				id: selectListSubHeader
				text: parameter.name
			}

			Column {
				id: selectListSubColumn
				width: root.itemsWidth
				spacing: selectListRadioButtons.spacing
				height: repeater.height

				Repeater {
					id: repeater
					model: parameter.selectList.items

					delegate: RadioButton {
						text: modelData
						checked: index === parameter.selectList.selectedIndex
						exclusiveGroup: optionGroup

						onCheckedChanged: {
							checked = Qt.binding(function() { return index === parameter.selectList.selectedIndex })
							parameter.selectList.setSelectedIndex(index)
						}

						Component.onCompleted: {
							repeater.height += (height + selectListSubColumn.spacing)
						}

						Component.onDestruction: {
							repeater.height -= (height - selectListSubColumn.spacing)
						}
					}
				}
			}
		}

		QuickControls.ExclusiveGroup { id: optionGroup }
	}

	// SelectList ----------------
	Item {
		id: interval
		height: visible ? ( intervalSlider.visible ? intervalSlider.height : intervalSpinBox.height): 0
		visible:parameter.type === Parameter.Interval

		Slider {
			id: intervalSlider
			visible: parameter.interval.type === IntervalParameter.Slider
			width: root.itemsWidth
			numericValueLabel: true

			minimumValue: parameter.interval.minValue
			maximumValue: parameter.interval.maxValue
			value: parameter.interval.value
			stepSize: parameter.interval.step

			onValueChanged: {
				parameter.interval.setValue(value)
			}
		}

        QuickControls.SpinBox {
			id: intervalSpinBox
			visible: parameter.interval.type === IntervalParameter.SpinBox
			width: root.itemsWidth

            minimumValue: parameter.interval.minValue
            maximumValue: parameter.interval.maxValue
			value: parameter.interval.value
			stepSize: parameter.interval.step

			onValueChanged: {
				parameter.interval.setValue(value)
			}
		}

		Label {
			anchors.top: intervalSlider.visible ? intervalSlider.bottom : intervalSpinBox.bottom
			id: intervalLabel
			color: Qt.darker(Theme.light.hintColor)
			text: parameter.name
		}
	}


	TooltipArea {
		text: parameter.description
	}
}
