#ifndef TEST_NODE_LOOKUP_H
#define TEST_NODE_LOOKUP_H

#include <QObject>

class test_node_lookup : public QObject
{
    Q_OBJECT
public:
    explicit test_node_lookup(QObject *parent = nullptr);

    void get_node_list(QString);
    QStringList getNodes() { return nodes; }

signals:
    void request_node_list(QString,bool);

public slots:
    void recv_node_list(QString, QStringList);

private:
    QStringList nodes;
};

#endif // TEST_NODE_LOOKUP_H
