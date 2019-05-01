#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <memory>

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class map_widget_interface;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    std::shared_ptr<map_widget_interface> m;
};

#endif // MAINWINDOW_H
