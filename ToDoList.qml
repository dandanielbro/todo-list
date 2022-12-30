import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3

import ToDo 1.0

ColumnLayout {
    Frame {
        id: root
        Layout.fillWidth: true

        ListView {
            implicitWidth: 250
            implicitHeight: 250
            clip: true
            anchors.fill: parent

            model: ToDoModel {
                // pass the toDoList, which is instantiated in main.cpp, to list property in ToDoModel
                list: toDoList
            }

            delegate: RowLayout {
                width: root.width

                CheckBox {
                    checked: model.done
                    onClicked: model.done = checked
                }
                TextField {
                    text: model.description
                    onEditingFinished: model.description = text
                    Layout.fillWidth: true
                }
            }
        }
    }

    RowLayout {
        Button {
            text: qsTr("Add new item")
            // While clicking the button, call appendItem()
            onClicked: toDoList.appendItem()
            Layout.fillWidth: true
        }

        Button {
            text: qsTr("Remove completed")
            // While clicking the button, call removeCompletedItems()
            onClicked: toDoList.removeCompletedItems()
            Layout.fillWidth: true
        }
    }
}
