#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QTimer>

class MainWindow : public QWidget {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    // Event triggered when window appears
    void showEvent(QShowEvent *event) override;

private slots:
    // Function to force window to bottom layer
    void pushToBottom();

private:
    void setupUI();
};

#endif // MAINWINDOW_H