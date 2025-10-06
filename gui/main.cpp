#include "mainwindow.h"  // Your GUI header (lowercase filename OK)
#include <QApplication>  // Qt app bootstrap
#include <QDebug>        // Optional debug

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);  // Create app instance
    MainWindow w;                // Capital 'M'—matches header class name
    w.show();                    // Display it (QMainWindow method)
    qDebug() << "GUI Main: Window shown";  // Debug to confirm run
    return a.exec();             // Event loop (blocks until close)
}