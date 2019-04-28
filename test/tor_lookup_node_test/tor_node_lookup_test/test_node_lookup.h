#ifndef TEST_NODE_LOOKUP_H
#define TEST_NODE_LOOKUP_H

#include <QObject>

class test_node_lookup : public QObject
{
    Q_OBJECT
public:
    explicit test_node_lookup(QObject *parent = nullptr);

    void get_node_list(QString);
    void download_node_list();
    QStringList getNodes() { return nodes; }
    bool get_latest_nodelist_download() {
        return download_result;
    }

signals:
    void request_node_list(QString,bool);
    void request_download_latest_node_list();
public slots:
    void recv_node_list(QString, QStringList);
    void recv_download_result(bool);

private:
    QStringList nodes;
    bool download_result;
};

#endif // TEST_NODE_LOOKUP_H
