
#include "tor_config_options.h"
#include "tor_options_factory.h"


TorOptionsFactory::TorOptionsFactory()
{

}

TorOptionsFactory::~TorOptionsFactory() { }

void TorOptionsFactory::create_tor_config_options(
        std::shared_ptr<tor_config_options_interface> &sptr,
        QString configfilename,
        QObject *parent)
{
    sptr = std::make_shared<tor_config_options>
            (configfilename,parent);
}
