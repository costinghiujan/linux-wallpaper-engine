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
    void showEvent(QShowEvent *event) override;

private slots:
    void pushToBottom();
    void updateMonitors(); // <--- NEW: Handles screen changes

private:
    void setupUI();
};

#endif // MAINWINDOW_H