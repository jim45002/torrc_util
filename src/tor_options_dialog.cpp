
#include <QTimer>
#include <QFile>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFileDialog>
#include <QListWidget>
#include <QList>
#include <QLayout>
#include <QString>
#include <QMutex>

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

   nodelist_mutex = std::make_shared<QMutex>();

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

   connect(ui->table_widget_undo_pushButton,SIGNAL(clicked(bool)),
           this,SLOT(tablewidget_undo_selected_pushbutton(bool)));

   connect(ui->pushButton_moveto_exclude_exits,SIGNAL(clicked(bool)),
            this,SLOT(on_button_moveto_exclude_exits_clicked(bool)));

   connect(ui->pushButton_movefrom_exclude_exits,SIGNAL(clicked(bool)),
            this,SLOT(on_button_movefrom_exclude_exits_clicked(bool)));

   connect(ui->pushButton_movefrom_middle_layer_nodes,SIGNAL(clicked(bool)),
            this,SLOT(on_button_movefrom_hs_layer_clicked(bool)));

   connect(ui->pushButton_moveto_middle_layer_nodes,SIGNAL(clicked(bool)),
            this,SLOT(on_button_moveto_hs_layer_clicked(bool)));

   connect(ui->pushButton_movefrom_hs_layer_3_nodes,SIGNAL(clicked(bool)),
            this,SLOT(on_button_movefrom_hs_layer_3_clicked(bool)));

   connect(ui->pushButton_moveto_hs_layer_3_nodes,SIGNAL(clicked(bool)),
            this,SLOT(on_button_moveto_hs_layer_3_clicked(bool)));

}

TorOptionsDialog::~TorOptionsDialog()
{

}

QStringList TorOptionsDialog::get_country_lat_lon(QString country_abbrv)
{
  QByteArray bytes;
  QStringList location_records;
  QStringList result;
  QFile country_location_file(QString("./countries_gps_data.txt"));
  if(country_location_file.open((QIODevice::ReadOnly)))
  {
      bytes = country_location_file.readAll();
      location_records = QString(bytes).split('\n');
  }
  else
  {
      qDebug() << "cold not open file "
               << country_location_file.fileName();
  }

  for(auto rec : location_records)
  {
      QStringList fields = rec.split('\t');
      if( fields[0].trimmed() == country_abbrv.trimmed())
      {
        result = fields;
      }
  }
  return result;
}


void TorOptionsDialog::country_list_widget_double_click(QListWidgetItem* l)
{
  QString tmp;
  int len = (tmp = (QString("Nodes: ") + l->text())).length();
  QString text = tmp + QString(len<32?64-len:0,' ');
  ui->table_widget_title_label->setText(text);
  ui->table_widget_title_label->setToolTip(l->text());
  ui->node_list_table_widget->clear();
  QString abbrv = countries_map[l->text().trimmed()];
  QStringList latlon_rec = get_country_lat_lon(abbrv);
  if(latlon_rec.count()==4)
  {
     mwfi->centerOn(latlon_rec[2].toDouble(),latlon_rec[1].toDouble());
  }

  if(nodelist_mutex->tryLock())
  {
    emit request_node_list(abbrv,QStringList(),
                         ui->build_country_files_checkBox->isChecked());
    ui->progressBar->show();
  }
}

