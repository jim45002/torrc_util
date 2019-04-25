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

class tor_config_options_interface;

class TorOptionsDialog : public TorOptionsDialogInterface
{
    Q_OBJECT

public:
    TorOptionsDialog(QWidget* parent);
    virtual ~TorOptionsDialog();
    virtual void setup_options_dialog();

    void ListWidgetStrings2QStringsList(QListWidget *l, QStringList &s);
    void add_strings_to_listwidget(QListWidget* l, const QStringList &strlist);
public slots:
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
private:
    Ui::Dialog_tor_options *ui;
    QList <QString> configfile_lines;
    QMap<QString,QString> countries_map;
    QString filename;
    QStringList country_str_list;
    QByteArray exclude_nodes_options;
    QByteArray exit_nodes_options;
    QByteArray entry_nodes_options;
};

#endif // TOR_OPTIONS_DIALOG_H
