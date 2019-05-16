
#include <iostream>
#include <vector>
#include <memory>
#include <cstdio>
#include <fstream>
#include <cassert>
#include <functional>

#include <QTimer>
#include <QFile>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QList>
#include <QListWidget>
#include <QString>

#include "tor_config_options.h"


tor_config_options::tor_config_options(QString configfilename, QObject* parent)
    : tor_config_options_interface (parent),     
      filename(configfilename)
{
    load_list(":/data/geo/countryAbbrv.txt");
    read_tor_config();
}

tor_config_options::~tor_config_options()
{

}

void tor_config_options::save_to_configfile()
{
    qDebug() << "configfile_lines == " << configfile_lines;
    bool result = false;
    QByteArray bytes;
    std::shared_ptr<QFile> loadFile = std::make_shared<QFile>(filename);

        if (loadFile->open(QIODevice::WriteOnly))
        {
            loadFile->resize(0);
            for(auto line : configfile_lines)
            {
                if(line.length())
                {
                    if(line.indexOf(QChar('\n'))<0)
                    {
                        line+="\n";
                    }
                    loadFile->write(line.toStdString().c_str());
                }
                else
                {

                }
            }
            loadFile->close();
            result = true;
        }
        emit send_completed_save_to_configfile(result);
}

void tor_config_options::get_countries_map()
{
    emit send_countries_map(countries_map);
}


void tor_config_options::load_list(QString filename)
{
    QByteArray bytes;
    QFile loadFile(filename);

    if(loadFile.exists())
    {
        if (loadFile.open(QIODevice::ReadOnly))
        {
            QList <QString> country_abbrv_strings;
            while (!loadFile.atEnd())
            {
                bytes = loadFile.readLine();
                country_abbrv_strings.push_back(bytes.toStdString().c_str());
            }

            QStringList strList;

            for(auto str : country_abbrv_strings)
            {
                strList = str.split(QChar('\t'));
                countries_map[strList[0].trimmed()] = strList[1].trimmed();
            }
            loadFile.close();
        }
    }
}

////////////////////////////////////////////
/// \brief tor_config_options::get_boolean_option_setting
/// \param boolean_option
/// \return
///
bool tor_config_options::get_boolean_option_setting(QString boolean_option)
{
    bool result = false;
    for(int i=0; i<configfile_lines.count(); ++i)
    {
        if(configfile_lines[i].trimmed().indexOf(boolean_option) == 0)
        {
            if(configfile_lines[i].trimmed().indexOf('1') > -1)
            {
                result = true;
                break;
            }
            else
            {
                break;
            }
        }
    }
    return result;
}


int tor_config_options::get_int_option_setting(QString int_option)
{
    int r = 0;
    for(int i=0; i<configfile_lines.count(); ++i)
    {
        if(configfile_lines[i].trimmed().indexOf(int_option) == 0)
        {
            int l = configfile_lines[i].trimmed().length();
            if(::isdigit(configfile_lines[i].trimmed()[l-1].toLatin1()))
            {
                QString val(configfile_lines[i].trimmed()[l-1]);
                r = val.toInt();
                break;
            }
            else
            {
                break;
            }
        }
    }
    return r;
}

void tor_config_options::sync_num_gaurds_with_torrc()
{
   int val = get_int_option_setting(QString("NumEntryGuards"));
   if(val == 0)
      val = 1;
   emit send_synced_num_gaurds_with_torrc(val);
}


//////////////////////////////////////////
/// \brief tor_config_options::sync_use_gaurds_with_torrc
/// \return
///
void tor_config_options::sync_use_gaurds_with_torrc()
{
  bool b = get_boolean_option_setting(QString("UseEntryGuards"));
  emit send_syned_use_gaurds_with_torrc(b);
}

/////////////////////////////////////////
/// \brief tor_config_options::sync_enforce_distinct_subnets_with_torrc
/// \return bool
///
void tor_config_options::sync_enforce_distinct_subnets_with_torrc()
{
    bool result = false;
    int i=0;
    for(; i<configfile_lines.count(); ++i)
    {
        if(configfile_lines[i].trimmed().indexOf("EnforceDistinctSubnets") == 0)
        {
            if(configfile_lines[i].trimmed().indexOf('1') > -1)
            {
                result = true;
                break;
            }
            else
            {
                break;
            }
        }
    }
    if(i == configfile_lines.count())
        result = true;
    emit send_synced_enforce_distinct_subnets_with_torrc(result);
}


