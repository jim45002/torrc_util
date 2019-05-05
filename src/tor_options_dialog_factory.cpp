
#include <QWidget>

#include "tor_options_dialog.h"
#include "tor_options_dialog_interface.h"
#include "tor_options_dialog_factory.h"

tor_options_dialog_factory::tor_options_dialog_factory()
{

}

tor_options_dialog_factory::~tor_options_dialog_factory()
{

}

void tor_options_dialog_factory::create_tor_config_options_dialog(
        std::shared_ptr<TorOptionsDialogInterface> &sptr,
        QWidget* parent)
{
    sptr = std::make_shared<TorOptionsDialog>(parent);
}
