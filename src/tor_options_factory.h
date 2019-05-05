#ifndef TOR_OPTIONS_FACTORY_H
#define TOR_OPTIONS_FACTORY_H

#include <memory>

#include <QString>
#include <QObject>

class tor_config_options_interface;

class TorOptionsFactory : public QObject
{
public:
    TorOptionsFactory();
    virtual ~TorOptionsFactory();
    static void
    create_tor_config_options(
            std::shared_ptr<tor_config_options_interface>& sptr,
            QString configfilename,
            QObject *parent);
};

#endif // TOR_OPTIONS_FACTORY_H
