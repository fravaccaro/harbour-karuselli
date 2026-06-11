import QtQuick 2.0
import Sailfish.Silica 1.0

CoverBackground {
    Label {
        anchors.centerIn: parent
        horizontalAlignment: Text.AlignHCenter
        color: Theme.primaryColor
        text: partnerController.partnerSpaceActive
              ? qsTr("%1/%2 apps").arg(partnerController.savedAppCount).arg(partnerController.maxCount)
              : qsTr("Disabled")
    }

    CoverActionList {
        id: coverAction

        CoverAction {
            iconSource: "image://theme/icon-cover-new"
            onTriggered: applicationWindow.activate()
        }
    }
}
