
#include <iostream>

#include <QDebug>
#include <QProcess>
#include <QByteArray>
#include <QFile>
#include <QFileInfo>
#include <QDateTime>

#include "node_lookup.h"

node_lookup::node_lookup(QObject *parent)
    : node_lookup_interface(parent)
{

}

void node_lookup::download_nodelist()
{
    QFileInfo check_access_file(QString("./TOR Node List.tmp"));
    bool r = false;
    if((check_access_file.exists() == false) ||
            (check_access_file.lastModified().addSecs(60*45) <
             QDateTime::currentDateTime()))
    {
        QString program = "curl";
        QStringList arguments;
        arguments << "https://www.dan.me.uk/tornodes";

        qprocess_ptr = std::make_shared<QProcess>();
        qprocess_ptr->start(program, arguments);
        qprocess_ptr->waitForFinished();
        qprocess_ptr->waitForReadyRead();

        QByteArray b = qprocess_ptr->readAllStandardOutput();
        if(b.indexOf(QByteArray("<!-- __BEGIN_TOR_NODE_LIST__ //-->")) > -1)
        {
            if(b.indexOf(QByteArray("Umm... You can only fetch the data every"
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
                    long long ioresult = f.write(b);
                    if(ioresult > 32)
                    {
                        f.copy(QString("./TOR Node List.html"));
                        r=true;
                        remove_nodelist_files();
                    }
                }
            }
            else
            {
               qDebug() << "remote server error - unable to download node list";
            }
        }
    }
    else
    {
        qDebug() << "file access time error - unable to download node list";
    }
    if(!r)
        qDebug() << "unable to download node list";
    emit send_download_result(r);
}


/////////////////////////////////////////
void node_lookup::get_node_list(QString country_abbrv,
                                bool make_country_file)
{
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
        if(make_country_file)
        {
            download_nodelist();

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

    qDebug() << "node_lookup";
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

    QStringList geo_records;

    {
        QFile geoipfile(QString("./geoip.txt"));
        bool opened = geoipfile.open(QIODevice::ReadOnly);
        if(!opened)
        {
            qDebug() << "unable to open " << geoipfile.fileName();
            return QStringList();
        }
        QByteArray bytes;
        bytes=geoipfile.readAll();
        geoipfile.close();
        QString buff(bytes);
        geo_records = buff.split('\n');
    }

    /////////////

    QStringList geo_ipv6_records;
    QMap<QString,QString> ipv6_address_map;

    {
        QFile geoipv6_file(QString("./geoip6.txt"));
        bool opened = geoipv6_file.open(QIODevice::ReadOnly);
        if(!opened)
        {
            qDebug() << "unable to open " << geoipv6_file.fileName();
            return QStringList();
        }
        QByteArray bytes;
        bytes=geoipv6_file.readAll();
        geoipv6_file.close();
        QString buff(bytes);
        bytes.clear();
        geo_ipv6_records = buff.split('\n');

        QStringList fields;
        for(auto rec : geo_ipv6_records)
        {
            fields = rec.split(',');
            auto f0 = fields[0].split(':');

            for(int i=0;i<f0.size();++i)
            {
                // qDebug() << "before normalized field is " << f0[i];

                while(f0[i].size()<4)
                {
                    if(f0[i].size() == 0)
                    {
                        f0.removeAt(i);
                        i=0;
                        break;
                    }
                    else
                    {
                        f0[i].push_front(QString("0"));
                    }
                }
                // qDebug() << "normalized field is " << f0[i];
            }

            QString normal_field = f0.join(':');
            if(fields.size() == 3)
            {
                ipv6_address_map[normal_field.trimmed()] = fields[2].trimmed();


                //             qDebug() << "ipv6_address_map: added "
                //                    << normal_field
                //                    << " value "
                //                    << fields[2];
            }
        }
        qDebug() << "map size is " << ipv6_address_map.size();
    }

    float node_count = nodeStrList.count();
    float progress;
    int percent_to_int;
    bool is_ipv6 = false;
    for(int i=0; i<nodeStrList.count(); ++i)
    {
        if(!nodeStrList[i].trimmed().length())
            continue;

        QStringList node_record_str = nodeStrList[i].split("|");

        QStringList node_record_ip_fields;

        if(!node_record_str[0].trimmed().length() ||
                node_record_str[0].trimmed().indexOf(":") > -1)
        {
            qDebug() << "got ipv6 address";
            is_ipv6 = true;
            node_record_ip_fields = node_record_str[0].split(":");
        }
        else
        {
            is_ipv6 = false;
            node_record_ip_fields = node_record_str[0].split(".");
        }

        //qDebug() << " node_record_str[0] " << node_record_str[0];

        ///////////////////////////
        auto add_first_num_strings = [&] (
                const QStringList& strlist,
                const int num_strings
                )
        {
            QString ipv6_network_address;
            for(int index=0;index<num_strings;++index)
            {
                if((index+1) < num_strings)
                {
                    ipv6_network_address += strlist[index] + ":";
                }
                else
                {
                    ipv6_network_address += strlist[index];
                }
            }
            return ipv6_network_address.trimmed();
        };

        //////////////////////////
        auto ipv6_is_in_region = [&] () -> bool
        {
                const int num_strings = node_record_ip_fields.count();
                int d_count = 4;
                bool r=false;
                for(int index=0;index<num_strings;++index)
        {
            QString tmp =
                    add_first_num_strings(node_record_ip_fields,d_count);
            //              qDebug() << "looking for "
            //                       << tmp;
            auto iter=ipv6_address_map.find(tmp);
            if(iter != ipv6_address_map.end())
            {
                qDebug() << "found "  << *iter;
                r=true;
                if(make_country_file)
                {
                    QFile country_node_file(QString("./")+
                                            iter->trimmed()+".txt");
                    country_node_file.open(QIODevice::Append);
                    country_node_file.write(nodeStrList[i].
                                            toStdString().c_str());
                    country_node_file.close();
                }
                else
                {
                    // qDebug() << "not creating country node file";
                }
                if(iter->trimmed() == country_abbrv.trimmed())
                    strl += nodeStrList[i];
                break;
            }
            --d_count;
            if(d_count<2)
                break;
        }
        return r;
    };

    ///////////////////////////////
    auto do_ipv6_ranges = [&] () -> void
    {

            bool is_in_region = ipv6_is_in_region();
            qDebug() << "is_in_region returned " << is_in_region;

            progress = float(i)/node_count;
            progress *= 100;
            //qDebug() << " progress == " << progress;
            percent_to_int = progress;
            emit send_progress(percent_to_int);
};

    //////////////////////////////
    auto do_ipv4_ranges = [&] () -> void
    {
            long long intip=iptoint(node_record_ip_fields[0].toInt(),
            node_record_ip_fields[1].toInt(),
            node_record_ip_fields[2].toInt(),
            node_record_ip_fields[3].toInt());

    int v4_count=geo_records.count();
    for(int record_index=0; record_index < v4_count; ++record_index)
    {
        QStringList ipRanges = geo_records[record_index].split(",");

        // qDebug() <<"comparing region : " <<ipRanges;

        if(ipRanges.count() != 3)
            continue;

        bool is_in_region =
                (intip >= ipRanges[0].toLongLong() &&
                intip <= ipRanges[1].toLongLong() &&
                ipRanges[2].trimmed().length());

        if(is_in_region)
        {
            nodeStrList[i] += "\n";
            if(make_country_file)
            {
                QFile country_node_file(QString("./")+
                                        ipRanges[2].trimmed()+".txt");
                country_node_file.open(QIODevice::Append);
                country_node_file.write(nodeStrList[i].
                                        toStdString().c_str());
                country_node_file.close();
            }
            else
            {
                // qDebug() << "not creating country node file";
            }
            if(ipRanges[2].trimmed() == country_abbrv.trimmed())
                strl += nodeStrList[i];

            // qDebug() <<"found " << ipRanges[2].trimmed()
            //          << " node : " << node_record_str[0];
            break;
        }
    }
    progress = float(i)/node_count;
    progress *= 100;
    //qDebug() << " progress == " << progress;
    percent_to_int = progress;
    emit send_progress(percent_to_int);
};

if(is_ipv6)
{
    do_ipv6_ranges();
}
else
{
do_ipv4_ranges();
}
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
