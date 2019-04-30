#ifndef MAP_WIDGET_INTERFACE
#define MAP_WIDGET_INTERFACE

class QWidget;

class map_widget_interface
{
public:
    map_widget_interface() {}
    virtual ~map_widget_interface() {}

    virtual QWidget* map_dispaly_widget() = 0;
    virtual void map_enable_scibble(bool = false) = 0;
    virtual void map_disable_scibble() = 0;
};

#endif // MAP_WIDGET_INTERFACE

