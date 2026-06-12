import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    id: page
    allowedOrientations: Orientation.All

    property string searchText: ""

    Component.onCompleted: {
        partnerController.appModel.filterText = ""
        searchText = ""
        searchField.text = ""
    }

    Column {
        anchors.fill: parent

        PageHeader {
            id: pickerHeader
            width: parent.width
            title: qsTr("Select app")
        }

        SearchField {
            id: searchField
            width: parent.width
            placeholderText: qsTr("Search apps")
            EnterKey.iconSource: "image://theme/icon-m-enter-close"
            EnterKey.onClicked: searchField.focus = false
            onTextChanged: {
                page.searchText = text
                partnerController.appModel.filterText = text
            }
        }

        SilicaListView {
            id: listView
            width: parent.width
            height: parent.height - pickerHeader.height - searchField.height
            model: partnerController.appModel

            delegate: ListItem {
                id: delegate
                width: listView.width
                enabled: !partnerController.selectedAppsModel.containsPath(desktopPath)
                contentHeight: Theme.itemSizeLarge

                Row {
                    anchors {
                        left: parent.left
                        leftMargin: Theme.horizontalPageMargin
                        verticalCenter: parent.verticalCenter
                    }
                    spacing: Theme.paddingMedium

                    Image {
                        width: Theme.iconSizeMedium
                        height: Theme.iconSizeMedium
                        source: iconSource
                        anchors.verticalCenter: parent.verticalCenter
                    }

                    Label {
                        anchors.verticalCenter: parent.verticalCenter
                        width: parent.parent.width - Theme.horizontalPageMargin * 2 - Theme.iconSizeMedium - Theme.paddingMedium
                        elide: Text.ElideRight
                        color: delegate.enabled
                               ? (delegate.highlighted ? Theme.highlightColor : Theme.primaryColor)
                               : Theme.secondaryColor
                        text: page.searchText.length > 0
                              ? Theme.highlightText(name, page.searchText, Theme.highlightColor)
                              : name
                    }
                }

                onClicked: {
                    partnerController.selectedAppsModel.appendFromMap({
                        name: name,
                        desktopPath: desktopPath,
                        iconSource: iconSource,
                        isAndroid: isAndroid,
                        missing: false
                    })
                    partnerController.appModel.filterText = ""
                    pageStack.pop()
                }
            }

            VerticalScrollDecorator {}
        }
    }
}