void TorOptionsDialog::recv_node_list(QString, QStringList nodes)
{

    auto selected_node_flag = [this] () -> QChar
    {
////            //FGRSDVX
            QChar result = 'z';
            if(ui->f_radioButton->isChecked())
            {
               result = 'f';
            }
            else
            if(ui->g_radioButton->isChecked())
            {
                 result = 'g';
            }
            else
            if(ui->r_radioButton->isChecked())
            {
              result = 'r';
            }
            else
            if(ui->s_radioButton->isChecked())
            {
              result = 's';
            }
            else
            if(ui->d_radioButton->isChecked())
            {
              result = 'd';
            }
            else
            if(ui->d_radioButton->isChecked())
            {
              result = 'd';
            }
            else
            if(ui->v_radioButton->isChecked())
            {
              result = 'v';
            }
            else
            if(ui->x_radioButton->isChecked())
            {
              result = 'x';
            }
            else
            if(ui->e_radioButton->isChecked())
            {
              result = 'e';
            }


            return result;
    };

    if(nodes.count())
    {
        node_records_map.clear();
        ui->node_list_table_widget->clear();
        ui->node_list_table_widget->setRowCount(nodes.count());
        ui->node_list_table_widget->
                setColumnCount(nodes[0].split("|").count());
        QStringList hlabels;
        // 132.167.123.136|JoeBy|443|21|FGRSDVX|59|Tor 0.2.9.16|
        //anon@simply-setup.script.by.Hello
        hlabels << "IP Address" << "Node" << "Port" << " ? " << "Flags" << " ? "
                << "Version" << "E-Mail";
        ui->node_list_table_widget->setHorizontalHeaderLabels(hlabels);
        QChar flag = selected_node_flag();
        for(int dex=0; dex<nodes.count();++dex)
        {
            QStringList fields = nodes[dex].split("|");

            if(!ui->all_radioButton->isChecked())
            {
               if(fields[4].indexOf(flag.toUpper()) < 0)
               {
                   continue;
               }
            }

            node_records_map[fields[0]] = nodes[dex];
            for(int col=0;col<fields.count();++col)
            {                                
                auto w = new QTableWidgetItem(fields[col]);
                ui->node_list_table_widget->setItem(dex,col,w);
                //            auto item = ui->node_list_table_widget->
                //                            item(dex,col);
                //            QString tool_tip;
                //            tool_tip += c + "\n";
                //            for(auto f : fields)
                //            {
                //                tool_tip += f + "\n";
                //            }
                //            item->setToolTip(tool_tip);
            }
           // qDebug() << "added " << fields[0];
        }
    }
    ui->progressBar->hide();
    nodelist_mutex->unlock();
}

void TorOptionsDialog::completed_save_to_configfile(bool)
{

}

void TorOptionsDialog::add_strings_to_listwidget(QListWidget* l,
                                                 const QStringList& strlist)
{
    l->clear();
    for(auto str : strlist)
    {
        l->addItem(str);
    }
}

void TorOptionsDialog::recv_progress(float p)
{
  //  qDebug() << "progress value : " << p;
    ui->progressBar->setValue(p);
}

////////////////////
void TorOptionsDialog::received_config_settings(QString config_option,
                                                QByteArray b)
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
    countries_map = m;
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

    emit populate_country_list(QString("HSLayer2Nodes"),
                               QStringList(),
                               country_str_list);

    emit populate_country_list(QString("ExcludeExitNodes"),
                               QStringList(),
                               country_str_list);

    emit populate_country_list(QString("HSLayer3Nodes"),
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
            else
            {
                if(config_option == "HSLayer2Nodes")
                {
                    add_strings_to_listwidget(
                                ui->listwidget_middle_layer_nodes,
                                option_strings);
                }
                else
                {
                    if(config_option == "ExcludeExitNodes")
                    {
                        add_strings_to_listwidget(
                                    ui->listwidget_excluded_exit_nodes,
                                    option_strings);
                    }
                    else
                    {
                        if(config_option == "HSLayer3Nodes")
                        {
                            add_strings_to_listwidget(
                                        ui->listwidget_hs_layer_3_nodes,
                                        option_strings);
                        }
                    }
                }
            }
        }
    }
}

void TorOptionsDialog::
received_synced_enforce_distinct_subnets_with_torrc(bool b)
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

void TorOptionsDialog::deselect_all_table_items()
{
    auto selected = ui->node_list_table_widget->selectedItems();
    for(auto i : selected)
    {
        i->setSelected(false);
    }
}

void TorOptionsDialog::tablewidget_undo_selected_pushbutton(bool)
{
    deselect_all_table_items();
}


///////////////////
void TorOptionsDialog::on_button_moveto_hs_layer_3_clicked(bool)
{
    QString tooltip = (ui->table_widget_title_label->text().remove("Nodes: ")).
                       trimmed();
    if(ui->node_list_table_widget->selectedItems().count())
    {
        auto selected = ui->node_list_table_widget->selectedItems();
        for(auto i : selected)
        {
            auto item = ui->node_list_table_widget->item(i->row(),0);

            if(ui->listwidget_hs_layer_3_nodes->findItems(item->text().
                                                             trimmed(),
                                             Qt::MatchExactly).count() == 0)
            {
                item->setToolTip(tooltip);
                ui->listwidget_hs_layer_3_nodes->addItem(item->text());
                selected.clear();
            }
        }
        QString str = ui->table_widget_title_label->text();
        str = str.remove(QString("Nodes:"));
        auto items = ui->countrylistWidget->findItems(str.trimmed(),
                                                      Qt::MatchExactly);
        if(items.count())
            items[0]->setSelected(false);
    }
    auto selected = ui->countrylistWidget->selectedItems();
    for(auto i : selected)
    {
        ui->listwidget_hs_layer_3_nodes->addItem(i->text());
        ui->listwidget_hs_layer_3_nodes->sortItems();
        delete i;
    }
}


