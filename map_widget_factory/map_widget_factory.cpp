
#include <memory>
#include <mutex>

#include "map_widget.h"
#include "map_widget_factory.h"
#include "map_widget_interface.h"

//std::shared_ptr<map_widget_factory> map_widget_factory::instance = nullptr;
std::shared_ptr<std::mutex> map_widget_factory::multiple_create_call_mutex = std::shared_ptr<std::mutex>();
//std::shared_ptr<std::mutex> map_widget_factory::multiple_getInstance_call_mutex = std::shared_ptr<std::mutex>();

map_widget_factory::map_widget_factory()

{
}

std::shared_ptr<map_widget_interface> map_widget_factory::create(QWidget* parent)
{
  //  std::lock_guard<std::mutex> lock(*multiple_create_call_mutex);
    return std::make_shared<map_widget>(parent);
}

//std::shared_ptr<map_widget_factory> map_widget_factory::getInstance()
//{
//    std::lock_guard<std::mutex> lock(*multiple_getInstance_call_mutex);
//    if (!instance)
//    {
//        instance = std::make_shared<map_widget_factory>();
//    }
//    return instance;
//}

