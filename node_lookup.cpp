
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

   QStringList tempStrList = temp.split("\n");

   auto iptoint = [](qint64  first, qint64 second, qint64 third, qint64 fourth) -> qint64 {
       return ((first << 24) | (second << 16) | (third << 8) | (fourth));
   };


   QFile f(QString("/tmp/geoip.txt"));
   f.open(QIODevice::ReadOnly);

   for(int i=0; i<tempStrList.count(); ++i)
   {
       if(!tempStrList[i].trimmed().length())
           continue;

       QStringList sl = tempStrList[i].split("|");

       if(!sl[0].trimmed().length())
           continue;

       QStringList sl2 = sl[0].split(".");

       qDebug() << "sl[0]" << sl[0];

       long long intip = iptoint(sl2[0].toInt(),
                          sl2[1].toInt(),
                          sl2[2].toInt(),
                          sl2[3].toInt());

       QByteArray bytes;
       while(!f.atEnd())
       {
           bytes=f.readLine();
           QString line(bytes);
           QStringList ipRanges = line.split(",");

           //qDebug() <<"comparing region : " <<ipRanges;
           if(ipRanges[2].trimmed() == "US")
           {
               if(intip >= ipRanges[0].toLongLong() && intip <= ipRanges[1].toLongLong())
               {
                   qDebug() <<"found " << ipRanges[2].trimmed() << " node : " << sl[0];
               }
           }
       }
       f.seek(0);
   }

   return strl;
}
