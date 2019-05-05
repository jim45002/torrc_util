#ifndef TOR_OPTIONS_DIALOG_INTERFACE_H
#define TOR_OPTIONS_DIALOG_INTERFACE_H

#include <QMap>
#include <QList>
#include <QString>
#include <QListWidget>

#include <QDialog>

class QFile;

class TorOptionsDialogInterface : public QDialog
{
    Q_OBJECT

public:
    TorOptionsDialogInterface(QWidget* parent) : QDialog(parent) { }
    virtual ~TorOptionsDialogInterface() { }
    virtual void setup_options_dialog() = 0;

signals: 
    void request_node_list(QString,QStringList,bool);
    void save_to_configfile();
    void read_config_settings(QString config_option);
    void get_countries_map();
    void update_useguards(QString, QString);
    void update_exitnodes(QStringList);
    void update_hslayer2_nodes(QStringList);
    void update_hslayer3_nodes(QStringList);
    void update_excluded_exit_nodes(QStringList);
    void update_excludenodes(QStringList);
    void update_strictnodes(QString);
    void update_EnforceDistinctSubnets(QString);
    void populate_country_list(QString, QStringList, QStringList);
    void update_entrynodes(QStringList);
    void sync_enforce_distinct_subnets_with_torrc();
    void sync_num_gaurds_with_torrc() ;
    void sync_strictnodes_with_torrc();
    void sync_use_gaurds_with_torrc();

public slots:
    virtual void recv_progress(float) = 0;
    virtual void recv_node_list(QString,QStringList) = 0;
    virtual void completed_save_to_configfile(bool) = 0;
    virtual void received_config_settings(QString config_option,
                                          QByteArray) = 0;
    virtual void received_countries_map(QMap<QString, QString>) = 0;
    virtual void received_populated_country_list(QString, QStringList, QStringList) = 0;
    virtual void received_synced_enforce_distinct_subnets_with_torrc(bool) = 0;
    virtual void received_synced_num_gaurds_with_torrc(int) = 0;
    virtual void received_synced_strictnodes_with_torrc(bool) = 0;
    virtual void received_syned_use_gaurds_with_torrc(bool) = 0;

    virtual void on_pushbutton_ok_clicked(bool = false) = 0;
    virtual void on_pushbutton_canceled_clicked(bool = false) = 0;
    virtual void on_button_moveto_excludes_clicked(bool) = 0;
    virtual void on_button_movefrom_excludes_clicked(bool) = 0;
    virtual void on_button_moveto_exits_clicked(bool) = 0;
    virtual void on_button_movefrom_exits_clicked(bool) = 0;
    virtual void on_button_moveto_entry_clicked(bool) = 0;
    virtual void on_button_movefrom_entry_clicked(bool) = 0;
};


#endif // TOR_OPTIONS_DIALOG_INTERFACE_H
