import QtQuick 2.0
import Sailfish.Silica 1.0
import Opal.SupportMe 1.0
import "pages"
import "components"

ApplicationWindow {
    id: app

    function showSupportDialog() {
        askForSupport.show()
    }

    initialPage: Component { MainPage { } }
    cover: Qt.resolvedUrl("cover/CoverPage.qml")
    allowedOrientations: defaultAllowedOrientations

    AskForSupport {
        id: askForSupport
        contents: Component {
            KaruselliSupportDialog {}
        }
    }
}
