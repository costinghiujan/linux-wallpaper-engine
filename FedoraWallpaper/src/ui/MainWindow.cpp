#include "MainWindow.h"
#include <QVBoxLayout>
#include <QQuickWidget>
#include <QScreen>
#include <QGuiApplication>
#include <QUrl>

// X11 Headers
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#undef KeyPress
#undef KeyRelease

MainWindow::MainWindow(QWidget *parent) : QWidget(parent) {
    setupUI();
}

MainWindow::~MainWindow() {
}

void MainWindow::setupUI() {
    // 1. Get Screen Geometry
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();

    // 2. Set Frameless and Desktop flags
    // "FramelessWindowHint" removes the title bar
    setWindowFlags(Qt::FramelessWindowHint);
    
    // 3. Prevent focus stealing
    setAttribute(Qt::WA_ShowWithoutActivating);

    // 4. Force Fullscreen Geometry
    setGeometry(screenGeometry);

    // 5. Setup Layout
    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    // 6. Create QML Widget
    auto *qmlWidget = new QQuickWidget(this);
    qmlWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);
    
    // 7. Load QML (Ensure this path matches the resource system!)
    qmlWidget->setSource(QUrl("qrc:/assets/qml/Main.qml"));

    layout->addWidget(qmlWidget);
}

void MainWindow::showEvent(QShowEvent *event) {
    QWidget::showEvent(event);
    // Delay the layering logic by 100ms to ensure window is mapped
    QTimer::singleShot(100, this, &MainWindow::pushToBottom);
}

void MainWindow::pushToBottom() {
    unsigned long windowID = this->winId();
    Display *display = XOpenDisplay(nullptr);
    
    if (display) {
        // --- 1. Define Atoms ---
        Atom typeAtom = XInternAtom(display, "_NET_WM_WINDOW_TYPE", False);
        
        // USE DESKTOP (Forces bottom), NOT DOCK (Forces top)
        Atom desktopAtom = XInternAtom(display, "_NET_WM_WINDOW_TYPE_DESKTOP", False);
        
        Atom stateAtom = XInternAtom(display, "_NET_WM_STATE", False);
        Atom belowAtom = XInternAtom(display, "_NET_WM_STATE_BELOW", False);
        Atom stickyAtom = XInternAtom(display, "_NET_WM_STATE_STICKY", False);

        // --- 2. Change Type to DESKTOP ---
        // This tells GNOME: "I am the wallpaper"
        XChangeProperty(display, windowID, typeAtom, XA_ATOM, 32, PropModeReplace, 
                        (unsigned char *)&desktopAtom, 1);

        // --- 3. Change State to BELOW + STICKY ---
        Atom states[] = { belowAtom, stickyAtom };
        XChangeProperty(display, windowID, stateAtom, XA_ATOM, 32, PropModeReplace, 
                        (unsigned char *)states, 2);

        // --- 4. Lower Window ---
        XLowerWindow(display, windowID);
        XFlush(display);
        XCloseDisplay(display);
    }
}