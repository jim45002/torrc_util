#include <iostream>
#include <memory>

#include <gtest/gtest.h>

#include <QCoreApplication>
#include <QObject>
#include <QDebug>

#include "node_lookup_interface.h"
#include "node_lookup.h"
#include "test_node_lookup.h"

// The fixture for testing class tor_config_options.
class tor_node_lookup_test : public ::testing::Test {
 protected:

  tor_node_lookup_test() {
  }

  ~tor_node_lookup_test() override {
  }

  void SetUp() override {

  }

  void TearDown() override {

  }

};


////////////////////////////////////
TEST_F(tor_node_lookup_test, DownloadLatestTorNodeListTest)
{
    std::shared_ptr<node_lookup_interface> nlup_ptr =
            std::make_shared<node_lookup>();

    std::shared_ptr<test_node_lookup> tnlup_ptr =
            std::make_shared<test_node_lookup>();

    QObject::connect(tnlup_ptr.get(),
                    SIGNAL(request_node_list(QString,bool)),
                    nlup_ptr.get(),
                    SLOT(get_node_list(QString,bool)));

    QObject::connect(nlup_ptr.get(),
                    SIGNAL(send_node_list(QString,QStringList)),
                    tnlup_ptr.get(),
                    SLOT(recv_node_list(QString,QStringList)));

    QObject::connect(tnlup_ptr.get(),
                    SIGNAL(request_download_latest_node_list()),
                    nlup_ptr.get(),
                    SLOT(download_nodelist()));

    QObject::connect(nlup_ptr.get(),
                    SIGNAL(send_download_result(bool)),
                    tnlup_ptr.get(),
                    SLOT(recv_download_result(bool)));

    tnlup_ptr->download_node_list();

    EXPECT_FALSE(tnlup_ptr->get_latest_nodelist_download());
}


/////////////////////////////////////
TEST_F(tor_node_lookup_test, GetTorNodeListTest)
{
    std::shared_ptr<node_lookup_interface> nlup_ptr =
            std::make_shared<node_lookup>();

    std::shared_ptr<test_node_lookup> tnlup_ptr =
            std::make_shared<test_node_lookup>();

    QObject::connect(tnlup_ptr.get(),
                    SIGNAL(request_node_list(QString,bool)),
                    nlup_ptr.get(),
                    SLOT(get_node_list(QString,bool)));

    QObject::connect(nlup_ptr.get(),
                    SIGNAL(send_node_list(QString,QStringList)),
                    tnlup_ptr.get(),
                    SLOT(recv_node_list(QString,QStringList)));

    tnlup_ptr->get_node_list("US");

    EXPECT_FALSE(tnlup_ptr->getNodes().isEmpty());

}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc,argv);
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