void tor_config_options::sync_strictnodes_with_torrc()
{
  bool b=get_boolean_option_setting(QString("StrictNodes"));
  emit send_synced_strictnodes_with_torrc(b);
}


//////////////////////////////////////
void tor_config_options::populate_country_list(QString config_setting,
                                               QStringList excludelistwidget,
                                               QStringList countrylistwidget)
{
    QString settings_data;

    auto type = settings_data.begin();

    auto is_ipv6_address =[&] (decltype(type) it)
    {
        int comma_offset = settings_data.indexOf(',');
        int colon_offset = settings_data.indexOf(QString(":"));

        bool result = false;
        if(colon_offset < comma_offset && colon_offset > 0)
        {
            while(*it != ',' && *it != '\n')
            {
                if(*it == ':')
                {
                    result = true;
                    break;
                }
                ++it;
            }
        }
        return result;
    };


    read_config_settings(config_setting,settings_data);
    QString abbrv;
    for(auto it = settings_data.begin();
        it != settings_data.end(); it++)
    {
        if(*it == '{')
        {
            while(*it != '}')
            {
                ++it;
                if(*it == ' ' || *it == '}')
                    continue;
                abbrv += *it;
            }

            QStringList keys = countries_map.keys();
            for(auto country : keys)
            {
                if(countries_map[country].toUpper().trimmed()
                        == abbrv.toUpper().trimmed())
                {
                    excludelistwidget.append(country.trimmed());
                    excludelistwidget.sort();

                    for(int index=0;
                        index<countrylistwidget.count(); ++index)
                    {
                        auto item = countrylistwidget.at(index);

                        if(item.trimmed() == country.trimmed())
                        {                         
                            countrylistwidget.removeAt(index);
                            countrylistwidget.sort();
                            break;
                        }
                    }
                    break;
                }
            }
            abbrv.clear();
        }
        else
        {
            QString ip_address;
            QString ipv6_address;
            bool is_ipv6=false;
            if(it->isNumber() && !(it+1)->isLetter() &&
                    ((is_ipv6=is_ipv6_address(it)) == false))
            {
                while(*it != ',' && *it != '\n')
                {
                    ip_address += *it;
                    ++it;
                }
                excludelistwidget.append(ip_address.trimmed());
            }
            else
            {
                if(is_ipv6)
                {
                    while(*it != ',' && *it != '\n')
                    {
                        ipv6_address += *it;
                        ++it;
                    }
                    excludelistwidget.append(ipv6_address.trimmed());
                }
            }
        }
    }
    emit send_populated_country_list(
                config_setting,excludelistwidget,countrylistwidget
                );
}


//////////////////////////////////////
void tor_config_options::read_config_settings(QString config_option)
{
    QString setting;
    for(int i=0; i<configfile_lines.count(); ++i)
    {
        int i2;
        if(configfile_lines[i].trimmed().indexOf(config_option) == 0)
        {
            for(i2=i; configfile_lines[i2].indexOf("\\")>=0; )
            {
                setting += configfile_lines[i2];
                setting = setting.remove('\\');
                setting = setting.remove('\n');
                setting = setting.remove('\t');
                setting.replace(QString(", "),QString(","));
                setting.replace(QString(" ,"),QString(","));
                configfile_lines[i2] = "";
                i2++;
            }
            configfile_lines[i2].remove('\t');
            configfile_lines[i2].remove(' ');
            setting += configfile_lines[i2];
            configfile_lines[i2] = "";
            configfile_lines[i] = setting;
            break;
        }
    }
    emit send_config_settings
            (config_option,QByteArray(setting.toStdString().c_str()));
}


