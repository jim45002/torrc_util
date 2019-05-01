
#include <QtDebug>

#include "test_node_lookup.h"

test_node_lookup::test_node_lookup(QObject *parent) : QObject(parent)
{

}

void test_node_lookup::get_node_list(QString s)
{
    emit request_node_list(s,true);
}

void test_node_lookup::download_node_list()
{
    emit request_download_latest_node_list();
}

void test_node_lookup::recv_node_list(QString , QStringList s1)
{
   //qDebug() << "recv " << s << "  " << s1;
    nodes = s1;
}

void test_node_lookup::recv_download_result(bool b)
{
   download_result = b;
   qDebug() << "download result is " << b;
}

