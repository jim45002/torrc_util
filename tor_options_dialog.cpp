
#include <QTimer>
#include <QFile>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFileDialog>
#include <QListWidget>
#include <QList>
#include <QLayout>

#include "coordinator.h"

#include "map_widget_interface.h"
#include "map_widget_factory.h"

#include "tor_options_dialog.h"
#include "ui_tor_options.h"
#include "tor_config_options.h"


TorOptionsDialog::TorOptionsDialog(QWidget* parent)
    : TorOptionsDialogInterface(parent),
      ui(new Ui::Dialog_tor_options)
{
   ui->setupUi(this);

   ui->progressBar->hide();

   mwfi = map_widget_factory::create(this);
   auto l = new QHBoxLayout;
   ui->map_widget->setLayout(l);
   l->addWidget(mwfi->map_dispaly_widget());
}


void TorOptionsDialog::setup_options_dialog()
{
   emit get_countries_map();

   connect(ui->countrylistWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
          this, SLOT(country_list_widget_double_click(QListWidgetItem*)));

   connect(ui->pushButton_ok,SIGNAL(clicked(bool)),
           this,SLOT(on_pushbutton_ok_clicked(bool)));
   connect(ui->pushButton_cancel,SIGNAL(clicked(bool)),
           this,SLOT(on_pushbutton_canceled_clicked(bool)));

   connect(ui->pushButton_movefrom_excludes,SIGNAL(clicked(bool)),
           this,SLOT(on_button_movefrom_excludes_clicked(bool)));
   connect(ui->pushButton_moveto_excludes,SIGNAL(clicked(bool)),
           this,SLOT(on_button_moveto_excludes_clicked(bool)));

   connect(ui->pushButton_moveto_exits,SIGNAL(clicked(bool)),
           this,SLOT(on_button_moveto_exits_clicked(bool)));
   connect(ui->pushButton_movefrom_exits,SIGNAL(clicked(bool)),
           this,SLOT(on_button_movefrom_exits_clicked(bool)));

   connect(ui->pushButton_moveto_entry,SIGNAL(clicked(bool)),
           this,SLOT(on_button_moveto_entry_clicked(bool)));
   connect(ui->pushButton_movefrom_entry,SIGNAL(clicked(bool)),
           this,SLOT(on_button_movefrom_entry_clicked(bool)));
}

TorOptionsDialog::~TorOptionsDialog()
{

}

void TorOptionsDialog::country_list_widget_double_click(QListWidgetItem* l)
{
  QString abbrv = countries_map[l->text().trimmed()];
  qDebug() << "country map has " << abbrv;
  emit request_node_list(abbrv,QStringList());
  ui->progressBar->show();

}

void TorOptionsDialog::recv_node_list(QString c,QStringList nodes)
{
  // qDebug() << "node list is " << c << " : " << nodes;
  node_records_map.clear();
  ui->nodelistWidget->clear();
  for(int dex=0; dex<nodes.count();++dex)
  {
      QStringList fields = nodes[dex].split("|");
      node_records_map[fields[0]] = nodes[dex];
      ui->nodelistWidget->insertItem(dex,fields[0]);
      auto item = ui->nodelistWidget->item(dex);
      QString tool_tip;
      tool_tip += c + "\n";
      for(auto f : fields)
      {
          tool_tip += f + "\n";
      }
      item->setToolTip(tool_tip);

      qDebug() << "added " << fields[0];
  }
  ui->progressBar->hide();
}

void TorOptionsDialog::completed_save_to_configfile(bool)
{

}

void TorOptionsDialog::add_strings_to_listwidget(QListWidget* l, const QStringList& strlist)
{
    l->clear();
    for(auto str : strlist)
    {
        l->addItem(str);
    }
}

void TorOptionsDialog::recv_progress(float p)
{
    qDebug() << "progress value : " << p;
    ui->progressBar->setValue(p);
}

void TorOptionsDialog::received_config_settings(QString config_option, QByteArray b)
{
    if(config_option == "ExcludeNodes")
    {
        exclude_nodes_options = b;
    }
    else
    {
        if(config_option == "ExitNodes")
        {
            exit_nodes_options = b;
        }
        else
        {
            if(config_option == "EntryNodes")
            {
                entry_nodes_options = b;
            }
        }
    }
}

void TorOptionsDialog::received_countries_map(QMap<QString, QString> m)
{
   // static int i;

    countries_map = m;
    //qDebug() << "Called " << ++i << "times " <<
    //            " set country_map to " << countries_map;

    for(auto item = countries_map.keyBegin();
        item != countries_map.keyEnd(); ++item)
    {
        ui->countrylistWidget->addItem(*item);
        country_str_list.append(*item);
    }

    emit sync_enforce_distinct_subnets_with_torrc();
    emit sync_strictnodes_with_torrc();
    emit sync_use_gaurds_with_torrc();
    emit sync_num_gaurds_with_torrc();

    emit populate_country_list(QString("ExcludeNodes"),
                               QStringList(),
                               country_str_list);

    emit populate_country_list(QString("ExitNodes"),
                               QStringList(),
                               country_str_list);

    emit populate_country_list(QString("EntryNodes"),
                               QStringList(),
                               country_str_list);

}

