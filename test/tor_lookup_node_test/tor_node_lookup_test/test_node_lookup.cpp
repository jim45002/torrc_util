
#include <QtDebug>

#include "test_node_lookup.h"

test_node_lookup::test_node_lookup(QObject *parent) : QObject(parent)
{

}

void test_node_lookup::get_node_list(QString s)
{
  emit request_node_list(s,true);
}

void test_node_lookup::recv_node_list(QString s , QStringList s1)
{
   //qDebug() << "recv " << s << "  " << s1;
   nodes = s1;
}

