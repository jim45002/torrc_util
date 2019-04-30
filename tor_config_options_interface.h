#ifndef TOR_CONFIG_OPTIONS_INTERFACE_H
#define TOR_CONFIG_OPTIONS_INTERFACE_H

#include <QMap>
#include <QList>
#include <QString>
#include <QListWidget>

#include <QObject>

class QListWidget;

class tor_config_options_interface : public QObject
{
    Q_OBJECT

public:
    tor_config_options_interface(QObject* parent)
        : QObject(parent) { }
    virtual ~tor_config_options_interface() { }

signals:
    void ui_send_nodelist(QString,QStringList);
    void req_node_list(QString,bool);
    void req_download_nodelist();
    void send_completed_save_to_configfile(bool);
    void send_config_settings(QString config_option, QByteArray);
    void send_countries_map(QMap<QString, QString>);
    void send_populated_country_list(QString, QStringList, QStringList);
    void send_synced_enforce_distinct_subnets_with_torrc(bool);
    void send_synced_num_gaurds_with_torrc(int);
    void send_synced_strictnodes_with_torrc(bool);
    void send_syned_use_gaurds_with_torrc(bool);

public slots:
    virtual void ui_request_nodelist(QString,QStringList) = 0;
    virtual void recv_nodelist(QString,QStringList) = 0;
    virtual void recv_node_download_result(bool) = 0;
    virtual void save_to_configfile() = 0;
    virtual void read_tor_config()  = 0;
    virtual void load_list(QString filename) = 0;
    virtual void read_config_settings(QString config_option, QString& ) = 0;
    virtual void get_countries_map() = 0;
    virtual void update_useguards(QString , QString ) = 0;
    virtual void update_exitnodes(QStringList) = 0;
    virtual void update_excludenodes(QStringList) = 0;
    virtual void update_strictnodes(QString)  = 0;
    virtual void update_EnforceDistinctSubnets(QString) = 0;
    virtual void populate_country_list(QString,                                     
                                       QStringList, QStringList ) = 0;
    virtual void update_entrynodes(QStringList) = 0;
    virtual void sync_enforce_distinct_subnets_with_torrc() = 0;
    virtual void sync_num_gaurds_with_torrc() = 0;
    virtual void sync_strictnodes_with_torrc() = 0;
    virtual void sync_use_gaurds_with_torrc() = 0;
};

#endif // TOR_CONFIG_OPTIONS_INTERFACE_H
