#ifndef MAIN_WINDOW_INTERFACE_H
#define MAIN_WINDOW_INTERFACE_H

#include <QMainWindow>

class MainWindowInterface : public QMainWindow
{
    Q_OBJECT

public:
    MainWindowInterface(QWidget* parent) : QMainWindow(parent) {}
    virtual ~MainWindowInterface() { }

public slots:

};

#endif // MAIN_WINDOWIN_TERFACE_H
