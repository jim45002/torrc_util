
#include <QMap>
#include <QTimer>
#include <QFile>

#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QThread>

#include "coordinator.h"
#include "tor_options_factory.h"
#include "tor_options_dialog_factory.h"
#include "tor_options_dialog_interface.h"
#include "tor_config_options_interface.h"
#include "node_lookup_interface.h"
#include "node_lookup.h"

#include "ui_tor_options.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    :MainWindowInterface(parent),
     ui(new Ui::mainWindow)
{
    ui->setupUi(this);
    QObject::connect(ui->start_button,SIGNAL(clicked(bool)),this,
                     SLOT(on_start_button(bool)));

    QObject::connect(ui->open_config_button,SIGNAL(clicked(bool)),this,
                     SLOT(on_open_tor_config_button(bool))); 
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::backup_configfile(QString filename)
{
    QString backup_file;
    for(int count=0; count<1000; ++count)
    {
        if(!QFile::exists((backup_file=(filename+QString(".bak.")
                                        +QString::number(count)))))
        {
            qDebug() << "backup_file is " << backup_file;
            QFile::copy(filename,backup_file);
            break;
        }
    }
}

//////////////////////////
void MainWindow::on_start_button(bool)
{
  QFileDialog fileDialog(this);
  filename = fileDialog.getOpenFileName();
  backup_configfile(filename);
}

void MainWindow::on_open_tor_config_button(bool)
{
    QMessageBox msgbox;
    if(!filename.trimmed().length())
    {
        msgbox.setWindowTitle("Open Configuration");
        msgbox.setText("You must select the Tor"
                       " configuration file before clicking open.");
        msgbox.exec();
    }
    else
    {
        std::shared_ptr<TorOptionsDialogInterface>
        tor_options_dialog_interface;

        tor_options_dialog_factory::
                create_tor_config_options_dialog(tor_options_dialog_interface,
                                                 nullptr);

        std::shared_ptr<tor_config_options_interface>
        tor_config_options_interface_ptr;

        TorOptionsFactory::
                create_tor_config_options(tor_config_options_interface_ptr,
                                          filename,
                                          nullptr);

        std::shared_ptr<node_lookup_interface> nli =
                std::make_shared<node_lookup>();

        Coordinator c;
        c.make_connections(nli.get(),
                           tor_config_options_interface_ptr.get());

        c.make_connections(tor_options_dialog_interface.get(),
                           tor_config_options_interface_ptr.get());

        tor_options_dialog_interface->setup_options_dialog();

        QThread* ui_work_thread = new QThread(this);

        tor_config_options_interface_ptr->moveToThread(ui_work_thread);

        ui_work_thread->start();

        tor_options_dialog_interface->exec();

        ui_work_thread->quit();
        ui_work_thread->wait(30000);
        delete ui_work_thread;
        filename.clear();
    }
}
