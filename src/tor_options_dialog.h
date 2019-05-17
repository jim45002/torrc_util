#ifndef TOR_OPTIONS_DIALOG_H
#define TOR_OPTIONS_DIALOG_H

#include <memory>

#include <QFile>
#include <QDialog>

#include "tor_options_dialog_interface.h"

namespace Ui {
    class Dialog_tor_options;
} // namespace Ui

class QFile;
class QListWidget;
class map_widget_interface;
class tor_config_options_interface;
class QMutex;

class TorOptionsDialog : public TorOptionsDialogInterface
{
    Q_OBJECT

public:
    TorOptionsDialog(QWidget* parent);
    virtual ~TorOptionsDialog();
    virtual void setup_options_dialog();

    void ListWidgetStrings2QStringsList(QListWidget *l, QStringList &s);
    void add_strings_to_listwidget(QListWidget* l, const QStringList &strlist);
    QStringList get_country_lat_lon(QString country_abbrv);
    void deselect_all_table_items();
public slots:
    virtual void recv_progress(float);
    virtual void recv_node_list(QString,QStringList);
    virtual void completed_save_to_configfile(bool);
    virtual void received_config_settings(QString config_option, QByteArray);
    virtual void received_countries_map(QMap<QString, QString>);
    virtual void received_populated_country_list(QString, QStringList, QStringList);
    virtual void received_synced_enforce_distinct_subnets_with_torrc(bool);
    virtual void received_synced_num_gaurds_with_torrc(int);
    virtual void received_synced_strictnodes_with_torrc(bool);
    virtual void received_syned_use_gaurds_with_torrc(bool);

    virtual void on_pushbutton_ok_clicked(bool = false);
    virtual void on_pushbutton_canceled_clicked(bool = false);
    virtual void on_button_moveto_excludes_clicked(bool);
    virtual void on_button_movefrom_excludes_clicked(bool);
    virtual void on_button_moveto_exits_clicked(bool);
    virtual void on_button_movefrom_exits_clicked(bool);
    virtual void on_button_moveto_entry_clicked(bool);
    virtual void on_button_movefrom_entry_clicked(bool);
    void country_list_widget_double_click(QListWidgetItem *);
    void tablewidget_undo_selected_pushbutton(bool);
    void on_button_moveto_exclude_exits_clicked(bool);
    void on_button_movefrom_exclude_exits_clicked(bool);
    void on_button_moveto_hs_layer_clicked(bool);
    void on_button_movefrom_hs_layer_clicked(bool);
    void on_button_moveto_hs_layer_3_clicked(bool);
    void on_button_movefrom_hs_layer_3_clicked(bool);
private:
    Ui::Dialog_tor_options *ui;
    QList <QString> configfile_lines;
    QMap<QString,QString> countries_map;
    QMap<QString,QString> node_records_map;
    QString filename;
    QString last_country_item_clicked;
    QStringList country_str_list;
    QByteArray exclude_nodes_options;
    QByteArray exit_nodes_options;
    QByteArray entry_nodes_options;
    std::shared_ptr<QMutex> nodelist_mutex;
    std::shared_ptr<map_widget_interface> mwfi;

};

#endif // TOR_OPTIONS_DIALOG_H
