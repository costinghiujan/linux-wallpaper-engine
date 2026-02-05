#include "MainWindow.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent) : QWidget(parent) {
    setupUI();
}

MainWindow::~MainWindow() {
}

void MainWindow::setupUI() {
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnBottomHint | Qt::Tool);

    setWindowTitle("Fedora Wallpaper Engine");

    setWindowState(Qt::WindowFullScreen);

    QPalette pal = palette();
    pal.setColor(QPalette::Window, Qt::black);
    setAutoFillBackground(true);
    setPalette(pal);

    auto *layout = new QVBoxLayout(this);
    
    auto *label = new QLabel("Wallpaper Engine: Fullscreen Mode", this);
    label->setStyleSheet("color: white; font-size: 24px; font-weight: bold;");
    label->setAlignment(Qt::AlignCenter);
    
    auto *closeButton = new QPushButton("Close Engine", this);
    closeButton->setStyleSheet("background-color: white; color: black; padding: 10px;");
    
    layout->addWidget(label);
    layout->addWidget(closeButton);
    
    layout->setAlignment(closeButton, Qt::AlignCenter);

    connect(closeButton, &QPushButton::clicked, this, &QWidget::close);
}