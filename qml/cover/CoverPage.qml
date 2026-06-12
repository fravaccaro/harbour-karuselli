import QtQuick 2.0
import Sailfish.Silica 1.0

CoverBackground {
    Image {
        id: coverBackground

        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width
        height: sourceSize.height * width / sourceSize.width
        fillMode: Image.PreserveAspectFit
        opacity: 0.25
        source: isLightTheme
                ? Qt.resolvedUrl("../../images/coverbg-light.png")
                : Qt.resolvedUrl("../../images/coverbg-dark.png")
    }

    Column {
        anchors.centerIn: parent
        width: parent.width - 2 * Theme.horizontalPageMargin
        spacing: Theme.paddingSmall

        Label {
            width: parent.width
            horizontalAlignment: Text.AlignHCenter
            color: Theme.primaryColor
            visible: !partnerController.partnerSpaceEnabled
                     || partnerController.selectedCount === 0
            text: partnerController.partnerSpaceEnabled
                  ? qsTr("Enabled")
                  : qsTr("Disabled")
        }

        Repeater {
            model: partnerController.partnerSpaceEnabled
                   && partnerController.selectedCount > 0
                   ? partnerController.selectedAppsModel
                   : null

            Label {
                width: parent.width
                horizontalAlignment: Text.AlignHCenter
                color: Theme.primaryColor
                text: name
                elide: Text.ElideRight
                maximumLineCount: 1
            }
        }
    }
}