/////////////////////
void TorOptionsDialog::on_button_movefrom_hs_layer_3_clicked(bool)
{
    auto selected = ui->listwidget_hs_layer_3_nodes->selectedItems();
    for(auto i : selected)
    {
        if(i->text().indexOf('.') > -1 || i->text().indexOf(':') > -1 )
        {

        }
        else
        {
            ui->countrylistWidget->addItem(i->text());
            ui->countrylistWidget->sortItems();
        }
        delete i;
    }
}


///////////////////
void TorOptionsDialog::on_button_moveto_hs_layer_clicked(bool)
{
    QString tooltip = (ui->table_widget_title_label->text().remove("Nodes: ")).
                       trimmed();
    if(ui->node_list_table_widget->selectedItems().count())
    {
        auto selected = ui->node_list_table_widget->selectedItems();
        for(auto i : selected)
        {
            auto item = ui->node_list_table_widget->item(i->row(),0);

            if(ui->listwidget_middle_layer_nodes->findItems(item->text().
                                                             trimmed(),
                                             Qt::MatchExactly).count() == 0)
            {
                item->setToolTip(tooltip);
                ui->listwidget_middle_layer_nodes->addItem(item->text());
                selected.clear();
            }
        }
        QString str = ui->table_widget_title_label->text();
        str = str.remove(QString("Nodes:"));
        auto items = ui->countrylistWidget->findItems(str.trimmed(),
                                                      Qt::MatchExactly);
        if(items.count())
            items[0]->setSelected(false);
    }
    auto selected = ui->countrylistWidget->selectedItems();
    for(auto i : selected)
    {
        ui->listwidget_middle_layer_nodes->addItem(i->text());
        ui->listwidget_middle_layer_nodes->sortItems();
        delete i;
    }
}


/////////////////////
void TorOptionsDialog::on_button_movefrom_hs_layer_clicked(bool)
{
    auto selected = ui->listwidget_middle_layer_nodes->selectedItems();
    for(auto i : selected)
    {
       if(i->text().indexOf('.') > -1 || i->text().indexOf(':') > -1 )
        {

        }
        else
        {
            ui->countrylistWidget->addItem(i->text());
            ui->countrylistWidget->sortItems();
        }
        delete i;
    }
}


///////////////////
void TorOptionsDialog::on_button_moveto_exclude_exits_clicked(bool)
{
    QString tooltip = (ui->table_widget_title_label->text().remove("Nodes: ")).
                       trimmed();
    if(ui->node_list_table_widget->selectedItems().count())
    {
        auto selected = ui->node_list_table_widget->selectedItems();
        for(auto i : selected)
        {
            auto item = ui->node_list_table_widget->item(i->row(),0);

            if(ui->listwidget_excluded_exit_nodes->findItems(item->text().
                                                             trimmed(),
                                             Qt::MatchExactly).count() == 0)
            {
                item->setToolTip(tooltip);
                ui->listwidget_excluded_exit_nodes->addItem(item->text());
                selected.clear();
            }
        }
        QString str = ui->table_widget_title_label->text();
        str = str.remove(QString("Nodes:"));
        auto items = ui->countrylistWidget->findItems(str.trimmed(),
                                                      Qt::MatchExactly);
        if(items.count())
            items[0]->setSelected(false);
    }
    auto selected = ui->countrylistWidget->selectedItems();
    for(auto i : selected)
    {
        ui->listwidget_excluded_exit_nodes->addItem(i->text());
        ui->listwidget_excluded_exit_nodes->sortItems();
        delete i;
    }
}


/////////////////////
void TorOptionsDialog::on_button_movefrom_exclude_exits_clicked(bool)
{
    auto selected = ui->listwidget_excluded_exit_nodes->selectedItems();
    for(auto i : selected)
    {
        if(i->text().indexOf('.') > -1 || i->text().indexOf(':') > -1 )
        {

        }
        else
        {
            ui->countrylistWidget->addItem(i->text());
            ui->countrylistWidget->sortItems();
        }
        delete i;
    }
}