void TorOptionsDialog::received_populated_country_list(
        QString config_option,
        QStringList option_strings,
        QStringList country_list)
{
    country_str_list = country_list;

    add_strings_to_listwidget(
                ui->countrylistWidget,
                country_list);


    if(config_option == "ExcludeNodes")
    {
        add_strings_to_listwidget(
                    ui->excluded_nodes,
                    option_strings);
    }
    else
    {
        if(config_option == "ExitNodes")
        {
            add_strings_to_listwidget(
                        ui->exit_nodes,
                        option_strings);
        }
        else
        {
            if(config_option == "EntryNodes")
            {
                add_strings_to_listwidget(
                            ui->entry_nodes,
                            option_strings);
            }
        }
    }
}

void TorOptionsDialog::received_synced_enforce_distinct_subnets_with_torrc(bool b)
{
   ui->checkBox_enforce_subnets->setChecked(b);
}

void TorOptionsDialog::received_synced_num_gaurds_with_torrc(int val)
{
   ui->spinval_num_guards->setValue(val);
}

void TorOptionsDialog::received_synced_strictnodes_with_torrc(bool b)
{
    ui->checkBox_strict_nodes->setChecked(b);
}

void TorOptionsDialog::received_syned_use_gaurds_with_torrc(bool b)
{
    ui->checkBox_use_guards->setChecked(b);
}

///////////////////
void TorOptionsDialog::on_button_moveto_excludes_clicked(bool)
{
  auto selected = ui->countrylistWidget->selectedItems();
  for(auto i : selected)
  {
    ui->excluded_nodes->addItem(i->text());
    ui->excluded_nodes->sortItems();
    delete i;
  }
}


////////////////////
void TorOptionsDialog::on_button_movefrom_excludes_clicked(bool)
{
    auto selected = ui->excluded_nodes->selectedItems();
    for(auto i : selected)
    {
      ui->countrylistWidget->addItem(i->text());
      ui->countrylistWidget->sortItems();
      delete i;
    }
}


/////////////////////
void TorOptionsDialog::on_button_moveto_exits_clicked(bool)
{
    auto selected = ui->countrylistWidget->selectedItems();
    for(auto i : selected)
    {
      ui->exit_nodes->addItem(i->text());
      ui->exit_nodes->sortItems();
      delete i;
    }
}


/////////////////////
void TorOptionsDialog::on_button_movefrom_exits_clicked(bool)
{
    auto selected = ui->exit_nodes->selectedItems();
    for(auto i : selected)
    {
        ui->countrylistWidget->addItem(i->text());
        ui->countrylistWidget->sortItems();
        delete i;
    }
}


//////////////////////
void TorOptionsDialog::on_button_moveto_entry_clicked(bool)
{
    auto selected = ui->countrylistWidget->selectedItems();
    for(auto i : selected)
    {
      ui->entry_nodes->addItem(i->text());
      ui->entry_nodes->sortItems();
      delete i;
    }
}


//////////////////////
void TorOptionsDialog::on_button_movefrom_entry_clicked(bool)
{
    auto selected = ui->entry_nodes->selectedItems();
    for(auto i : selected)
    {
        ui->countrylistWidget->addItem(i->text());
        ui->countrylistWidget->sortItems();
        delete i;
    }
}


void TorOptionsDialog::ListWidgetStrings2QStringsList(QListWidget* l, QStringList& s)
{
   l->selectedItems();
   for(int i=0; i<l->count();++i)
   {
       s.append(l->item(i)->text());
   }
}

////////////////////
void TorOptionsDialog::on_pushbutton_ok_clicked(bool)
 {
     QStringList list_items;

     if(ui->excluded_nodes->count())
     {
         ListWidgetStrings2QStringsList(ui->excluded_nodes,
                                        list_items);
         emit update_excludenodes(list_items);
         list_items.clear();
     }

     if(ui->exit_nodes->count())
     {
        ListWidgetStrings2QStringsList(
                    ui->exit_nodes,list_items);
        emit update_exitnodes(list_items);
        list_items.clear();
     }

     if(ui->entry_nodes->count())
     {
         ListWidgetStrings2QStringsList(ui->entry_nodes,
                                        list_items);
         emit update_entrynodes(list_items);
         list_items.clear();
     }

     if(ui->checkBox_enforce_subnets->isChecked())
         emit update_EnforceDistinctSubnets(QString("1"));
     else
     {
         emit update_EnforceDistinctSubnets(QString("0"));
     }
     if(ui->checkBox_strict_nodes->isChecked())
         emit update_strictnodes(QString("1"));
     else
     {
         emit update_strictnodes(QString("0"));
     }

     if(ui->checkBox_use_guards->isChecked())
         emit update_useguards(QString("1"),
                                      QString::
                                      number(ui->spinval_num_guards->value()));
     else
     {
         emit update_useguards(QString("0"),
                                      QString::
                                      number(ui->spinval_num_guards->value()));
     }
     emit save_to_configfile();
     done(QDialog::Accepted);
 }

 void TorOptionsDialog::on_pushbutton_canceled_clicked(bool)
 {
   done(QDialog::Rejected);
 }