void tor_config_options::read_config_settings(
        QString config_option, QString& s
        )
{
    QString setting;
    for(int i=0; i<configfile_lines.count(); ++i)
    {
        int i2;
        if(configfile_lines[i].trimmed().indexOf(config_option) == 0)
        {
            for(i2=i; configfile_lines[i2].indexOf("\\")>=0; )
            {
                setting += configfile_lines[i2];
                setting = setting.remove('\\');
                setting = setting.remove('\n');
                setting = setting.remove('\t');
                setting.replace(QString(", "),QString(","));
                setting.replace(QString(" ,"),QString(","));
                configfile_lines[i2] = "";
                i2++;
            }
            configfile_lines[i2].remove('\t');
            configfile_lines[i2].remove(' ');
            setting += configfile_lines[i2];
            configfile_lines[i2] = "";
            configfile_lines[i] = setting;
            break;
        }
    }
    s = setting;
}

void tor_config_options::recv_progress(float p)
{
   emit send_progress(p);
}

void tor_config_options::ui_request_nodelist(QString c, QStringList, bool b)
{
   emit req_node_list(c,b);
}

void tor_config_options::recv_nodelist(QString c, QStringList nl)
{
  emit ui_send_nodelist(c,nl);
}

void tor_config_options::recv_node_download_result(bool)
{

}


//////////////////////////////////////
void tor_config_options::update_EnforceDistinctSubnets(QString val)
{
     bool EnforceDistinctSubnets_update = false;
     for(int i=0; i<configfile_lines.count(); ++i)
     {
         if(configfile_lines[i].trimmed().indexOf("EnforceDistinctSubnets")==0)
         {
            configfile_lines[i] = "EnforceDistinctSubnets " + val;
            EnforceDistinctSubnets_update = true;
            break;
         }
     }
     QByteArray bytes;
     if(!EnforceDistinctSubnets_update)
     {
         bytes.clear();
         bytes += "EnforceDistinctSubnets " + val + "\n";
         configfile_lines.append(QString(bytes));
     }
}


//////////////////////////////////////
void tor_config_options::update_hslayer3_nodes(
        QStringList hslayer3_nodes)
{
    decltype (hslayer3_nodes) hs_layer3_nodesList;
    for(int i=0; i< hslayer3_nodes.count(); ++i)
    {
        hs_layer3_nodesList += hslayer3_nodes.at(i);
    }

    QStringList strList;
    int cnt = hs_layer3_nodesList.count();
    for(int i=0; i<cnt; ++i)
    {
        if((i+1) < cnt)
        {
            if(hs_layer3_nodesList[i].trimmed().indexOf('.') > -1 ||
                  hs_layer3_nodesList[i].trimmed().indexOf(':') > -1 )
            {
                strList += (QString(hs_layer3_nodesList[i].
                            trimmed()+","));
            }
            else
            {
                strList += (QString("{" + countries_map[hs_layer3_nodesList[i]].
                            trimmed() + "},"));
            }
        }
        else
        {
            if(hs_layer3_nodesList[i].trimmed().indexOf('.') > -1 ||
                hs_layer3_nodesList[i].trimmed().indexOf(':') > -1)
            {
                strList += (QString(hs_layer3_nodesList[i].
                            trimmed()));
            }
            else
            {
                strList += (QString("{" + countries_map[hs_layer3_nodesList[i]].
                            trimmed() + "}"));
            }
        }
    }

    for(int i=0; i<configfile_lines.count(); ++i)
    {
        if(configfile_lines[i].trimmed().indexOf("HSLayer3Nodes") == 0)
        {
            QString t("");
            read_config_settings("HSLayer3Nodes",t);
            if(strList.count())
                configfile_lines[i] = "HSLayer3Nodes " ;
            else
                configfile_lines[i] = "" ;
            for(auto l : strList)
            {
                configfile_lines[i] += l;
            }
            strList.clear();
            break;
        }
    }
    QByteArray bytes;
    if(strList.count())
    {
        bytes.clear();
        bytes += "HSLayer3Nodes " ;
        for(auto l : strList)
        {
           bytes += l;
        }
        configfile_lines.append(bytes+"\n");
    }
}



