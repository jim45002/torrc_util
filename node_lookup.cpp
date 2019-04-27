
#include <iostream>

#include <QDebug>
#include <QProcess>
#include <QByteArray>
#include <QFile>

#include "node_lookup.h"

node_lookup::node_lookup(QObject *parent)
    : node_lookup_interface(parent)
{

}

void node_lookup::get_node_list(QString country)
{
    QString program = "curl";
    QStringList arguments;
    arguments << "https://doc.qt.io/qt-5/qprocess.html";

    qprocess_ptr = std::make_shared<QProcess>();

    qprocess_ptr->start(program, arguments);

    qprocess_ptr->waitForFinished();
    qprocess_ptr->waitForReadyRead();

    QByteArray b = qprocess_ptr->readAllStandardOutput();

    QFile f(QString("/tmp/TOR Node List.html"));
    f.open(QIODevice::ReadOnly);
    b = f.readAll();
    QStringList nodes;
    parseNodeList(b,nodes);
    emit send_node_list(country,nodes);
}

void node_lookup::onReadyRead()
{

}


void node_lookup::remove_nodelist_files()
{
    QFile geoipfile(QString("/tmp/geoip.txt"));
    geoipfile.open(QIODevice::ReadOnly);

    QByteArray bytes;
    while(!geoipfile.atEnd())
    {
        bytes=geoipfile.readLine();
        QString geoipline(bytes);
        QStringList ipRanges = geoipline.split(",");
        if(!ipRanges[2].trimmed().length())
            continue;
        QFile country_node_file(QString("/tmp/")+ipRanges[2].trimmed()+".txt");
        if(country_node_file.exists())
        {
            country_node_file.remove();
        }
    }
}

QStringList node_lookup::parseNodeList(QByteArray& b, QStringList& strl)
{
   QString dataStr(b);
   QString begin("<!-- __BEGIN_TOR_NODE_LIST__ //-->");
   QString end("<!-- __END_TOR_NODE_LIST__ //-->");

   int bdex = dataStr.indexOf(begin) + begin.length();
   int edex = dataStr.indexOf(end);

   auto iter_begin = dataStr.begin() + bdex;
   auto iter_end   = dataStr.begin() + edex;
   QString temp;
   for(; iter_begin != iter_end; ++iter_begin)
   {
       temp.push_back(*iter_begin);
   }
   temp = temp.remove(QString("<br>"));
   temp = temp.remove(QString("&gt"));
   temp = temp.remove(QString("&lt"));

   QStringList nodeStrList = temp.split("\n");

   auto iptoint = [](qint64  first, qint64 second, qint64 third, qint64 fourth) -> qint64 {
       return ((first << 24) | (second << 16) | (third << 8) | (fourth));
   };

   remove_nodelist_files();

   QFile geoipfile(QString("/tmp/geoip.txt"));
   geoipfile.open(QIODevice::ReadOnly);

   for(int i=0; i<nodeStrList.count(); ++i)
   {
       if(!nodeStrList[i].trimmed().length())
           continue;

       QStringList node_record_str = nodeStrList[i].split("|");

       if(!node_record_str[0].trimmed().length())
           continue;

       QStringList node_record_ip_fields = node_record_str[0].split(".");

       if(!node_record_ip_fields.count())
           continue;

       qDebug() << " node_record_str[0] " << node_record_str[0];

       long long intip = iptoint(node_record_ip_fields[0].toInt(),
                          node_record_ip_fields[1].toInt(),
                          node_record_ip_fields[2].toInt(),
                          node_record_ip_fields[3].toInt());

       QByteArray bytes;
       while(!geoipfile.atEnd())
       {
           bytes=geoipfile.readLine();
           QString geoipline(bytes);
           QStringList ipRanges = geoipline.split(",");

           //qDebug() <<"comparing region : " <<ipRanges;
           if(intip >= ipRanges[0].toLongLong() && intip <= ipRanges[1].toLongLong() &&
                   ipRanges[2].trimmed().length())
           {
               QFile country_node_file(QString("/tmp/")+ipRanges[2].trimmed()+".txt");
               country_node_file.open(QIODevice::Append);
               nodeStrList[i] += "\n";
               country_node_file.write(nodeStrList[i].toStdString().c_str());
               country_node_file.close();
               qDebug() <<"found " << ipRanges[2].trimmed()
                        << " node : " << node_record_str[0];
               break;
           }
       }
       geoipfile.seek(0);
    }
   return strl;
}
