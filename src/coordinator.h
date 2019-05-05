#ifndef COORDINATOR_H
#define COORDINATOR_H

#include <QObject>

class TorOptionsDialogInterface;
class  tor_config_options_interface;
class node_lookup_interface;

class Coordinator : public QObject
{
    Q_OBJECT

public:
    Coordinator();
    ~Coordinator() {}

    bool make_connections(TorOptionsDialogInterface*,
                          tor_config_options_interface*);

     bool make_connections(node_lookup_interface*,
                 tor_config_options_interface*);
};

#endif // COORDINATOR_H
