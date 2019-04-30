
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

void node_lookup::download_nodelist()
{
    QString program = "curl";
    QStringList arguments;
    arguments << "https://www.dan.me.uk/tornodes";

    qprocess_ptr = std::make_shared<QProcess>();
    qprocess_ptr->start(program, arguments);
    qprocess_ptr->waitForFinished();
    qprocess_ptr->waitForReadyRead();

    QByteArray b = qprocess_ptr->readAllStandardOutput();
    bool r = false;
    if(b.indexOf(QString("Umm... You can only fetch the data every"
                         " 30 minutes - sorry.")) < 0)
    {
        QFile f(QString("./TOR Node List.tmp"));
        bool opened = f.open(QIODevice::WriteOnly);
        if(!opened)
        {
            qDebug() << "unable to open " << f.fileName();
        }
        else
        {
            bool ioresult = f.write(b);
            if(f.size()>1024 && ioresult)
            {
                f.copy(QString("./TOR Node List.html"));
                r=true;
            }
        }
    }
    emit send_download_result(r);
}


/////////////////////////////////////////
void node_lookup::get_node_list(QString country_abbrv,
                                bool make_country_file)
{
    qDebug() << "get node list" ;

    QStringList nodes;
    QFile nodelistfile(QString("./")+country_abbrv+".txt");
    if(nodelistfile.exists() && !make_country_file)
    {
        bool opened = nodelistfile.open(QIODevice::ReadOnly);
        if(!opened)
        {
            qDebug() << "unable to open " << nodelistfile.fileName();
        }
        else
        {
            while (!nodelistfile.atEnd())
            {
                nodes += nodelistfile.readLine();
            }
        }
    }
    else
    {
        QByteArray b;
        QFile f(QString("./TOR Node List.html"));
        bool opened = f.open(QIODevice::ReadOnly);
        if(!opened)
        {
            qDebug() << "unable to open " << f.fileName();
        }
        else
        {
            b = f.readAll();
            parseNodeList(b,nodes,country_abbrv,make_country_file);
        }
    }
    emit send_progress(100);
    emit send_node_list(country_abbrv,nodes);
}

void node_lookup::onReadyRead()
{

}


QStringList node_lookup::parseNodeList(QByteArray& b,
                                       QStringList& strl,
                                       QString country_abbrv,
                                       bool make_country_file)
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

   auto iptoint = [](qint64  first, qint64 second, qint64 third, qint64 fourth)
           -> qint64 {
       return ((first << 24) | (second << 16) | (third << 8) | (fourth));
   };

   if(make_country_file)
   {
      remove_nodelist_files();
   }

   QFile geoipfile(QString("./geoip.txt"));
   bool opened = geoipfile.open(QIODevice::ReadOnly);
   if(!opened)
       qDebug() << "unable to open " << geoipfile.fileName();

   float node_count = nodeStrList.count();
   float progress;
   int percent_to_int;
   for(int i=0; i<nodeStrList.count(); ++i)
   {
       if(!nodeStrList[i].trimmed().length())
           continue;

       QStringList node_record_str = nodeStrList[i].split("|");

       if(!node_record_str[0].trimmed().length() ||
           node_record_str[0].trimmed().indexOf(":") > -1)
           continue;

       QStringList node_record_ip_fields = node_record_str[0].split(".");

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
           if(intip >= ipRanges[0].toLongLong() &&
                   intip <= ipRanges[1].toLongLong() &&
                   ipRanges[2].trimmed().length())
           {
               nodeStrList[i] += "\n";
               if(make_country_file)
               {
                   QFile country_node_file(QString("./")+
                                           ipRanges[2].trimmed()+".txt");
                   country_node_file.open(QIODevice::Append);
                   country_node_file.write(nodeStrList[i].toStdString().c_str());
                   country_node_file.close();
               }
               if(ipRanges[2].trimmed() == country_abbrv.trimmed())
                    strl += nodeStrList[i];

               qDebug() <<"found " << ipRanges[2].trimmed()
                        << " node : " << node_record_str[0];
               break;
           }
       }
       geoipfile.seek(0);
       progress = float(i)/node_count;
       progress *= 100;
       qDebug() << " progress == " << progress;
       percent_to_int = progress;
       emit send_progress(percent_to_int);
    }

   return strl;
}


void node_lookup::remove_nodelist_files()
{
    QFile geoipfile(QString("./geoip.txt"));
    if(!geoipfile.open(QIODevice::ReadOnly))
    {
        qDebug() << "unable to open " << geoipfile.fileName();
        return;
    }

    QByteArray bytes;
    while(!geoipfile.atEnd())
    {
        bytes=geoipfile.readLine();
        QString geoipline(bytes);
        QStringList ipRanges = geoipline.split(",");
        if(!ipRanges[2].trimmed().length())
            continue;
        QFile country_node_file(QString("./")+ipRanges[2].trimmed()+".txt");
        if(country_node_file.exists())
        {
            country_node_file.remove();
        }
    }
}
