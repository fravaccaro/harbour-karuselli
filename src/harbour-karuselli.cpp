#include <sailfishapp.h>

#include <QGuiApplication>
#include <QQmlContext>
#include <QQmlEngine>
#include <QQuickView>

#include "partnercontroller.h"

int main(int argc, char *argv[])
{
    QScopedPointer<QGuiApplication> application(SailfishApp::application(argc, argv));
    QScopedPointer<QQuickView> view(SailfishApp::createView());

    view->engine()->addImportPath(
        SailfishApp::pathTo(QStringLiteral("qml/modules")).toString());

    PartnerController partnerController;
    view->rootContext()->setContextProperty(QStringLiteral("partnerController"), &partnerController);

    view->setSource(SailfishApp::pathToMainQml());
    view->show();

    return application->exec();
}
