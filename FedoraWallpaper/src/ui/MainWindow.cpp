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
    // 1. Basic Window Setup
    // REMOVED: Qt::BypassWindowManagerHint (This fixes the "On Top" issue)
    setWindowFlags(Qt::FramelessWindowHint); 
    setAttribute(Qt::WA_ShowWithoutActivating);

    // 2. Setup Layout & QML
    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    auto *qmlWidget = new QQuickWidget(this);
    qmlWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);
    qmlWidget->setClearColor(Qt::transparent);
    qmlWidget->setSource(QUrl("qrc:/assets/qml/Main.qml"));

    layout->addWidget(qmlWidget);

    // 3. Black Background
    QPalette pal = palette();
    pal.setColor(QPalette::Window, Qt::black);
    setAutoFillBackground(true);
    setPalette(pal);

    // 4. Connect Signals
    connect(qApp, &QGuiApplication::screenAdded, this, &MainWindow::updateMonitors);
    connect(qApp, &QGuiApplication::screenRemoved, this, &MainWindow::updateMonitors);
    connect(qApp, &QGuiApplication::primaryScreenChanged, this, &MainWindow::updateMonitors);

    // 5. Initial Geometry
    updateMonitors();
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
        Atom desktopAtom = XInternAtom(display, "_NET_WM_WINDOW_TYPE_DESKTOP", False);
        
        Atom stateAtom = XInternAtom(display, "_NET_WM_STATE", False);
        Atom belowAtom = XInternAtom(display, "_NET_WM_STATE_BELOW", False);

        // --- 2. Set Type to DESKTOP ---
        // Once this is set, GNOME allows the window to cover the panel area.
        XChangeProperty(display, windowID, typeAtom, XA_ATOM, 32, PropModeReplace, 
                        (unsigned char *)&desktopAtom, 1);

        // --- 3. Set State to BELOW ---
        XChangeProperty(display, windowID, stateAtom, XA_ATOM, 32, PropModeReplace, 
                        (unsigned char *)&belowAtom, 1);

        // --- 4. Force Geometry Reset (The Fix for the Black Bar) ---
        // Now that we are a "Desktop", we can force the window to (0,0) 
        // and the Window Manager will finally accept it.
        
        // Calculate total size again
        QRect totalGeometry;
        auto screens = QGuiApplication::screens();
        for (QScreen *screen : screens) {
            totalGeometry = totalGeometry.united(screen->geometry());
        }

        // Use XMoveResizeWindow to bypass Qt's internal offset cache
        XMoveResizeWindow(display, windowID, 
                          totalGeometry.x(), totalGeometry.y(), 
                          totalGeometry.width(), totalGeometry.height());

        // --- 5. Lower Window ---
        XLowerWindow(display, windowID);
        XFlush(display);
        XCloseDisplay(display);
    }
}

void MainWindow::updateMonitors() {
    // 1. Reset the Geometry Accumulator
    QRect totalGeometry;
    auto screens = QGuiApplication::screens();

    // 2. Combine ALL screens into one big rectangle
    for (QScreen *screen : screens) {
        totalGeometry = totalGeometry.united(screen->geometry());
    }

    // 3. Force the window to cover the TOTAL area
    // This fixes the "Black Bar" by explicitly setting the position 
    // to the absolute top-left (x, y) of the virtual desktop.
    setGeometry(totalGeometry);
    
    // 4. Force a repaint just in case
    update();
    
    // 5. Re-apply the "Bottom" layer logic 
    // (Sometimes unplugging a monitor resets window stacking)
    QTimer::singleShot(500, this, &MainWindow::pushToBottom);
}