
#include <QMap>
#include <QList>
#include <QString>
#include <QListWidget>

#include "tor_config_options_interface.h"
#include "tor_options_dialog_interface.h"
#include "node_lookup_interface.h"
#include "coordinator.h"


Coordinator::Coordinator()
{

}

bool Coordinator::
make_connections(TorOptionsDialogInterface* tod,
                 tor_config_options_interface* tco)
{
   connect(tod,SIGNAL(get_countries_map()),
           tco,SLOT(get_countries_map()));

   connect(tod,SIGNAL(sync_enforce_distinct_subnets_with_torrc()),
           tco,SLOT(sync_enforce_distinct_subnets_with_torrc()));

   connect(tod,SIGNAL(sync_strictnodes_with_torrc()),
           tco,SLOT(sync_strictnodes_with_torrc()));

   connect(tod,SIGNAL(sync_use_gaurds_with_torrc()),
           tco,SLOT(sync_use_gaurds_with_torrc()));

   connect(tod,SIGNAL(update_strictnodes(QString )),
           tco,SLOT(update_strictnodes(QString )));

   connect(tod,SIGNAL(update_EnforceDistinctSubnets(QString )),
           tco,SLOT(update_EnforceDistinctSubnets(QString )));

   connect(tod,SIGNAL( populate_country_list(QString,
                                             QStringList,
                                             QStringList)),
           tco,SLOT( populate_country_list(QString,
                                           QStringList,
                                           QStringList)));

   connect(tod,SIGNAL(update_useguards(QString, QString)),
           tco,SLOT(update_useguards(QString, QString)));

   connect(tod,SIGNAL(update_excludenodes(QStringList)),
           tco,SLOT(update_excludenodes(QStringList)));

   connect(tod,SIGNAL(update_excluded_exit_nodes(QStringList)),
           tco,SLOT(update_exclude_exit_nodes(QStringList)));

   connect(tod,SIGNAL(
               update_hslayer2_nodes(QStringList)),
           tco,SLOT(
               update_hslayer2_nodes(QStringList)));

   connect(tod,SIGNAL(
               update_hslayer3_nodes(QStringList)),
           tco,SLOT(
               update_hslayer3_nodes(QStringList)));

   connect(tod,SIGNAL(update_exitnodes(QStringList)),
           tco,SLOT(update_exitnodes(QStringList)));

   connect(tod,SIGNAL(update_entrynodes(QStringList)),
           tco,SLOT(update_entrynodes(QStringList)));

   connect(tod,SIGNAL(sync_num_gaurds_with_torrc()),
           tco,SLOT(sync_num_gaurds_with_torrc()));

   connect(tod,SIGNAL(save_to_configfile()),
           tco,SLOT(save_to_configfile()));



   connect(tco,SIGNAL(send_countries_map(QMap<QString, QString>)),
           tod,SLOT(received_countries_map(QMap<QString, QString>)));

   connect(tco,SIGNAL( send_populated_country_list(QString, QStringList, QStringList) ),
           tod,SLOT(received_populated_country_list(QString, QStringList, QStringList)));

   connect(tco,SIGNAL(send_synced_enforce_distinct_subnets_with_torrc(bool) ),
           tod,SLOT(received_synced_enforce_distinct_subnets_with_torrc(bool) ));

   connect(tco,SIGNAL(send_synced_num_gaurds_with_torrc(int) ),
           tod,SLOT( received_synced_num_gaurds_with_torrc(int) ));

   connect(tco,SIGNAL(send_synced_strictnodes_with_torrc(bool) ),
           tod,SLOT(received_synced_strictnodes_with_torrc(bool) ));

   connect(tco,SIGNAL(send_syned_use_gaurds_with_torrc(bool) ),
           tod,SLOT( received_syned_use_gaurds_with_torrc(bool)));

   connect(tod,SIGNAL(request_node_list(QString,QStringList,bool)),
           tco,SLOT(ui_request_nodelist(QString,QStringList,bool)));

   connect(tco,SIGNAL(ui_send_nodelist(QString,QStringList)),
           tod,SLOT(recv_node_list(QString,QStringList)));

   connect(tco,SIGNAL(send_progress(float)),
           tod,SLOT(recv_progress(float)));


//   connect(tco,SIGNAL( ),
//           tod,SLOT( ));

//   connect(tco,SIGNAL( ),
//           tod,SLOT( ));


   return true;
}

bool Coordinator::
make_connections(node_lookup_interface* nli,
                 tor_config_options_interface* tcoi)
{
    connect(nli,
            SIGNAL(send_node_list(QString,QStringList)),
            tcoi,
            SLOT(recv_nodelist(QString,QStringList)));

    connect(tcoi,
            SIGNAL(req_node_list(QString,bool)),
            nli,
            SLOT(get_node_list(QString,bool)));

    connect(tcoi,
            SIGNAL(req_download_nodelist()),
            nli,
            SLOT(download_nodelist()));

    connect(nli,
            SIGNAL(send_download_result(bool)),
            tcoi,
            SLOT(recv_node_download_result(bool)));

    connect(nli,
            SIGNAL(send_progress(float)),
            tcoi,
            SLOT(recv_progress(float)));

    return true;
}

