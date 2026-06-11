import QtQuick 2.0
import Sailfish.Silica 1.0
import Opal.Delegates 1.0 as D
import Opal.DragDrop 1.0
import "../components"

Page {
    id: page
    allowedOrientations: Orientation.All

    RemorsePopup {
        id: restartRemorse
    }

    Column {
        id: bottomPanel
        anchors {
            left: parent.left
            right: parent.right
            bottom: parent.bottom
        }
        visible: partnerController.showRestartHint
                 || partnerController.partnerSpaceEnabled
                 || partnerController.dirty

        MuotoTextLabel {
            visible: partnerController.showRestartHint
            text: qsTr("Partner space enabled. Restart the Home screen from the pulley menu to activate it.")
        }

        MuotoTextLabel {
            visible: partnerController.partnerSpaceEnabled
            text: qsTr("Native apps may look transparent on the Home screen. Install “Blur Partnerspace Background” from Patchmanager.")
        }

        ButtonLayout {
            id: applyBar
            x: Theme.horizontalPageMargin
            width: parent.width - 2 * Theme.horizontalPageMargin
            preferredWidth: Theme.buttonWidthSmall
            visible: partnerController.partnerSpaceEnabled || partnerController.dirty

            Button {
                text: qsTr("Apply")
                enabled: partnerController.dirty
                onClicked: {
                    if (partnerController.apply())
                        return
                }
            }
        }

        Item {
            width: parent.width
            height: Theme.paddingLarge
        }
    }

    SilicaListView {
        id: selectedAppsView
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: bottomPanel.top
        model: partnerController.selectedAppsModel

        header: PageHeader {
            title: "Karuselli"
        }

        ViewDragHandler {
            id: viewDragHandler
            listView: selectedAppsView
            active: partnerController.partnerSpaceEnabled
        }

        ViewPlaceholder {
            enabled: !partnerController.partnerSpaceEnabled
                     || (partnerController.partnerSpaceEnabled
                         && partnerController.selectedCount === 0)
            text: partnerController.partnerSpaceEnabled
                  ? qsTr("No apps selected")
                  : qsTr("Partner space disabled")
            hintText: partnerController.partnerSpaceEnabled
                      ? qsTr("Pull down to add an app")
                      : qsTr("Swipe down to enable the partner space")
        }

        delegate: D.TwoLineDelegate {
            id: rowDelegate
            width: selectedAppsView.width
            hidden: !partnerController.partnerSpaceEnabled
            interactive: false
            dragHandler: viewDragHandler
            text: name
            description: missing
                ? qsTr("Partner slot %1 · App not found").arg(index + 1)
                : qsTr("Partner slot %1").arg(index + 1)

            textLabel.opacity: missing ? Theme.opacityDisabled : 1.0
            descriptionLabel.opacity: missing ? Theme.opacityDisabled : 1.0

            leftItem: Component {
                Image {
                    width: Theme.iconSizeMedium
                    height: width
                    source: iconSource
                    fillMode: Image.PreserveAspectFit
                    opacity: missing ? Theme.opacityDisabled : 1.0
                }
            }

            menu: Component {
                ContextMenu {
                    MenuItem {
                        text: qsTr("Remove")
                        onClicked: {
                            var appPath = desktopPath
                            var controller = partnerController
                            rowDelegate.remorseDelete(function() {
                                controller.removeApp(appPath)
                            })
                        }
                    }
                }
            }
        }

        VerticalScrollDecorator {}

        PullDownMenu {
            MenuItem {
                text: qsTr("About")
                onClicked: pageStack.push(Qt.resolvedUrl("AboutPage.qml"))
            }
            MenuItem {
                visible: false
                text: qsTr("Support Karuselli")
                onClicked: app.showSupportDialog()
            }
            MenuItem {
                text: qsTr("Restart the Home screen")
                onClicked: {
                    var controller = partnerController
                    restartRemorse.execute(
                        qsTr("Restarting the Home screen"),
                        function() { controller.restartHomescreen() })
                }
            }
            MenuItem {
                text: qsTr("Add app")
                enabled: partnerController.partnerSpaceEnabled
                         && partnerController.selectedAppsModel.count < partnerController.maxCount
                onClicked: pageStack.push(Qt.resolvedUrl("AppPickerPage.qml"))
            }
            MenuItem {
                text: partnerController.partnerSpaceEnabled
                      ? qsTr("Disable partner space")
                      : qsTr("Enable partner space")
                onClicked: partnerController.partnerSpaceEnabled =
                           !partnerController.partnerSpaceEnabled
            }
        }
    }
}
