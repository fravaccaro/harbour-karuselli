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

    SilicaListView {
        id: selectedAppsView
        anchors.fill: parent
        model: partnerController.selectedAppsModel

        header: Column {
            width: parent.width

            PageHeader {
                title: "Karuselli"
            }

            LabelSpacer { }

            MuotoTextLabel {
                visible: partnerController.showRestartHint
                text: qsTr("Changes saved. Restart the homescreen from the pulley menu to see them.")
            }
        }

        footer: ButtonLayout {
            visible: partnerController.partnerSpaceEnabled || partnerController.dirty
            x: Theme.horizontalPageMargin
            width: parent.width - 2 * Theme.horizontalPageMargin
            preferredWidth: Theme.buttonWidthSmall

            Button {
                text: qsTr("Apply")
                enabled: partnerController.dirty
                onClicked: {
                    if (partnerController.apply())
                        return
                }
            }
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
                text: partnerController.partnerSpaceEnabled
                      ? qsTr("Disable partner space")
                      : qsTr("Enable partner space")
                onClicked: partnerController.partnerSpaceEnabled =
                           !partnerController.partnerSpaceEnabled
            }
            MenuItem {
                text: qsTr("Add app")
                enabled: partnerController.partnerSpaceEnabled
                         && partnerController.selectedAppsModel.count < partnerController.maxCount
                onClicked: pageStack.push(Qt.resolvedUrl("AppPickerPage.qml"))
            }
            MenuItem {
                text: qsTr("Restart homescreen")
                onClicked: {
                    var controller = partnerController
                    restartRemorse.execute(
                        qsTr("Restarting homescreen"),
                        function() { controller.restartHomescreen() })
                }
            }
            MenuItem {
                text: qsTr("About")
                onClicked: pageStack.push(Qt.resolvedUrl("AboutPage.qml"))
            }
            MenuItem {
                // debug
                visible: true
                text: qsTr("Support Karuselli")
                onClicked: app.showSupportDialog()
            }
        }
    }
}
