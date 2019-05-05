#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <memory>

#include <QMap>
#include <QMainWindow>

class QThread;
class DownloadManagerInterface;
class DarknetJSonInterface;
class DarknetJSonInterface;
class workerThread;

#include "main_window_interface.h"

namespace Ui {
class mainWindow;
}

class TorOptionsDialogInterface;
class tor_config_options_interface;

class MainWindow : public MainWindowInterface
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    virtual ~MainWindow();
    void backup_configfile(QString filename);
public slots:
    void on_start_button(bool);
    void on_open_tor_config_button(bool);
private:
    Ui::mainWindow *ui;
    QString filename;
};

#endif // MAINWINDOW_H
