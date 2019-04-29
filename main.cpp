/* Author: Yuriy Kuzin
 */
#include "mainwindow.h"
#include <QApplication>
#include <QMessageBox>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        QMessageBox::critical(0, QObject::tr("Systray"),
                              QObject::tr("I couldn't detect any system tray on this system."));
        return 1;
    }
    QApplication::setQuitOnLastWindowClosed(false);
    QString platform="";
    Q_UNUSED(platform);
    #if __GNUC__
        #if __x86_64__
            platform="-64bit";
        #endif
    #endif
    a.setProperty("appversion","0.01a" + platform);
    a.setProperty("appname","QT Optirun Indicator");
    #ifdef Q_OS_LINUX
        a.setWindowIcon(QIcon(":/images/intel_circle.png"));
    #endif

    MainWindow w;
    w.setWindowTitle(a.property("appname").toString() + " " + a.property("appversion").toString());
    //w.show();

    return a.exec();
}