//////////////////////////////////////
void tor_config_options::update_hslayer2_nodes(
        QStringList hslayer2_nodes)
{
    decltype (hslayer2_nodes) hs_layer2_nodesList;
    for(int i=0; i< hslayer2_nodes.count(); ++i)
    {
        hs_layer2_nodesList += hslayer2_nodes.at(i);
    }

    QStringList strList;
    int cnt = hs_layer2_nodesList.count();
    for(int i=0; i<cnt; ++i)
    {
        if((i+1) < cnt)
        {
            if(hs_layer2_nodesList[i].trimmed().indexOf('.') > -1 ||
               hs_layer2_nodesList[i].trimmed().indexOf(':') > -1)
            {
                strList += (QString(hs_layer2_nodesList[i].
                            trimmed()+","));
            }
            else
            {
                strList += (QString("{" + countries_map[hs_layer2_nodesList[i]].
                            trimmed() + "},"));
            }
        }
        else
        {
            if(hs_layer2_nodesList[i].trimmed().indexOf('.') > -1 ||
               hs_layer2_nodesList[i].trimmed().indexOf(':') > -1)
            {
                strList += (QString(hs_layer2_nodesList[i].
                            trimmed()));
            }
            else
            {
                strList += (QString("{" + countries_map[hs_layer2_nodesList[i]].
                            trimmed() + "}"));
            }
        }
    }

    for(int i=0; i<configfile_lines.count(); ++i)
    {
        if(configfile_lines[i].trimmed().indexOf("HSLayer2Nodes") == 0)
        {
            QString t("");
            read_config_settings("HSLayer2Nodes",t);
            if(strList.count())
                configfile_lines[i] = "HSLayer2Nodes " ;
            else
                configfile_lines[i] = "" ;
            for(auto l : strList)
            {
                configfile_lines[i] += l;
            }
            strList.clear();
            break;
        }
    }
    QByteArray bytes;
    if(strList.count())
    {
        bytes.clear();
        bytes += "HSLayer2Nodes " ;
        for(auto l : strList)
        {
           bytes += l;
        }
        configfile_lines.append(bytes+"\n");
    }
}


//////////////////////////////////////
void tor_config_options::update_exclude_exit_nodes(
        QStringList excluded_exit_nodes)
{
    decltype (excluded_exit_nodes) excluded_exitnodesList;
    for(int i=0; i< excluded_exit_nodes.count(); ++i)
    {
        excluded_exitnodesList += excluded_exit_nodes.at(i);
    }

    QStringList strList;
    int cnt = excluded_exitnodesList.count();
    for(int i=0; i<cnt; ++i)
    {
        if((i+1) < cnt)
        {
            if(excluded_exitnodesList[i].trimmed().indexOf('.') > -1 ||
               excluded_exitnodesList[i].trimmed().indexOf(':') > -1)
            {
                strList += (QString(excluded_exitnodesList[i].
                            trimmed()+","));
            }
            else
            {
                strList += (QString("{" + countries_map[excluded_exitnodesList[i]].
                            trimmed() + "},"));
            }
        }
        else
        {
            if(excluded_exitnodesList[i].trimmed().indexOf('.') > -1 ||
               excluded_exitnodesList[i].trimmed().indexOf(':') > -1)
            {
                strList += (QString(excluded_exitnodesList[i].
                            trimmed()));
            }
            else
            {
                strList += (QString("{" + countries_map[excluded_exitnodesList[i]].
                            trimmed() + "}"));
            }
        }
    }

    for(int i=0; i<configfile_lines.count(); ++i)
    {
        if(configfile_lines[i].trimmed().indexOf("ExcludeExitNodes") == 0)
        {
            QString t("");
            read_config_settings("ExcludeExitNodes",t);
            if(strList.count())
                configfile_lines[i] = "ExcludeExitNodes " ;
            else
                configfile_lines[i] = "" ;
            for(auto l : strList)
            {
                configfile_lines[i] += l;
            }
            strList.clear();
            break;
        }
    }
    QByteArray bytes;
    if(strList.count())
    {
        bytes.clear();
        bytes += "ExcludeExitNodes " ;
        for(auto l : strList)
        {
           bytes += l;
        }
        configfile_lines.append(bytes+"\n");
    }
}


