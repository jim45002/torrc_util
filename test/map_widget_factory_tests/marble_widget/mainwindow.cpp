
#include <QWidget>
#include <QLayout>

#include "map_widget_interface.h"
#include "map_widget_factory.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m = map_widget_factory::create(this->ui->widget);
    auto l = new QHBoxLayout;
    ui->widget->setLayout(l);
    l->addWidget(m->map_dispaly_widget());
}

MainWindow::~MainWindow()
{
    delete ui;
}
