import Opal.About 1.0 as A
import QtQuick 2.0
import Sailfish.Silica 1.0

A.AboutPageBase {
    appName: "Karuselli"
    appIcon: "/usr/share/icons/hicolor/128x128/apps/harbour-karuselli.png"
    appVersion: "0.1"
    description: qsTr("Customize the Sailfish OS partner space by choosing up to five apps.")
    authors: ["fravaccaro"]
    homepageUrl: "https://github.com/fravaccaro/harbour-karuselli"
    sourcesUrl: "https://github.com/fravaccaro/harbour-karuselli"
    licenses: A.License {
        spdxId: "GPL-3.0-or-later"
    }
    extraSections: [
        A.InfoSection {
            title: qsTr("Partner space")
            text: qsTr("When you enable the partner space, restart the Home screen from the pulley menu to activate it. Changing the app list does not require a restart.")
        },
        A.InfoSection {
            title: qsTr("Native apps")
            text: qsTr("Some native apps show a transparent background in the partner space. This is a Sailfish OS bug, not a Karuselli bug. To display them correctly, install the Patchmanager patch “Blur Partnerspace Background”.")
        },
        A.InfoSection {
            title: qsTr("Distribution")
            text: qsTr("Runs without Sailjail; intended for sideload or Chum distribution.")
        }
    ]
}
