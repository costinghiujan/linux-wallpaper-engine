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
    setWindowTitle("Fedora Wallpaper Engine");

    resize(800, 600);

    auto *layout = new QVBoxLayout(this);

    auto *label = new QLabel("Wallpaper Engine Core Loaded", this);
    label->setAlignment(Qt::AlignCenter);

    auto *closeButton = new QPushButton("Close Engine", this);

    layout->addWidget(label);
    layout->addWidget(closeButton);

    connect(closeButton, &QPushButton::clicked, this, &QWidget::close);
}