///////////////////
void TorOptionsDialog::on_button_moveto_excludes_clicked(bool)
{
    QString tooltip = (ui->table_widget_title_label->text().remove("Nodes: ")).
                       trimmed();
    if(ui->node_list_table_widget->selectedItems().count())
    {
        auto selected = ui->node_list_table_widget->selectedItems();
        for(auto i : selected)
        {
            auto item = ui->node_list_table_widget->item(i->row(),0);

            if(ui->excluded_nodes->findItems(item->text().trimmed(),
                                             Qt::MatchExactly).count() == 0)
            {
                item->setToolTip(tooltip);
                ui->excluded_nodes->addItem(item->text());
                selected.clear();
            }
        }
        QString str = ui->table_widget_title_label->text();
        str = str.remove(QString("Nodes:"));
        auto items = ui->countrylistWidget->findItems(str.trimmed(),
                                                      Qt::MatchExactly);
        if(items.count())
            items[0]->setSelected(false);
    }
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
        if(i->text().indexOf('.') > -1 || i->text().indexOf(':') > -1 )
        {

        }
        else
        {
            ui->countrylistWidget->addItem(i->text());
            ui->countrylistWidget->sortItems();
        }
        delete i;
    }
}


/////////////////////
void TorOptionsDialog::on_button_moveto_exits_clicked(bool)
{
    QString tooltip = (ui->table_widget_title_label->text().remove("Nodes: ")).
                       trimmed();
    if(ui->node_list_table_widget->selectedItems().count())
    {
        auto selected = ui->node_list_table_widget->selectedItems();
        for(auto i : selected)
        {
            auto item = ui->node_list_table_widget->item(i->row(),0);

            if(ui->exit_nodes->findItems(item->text().trimmed(),
                                         Qt::MatchExactly).count() == 0)
            {
                item->setToolTip(tooltip);
                ui->exit_nodes->addItem(item->text());
                selected.clear();
            }
        }

        QString str = ui->table_widget_title_label->text();
        str = str.remove(QString("Nodes:"));
        auto items = ui->countrylistWidget->findItems(str.trimmed(),
                                                      Qt::MatchExactly);
        if(items.count())
            items[0]->setSelected(false);

    }

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
        if(i->text().indexOf('.') > -1 || i->text().indexOf(':') > -1 )
        {

        }
        else
        {
          int colon_offset = i->text().indexOf(QString(":"));
          if(colon_offset < 0)
          {
            ui->countrylistWidget->addItem(i->text());
            ui->countrylistWidget->sortItems();
          }
          else
          {

          }
        }
        delete i;
    }
}


//////////////////////
void TorOptionsDialog::on_button_moveto_entry_clicked(bool)
{
    QString tooltip = (ui->table_widget_title_label->text().remove("Nodes: ")).
                       trimmed();
    if(ui->node_list_table_widget->selectedItems().count())
    {
        auto selected = ui->node_list_table_widget->selectedItems();
        for(auto i : selected)
        {
            auto item = ui->node_list_table_widget->item(i->row(),0);

            if(ui->entry_nodes->findItems(item->text().trimmed(),
                                         Qt::MatchExactly).count() == 0)
            {
                item->setToolTip(tooltip);
                ui->entry_nodes->addItem(item->text());
                selected.clear();
            }
        }

        QString str = ui->table_widget_title_label->text();
        str = str.remove(QString("Nodes:"));
        auto items = ui->countrylistWidget->findItems(str.trimmed(),
                                                      Qt::MatchExactly);
        if(items.count())
            items[0]->setSelected(false);
    }

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
        if(i->text().indexOf('.') > -1 || i->text().indexOf(':') > -1 )
        {

        }
        else
        {
            ui->countrylistWidget->addItem(i->text());
            ui->countrylistWidget->sortItems();
        }
        delete i;
    }
}


void TorOptionsDialog::
ListWidgetStrings2QStringsList(QListWidget* l, QStringList& s)
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

     if(ui->listwidget_excluded_exit_nodes->count())
     {
         ListWidgetStrings2QStringsList(ui->listwidget_excluded_exit_nodes,
                                        list_items);
         emit update_excluded_exit_nodes(list_items);
         list_items.clear();
     }

     if(ui->listwidget_middle_layer_nodes->count())
     {
         ListWidgetStrings2QStringsList(ui->listwidget_middle_layer_nodes,
                                        list_items);
         emit update_hslayer2_nodes(list_items);
         list_items.clear();
     }

     if(ui->listwidget_hs_layer_3_nodes->count())
     {
         ListWidgetStrings2QStringsList(ui->listwidget_hs_layer_3_nodes,
                                        list_items);
         emit update_hslayer3_nodes(list_items);
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
