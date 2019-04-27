#ifndef NODE_LOOKUP_H
#define NODE_LOOKUP_H

#include <memory>

#include "node_lookup_interface.h"

class  QProcess;

class node_lookup : public node_lookup_interface
{
    Q_OBJECT
public:
    explicit node_lookup(QObject *parent = nullptr);
    virtual ~node_lookup() { }

    QStringList parseNodeList(QByteArray& b,
                              QStringList& strl,
                              QString country_abbrv,
                              bool make_country_file);
    void remove_nodelist_files();
public slots:
    virtual void get_node_list(QString country_abbrv, bool make_country_file);
    void onReadyRead();

private:
    std::shared_ptr<QProcess> qprocess_ptr;

};

#endif // NODE_LOOKUP_H