//////////////////////////////////////
void tor_config_options::update_entrynodes(QStringList entry_nodes)
{
    decltype (entry_nodes) entrynodesList;
    for(int i=0; i< entry_nodes.count(); ++i)
    {
        entrynodesList += entry_nodes.at(i);
    }

    QStringList strList;
    int cnt = entrynodesList.count();
    for(int i=0; i<cnt; ++i)
    {
        if((i+1) < cnt)
        {
            if(entrynodesList[i].trimmed().indexOf('.') > -1 ||
               entrynodesList[i].trimmed().indexOf(':') > -1)
            {
                strList += (QString(entrynodesList[i].
                            trimmed()+","));
            }
            else
            {
                strList += (QString("{" + countries_map[entrynodesList[i]].
                            trimmed() + "},"));
            }
        }
        else
        {
            if(entrynodesList[i].trimmed().indexOf('.') > -1 ||
               entrynodesList[i].trimmed().indexOf(':') > -1)
            {
                strList += (QString(entrynodesList[i].
                            trimmed()));
            }
            else
            {
                strList += (QString("{" + countries_map[entrynodesList[i]].
                            trimmed() + "}"));
            }
        }
    }

    for(int i=0; i<configfile_lines.count(); ++i)
    {
        if(configfile_lines[i].trimmed().indexOf("EntryNodes") == 0)
        {
            QString t("");
            read_config_settings("EntryNodes",t);
            if(strList.count())
                configfile_lines[i] = "EntryNodes " ;
            else
                configfile_lines[i] = "" ;
            for(auto l : strList)
            {
                configfile_lines[i] += l;
            }
            strList.clear();
            break;
        }
    }
    QByteArray bytes;
    if(strList.count())
    {
        bytes.clear();
        bytes += "EntryNodes " ;
        for(auto l : strList)
        {            
           bytes += l;
        }
        configfile_lines.append(bytes+"\n");
    }
}


//////////////////////////////////////
//
//
//
/////////////////////////////////////
void tor_config_options::update_strictnodes(QString val)
{

    bool strict_nodes_update = false;
    for(int i=0; i<configfile_lines.count(); ++i)
    {
        if(configfile_lines[i].trimmed().indexOf("StrictNodes")==0)
        {
           configfile_lines[i] = "StrictNodes " + val;

           strict_nodes_update = true;
           break;
        }
    }
    QByteArray bytes;
    if(!strict_nodes_update)
    {
        bytes.clear();
        bytes += "StrictNodes " + val;
        configfile_lines.append(bytes+"\n");
    }
}


void tor_config_options::update_excludenodes(QStringList excluded_nodes)
{
    decltype(excluded_nodes) excludesList;
    for(int i=0; i< excluded_nodes.count(); ++i)
    {
        excludesList += excluded_nodes.at(i);
    }

    QStringList strList;
    int cnt = excludesList.count();
    for(int i=0; i<cnt; ++i)
    {
        if((i+1) < cnt)
        {

            if(excludesList[i].trimmed().indexOf('.') > -1 ||
               excludesList[i].trimmed().indexOf(':') > -1)
            {
                strList += (QString(excludesList[i].
                            trimmed()+","));
            }
            else
            {
                strList += (QString("{" + countries_map[excludesList[i]].
                            trimmed() + "},"));
            }
        }
        else
        {
            if(excludesList[i].trimmed().indexOf('.') > -1 ||
               excludesList[i].trimmed().indexOf(':') > -1)
            {
                strList += (QString(excludesList[i].
                            trimmed()));
            }
            else
            {
                strList += (QString("{" + countries_map[excludesList[i]].
                            trimmed() + "}"));
            }
        }
    }

    for(int i=0; i<configfile_lines.count(); ++i)
    {
        if(configfile_lines[i].trimmed().indexOf("ExcludeNodes") == 0)
        {
            QString t("");
            read_config_settings("ExcludeNodes",t);
            if(strList.count())
                configfile_lines[i] = "ExcludeNodes " ;
            else
            {
                configfile_lines[i] = "" ;
            }
            for(auto l : strList)
            {
                configfile_lines[i] += l;
            }
            strList.clear();
            break;
        }
    }
    QByteArray bytes;
    if(strList.count())
    {
        bytes.clear();
        bytes += "ExcludeNodes " ;
        for(auto l : strList)
        {            
            if(l.length())
                bytes += l;
        }
        configfile_lines.append(bytes+"\n");
    }
}


