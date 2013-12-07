#include <iostream>
#include <QApplication>

#include "mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    bool fullscreenMode = true;
    foreach (QString arg, a.arguments()) {
        if (arg == "--help" || arg == "-h") {
            std::cout << "Usage: " << a.arguments()[0].toStdString() << " [--help] [--no-fullscreen]" << std::endl;
            return 0;
        } else if (arg == "--no-fullscreen") {
            fullscreenMode = false;
        }
    }

    MainWindow w;
    if (fullscreenMode) {
        w.showFullScreen();
    } else {
        w.show();
    }

    return a.exec();
}
