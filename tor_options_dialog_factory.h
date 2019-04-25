#ifndef TOR_OPTIONS_DIALOG_FACTORY_H
#define TOR_OPTIONS_DIALOG_FACTORY_H

#include <memory>

#include <QObject>

class QWidget;
class TorOptionsDialogInterface;

class tor_options_dialog_factory : public QObject
{
public:
    tor_options_dialog_factory();
    virtual ~tor_options_dialog_factory();

    static void create_tor_config_options_dialog(
            std::shared_ptr<TorOptionsDialogInterface>& sptr,
            QWidget *parent);
};

#endif // TOR_OPTIONS_DIALOG_FACTORY_H
