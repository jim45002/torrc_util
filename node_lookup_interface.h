#ifndef NODE_LOOKUP_INTERFACE_H
#define NODE_LOOKUP_INTERFACE_H

#include <QStringList>
#include <QObject>

class node_lookup_interface : public QObject
{
    Q_OBJECT
public:

    node_lookup_interface(QObject*) {}
    virtual ~node_lookup_interface() { }

signals:
    void send_node_list(QString,QStringList);

public slots:
    virtual void get_node_list(QString,bool) = 0;
    virtual void download_nodelist() = 0;
};

#endif // NODE_LOOKUP_INTERFACE_H
