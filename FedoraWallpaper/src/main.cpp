#include <QApplication>
#include "ui/MainWindow.h"

int main(int argc, char *argv[]) {
    // Initialize Application
    QApplication app(argc, argv);

    // Create and show the wallpaper window
    MainWindow window;
    window.show();

    return app.exec();
}