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
    virtual void send_completed_save_to_configfile(bool);
    virtual void send_config_settings(QString config_option, QByteArray);
    virtual void send_countries_map(QMap<QString, QString>);
    virtual void send_populated_country_list(QString, QStringList, QStringList);
    virtual void send_synced_enforce_distinct_subnets_with_torrc(bool);
    virtual void send_synced_num_gaurds_with_torrc(int);
    virtual void send_synced_strictnodes_with_torrc(bool);
    virtual void send_syned_use_gaurds_with_torrc(bool);

public slots:
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