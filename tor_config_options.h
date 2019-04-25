#ifndef TOR_CONFIG_OPTIONS_H
#define TOR_CONFIG_OPTIONS_H

#include <memory>

#include "tor_config_options_interface.h"

namespace Ui {
    class Dialog_tor_options;
} // namespace Ui

class QListWidget;
class QFile;

class tor_config_options : public tor_config_options_interface
{
    Q_OBJECT

public:
    tor_config_options(QString configfilename, QObject *parent=nullptr);
    virtual ~tor_config_options();

    void read_config_settings(QString config_option, QString &s);

public slots:
    virtual void save_to_configfile();
    virtual void read_tor_config();
    virtual void load_list(QString filename);
    virtual void read_config_settings(QString);
    virtual void get_countries_map();
    virtual void update_useguards(QString val, QString num);
    virtual void update_exitnodes(QStringList exit_nodes);
    virtual void update_excludenodes(QStringList excluded_nodes);
    virtual void update_strictnodes(QString val);
    virtual void update_EnforceDistinctSubnets(QString val);
    virtual void populate_country_list(QString config_setting,
        QStringList excludelistwidget, QStringList countrylistwidget);
    virtual void update_entrynodes(QStringList entry_nodes);
    virtual void sync_enforce_distinct_subnets_with_torrc();
    virtual void sync_num_gaurds_with_torrc();
    virtual void sync_strictnodes_with_torrc();
    virtual void sync_use_gaurds_with_torrc();
    virtual bool get_boolean_option_setting(QString boolean_option);
    virtual int get_int_option_setting(QString int_option);

private:
    Ui::Dialog_tor_options *ui;
    QList <QString> configfile_lines;
    QMap<QString,QString> countries_map;
    QString filename;
};

#endif // TOR_CONFIG_OPTIONS_H