//////////////////////////////////////////
void tor_config_options::update_exitnodes(QStringList exit_nodes)
{
    decltype(exit_nodes) exitList;
    for(int i=0; i< exit_nodes.count(); ++i)
    {
      exitList += exit_nodes.at(i);
    }

   QStringList strExitList;
   int exits_cnt = exitList.count();
   qDebug() << "exit list has " << exits_cnt << " items";
   for(int i=0; i<exits_cnt; ++i)
   {
     if((i+1) < exits_cnt)
     {
         if(exitList[i].trimmed().indexOf('.') > -1 ||
            exitList[i].trimmed().indexOf(':') > -1)
         {
             strExitList += (QString(exitList[i].
                             trimmed()+","));
         }
         else
         {
             strExitList += (QString("{" + countries_map[exitList[i]].
                             trimmed() + "},"));
         }
     }
     else
     {
         if(exitList[i].trimmed().indexOf('.') > -1 ||
            exitList[i].trimmed().indexOf(':') > -1)
         {
             strExitList += (QString(exitList[i].
                             trimmed()));
         }
         else
         {
             strExitList += (QString("{" + countries_map[exitList[i]].
                             trimmed() + "}"));
         }
     }
   }

   for(int i=0; i<configfile_lines.count(); ++i)
   {
       if(configfile_lines[i].trimmed().indexOf("ExitNodes") ==0 )
       {
          QString s;
          read_config_settings("ExitNodes",s);
          if(strExitList.count())
               configfile_lines[i] = "ExitNodes " ;
          else
               configfile_lines[i] = "" ;
          for(auto l : strExitList)
          {
            configfile_lines[i] += l;
          }

          if(strExitList.count()>i && configfile_lines[i].
                  indexOf(QChar('\n'))<0)
          {
             configfile_lines[i]+="\n";
          }
          strExitList.clear();
          break;
       }
   }
   QByteArray bytes;
   if(strExitList.count())
   {
       bytes.clear();
       bytes += "ExitNodes " ;
       for(auto l : strExitList)
       {
           bytes += l;
       }
       configfile_lines.append(bytes+"\n");
   }
}

/////////////////////////////////
//
//
//
//
////////////////////////////////
void tor_config_options::update_useguards(QString val, QString num)
{
    bool use_guards_update = false;

    for(int i=0; i<configfile_lines.count(); ++i)
    {
        if(configfile_lines[i].trimmed().indexOf("UseEntryGuards") == 0 )
        {
           configfile_lines[i] = "UseEntryGuards " + val;

           use_guards_update = true;
           break;
        }
    }
    QByteArray bytes;
    if(!use_guards_update)
    {
        bytes.clear();
        bytes += "UseEntryGuards " + val;
        configfile_lines.append(bytes+"\n");
    }
    qDebug() << "line added " << bytes;

    /////////////////////////////////
    //
    //
    //
    //
    ////////////////////////////////
    bool num_guards_update = false;
    for(int i=0; i<configfile_lines.count(); ++i)
    {
        if(configfile_lines[i].trimmed().indexOf("NumEntryGuards") == 0 )
        {
           configfile_lines[i] = "NumEntryGuards " + num ;

           num_guards_update = true;
           break;
        }
    }
    if(!num_guards_update)
    {
        bytes.clear();
        bytes += "NumEntryGuards " + num;
        configfile_lines.append(bytes+"\n");
    }
    qDebug() << "line added " << bytes;
}

///////////////
void tor_config_options::read_tor_config()
{
    bool result = true;

    std::shared_ptr<QFile> loadFile = std::make_shared<QFile>(filename);
    loadFile->setFileName(filename);

    if(loadFile->exists())
    {
        loadFile->copy(QString("backup_"+loadFile->fileName()));

        if (loadFile->open(QIODevice::ReadOnly))
        {
            QByteArray bytes;
            while (!loadFile->atEnd())
            {
                bytes = loadFile->readLine();
                configfile_lines.push_back(bytes.toStdString().c_str());
            }


            loadFile->close();
        }
        else
            result = false;
    }
    else
        result = false;
    if(!configfile_lines.count() || result == false)
    {
        qDebug() << "error reading config file";
    }
}
