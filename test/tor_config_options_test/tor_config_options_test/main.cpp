
#include <gtest/gtest.h>

#include <QApplication>
#include <QListWidget>
#include <QFile>
#include <QDebug>

#include "tor_config_options.h"
#include "tor_config_options_interface.h"

// The fixture for testing class tor_config_options.
class tor_config_options_test : public ::testing::Test {
 protected:

  tor_config_options_test() {
     // You can do set-up work for each test here.
  }

  ~tor_config_options_test() override {
     // You can do clean-up work that doesn't throw exceptions here.
  }

  // If the constructor and destructor are not enough for setting up
  // and cleaning up each test, you can define the following methods:

  void SetUp() override {
     // Code here will be called immediately after the constructor (right
     // before each test).
       QFile::copy(filename+".bak",filename);
  }

  void TearDown() override {
     // Code here will be called immediately after each test (right
     // before the destructor).
      QFile::remove(filename);
  }

  // Objects declared here can be used by all tests in the test suite for Foo.
  QString filename {"./torrc"};
  QString filename_multiline_test{"./multilinetorrc.bak"};
};


TEST_F(tor_config_options_test, SyncStrictNodesWithTorrc_Test)
{
    bool is_checked = false;
    {
        std::shared_ptr<tor_config_options_interface> tor_options =
            std::make_shared<tor_config_options>(filename);
        tor_options->sync_strictnodes_with_torrc(is_checked);
    }

    QFile loadFile(filename);
    if (!loadFile.open(QIODevice::ReadOnly))
    {
        qDebug() << "Couldn't open load file.";
        return;
    }
    QByteArray bytes;
    bytes = loadFile.readAll();
    loadFile.close();
    QString config_data(bytes);
    qDebug() << " config_data == " << config_data;
    EXPECT_EQ(is_checked,true);
}


TEST_F(tor_config_options_test, SyncNumGaurdsWithTorrc_Test)
{
    int n = 1;
    {
        std::shared_ptr<tor_config_options_interface> tor_options =
            std::make_shared<tor_config_options>(filename);
        tor_options->sync_num_gaurds_with_torrc(n);
    }

    QFile loadFile(filename);
    if (!loadFile.open(QIODevice::ReadOnly))
    {
        qDebug() << "Couldn't open load file.";
        return;
    }
    QByteArray bytes;
    bytes = loadFile.readAll();
    loadFile.close();
    QString config_data(bytes);
    qDebug() << " config_data == " << config_data;
    EXPECT_EQ(n,2);
}


TEST_F(tor_config_options_test, SyncUseGaurdsWithTorrc_Test)
{
    bool is_checked = false;
    {
        std::shared_ptr<tor_config_options_interface> tor_options =
            std::make_shared<tor_config_options>(filename);
        tor_options->sync_use_gaurds_with_torrc(is_checked);
    }

    QFile loadFile(filename);
    if (!loadFile.open(QIODevice::ReadOnly))
    {
        qDebug() << "Couldn't open load file.";
        return;
    }
    QByteArray bytes;
    bytes = loadFile.readAll();
    loadFile.close();
    QString config_data(bytes);
    qDebug() << " config_data == " << config_data;
    EXPECT_EQ(is_checked,true);
}


TEST_F(tor_config_options_test, SyncEnforceDistinctSubnetsWithTorrc_Test)
{
    bool is_checked = false;
    {
        std::shared_ptr<tor_config_options_interface> tor_options =
            std::make_shared<tor_config_options>(filename);
        tor_options->sync_enforce_distinct_subnets_with_torrc(is_checked);
    }

    QFile loadFile(filename);
    if (!loadFile.open(QIODevice::ReadOnly))
    {
        qDebug() << "Couldn't open load file.";
        return;
    }
    QByteArray bytes;
    bytes = loadFile.readAll();
    loadFile.close();
    QString config_data(bytes);
    qDebug() << " config_data == " << config_data;
    EXPECT_EQ(is_checked,true);
}

TEST_F(tor_config_options_test, PopulateConfigSettingsListWidgetTest)
{

        std::shared_ptr<tor_config_options_interface> tor_options =
                std::make_shared<tor_config_options>(filename);

        std::shared_ptr<QListWidget> list_widget =
                std::make_shared<QListWidget>();

        std::shared_ptr<QListWidget> countrylist_widget =
                std::make_shared<QListWidget>();

        tor_options->populate_country_list(QString("ExcludeNodes"),
                              list_widget.get(),countrylist_widget.get());
        QString str;
        for(int index=0; index<list_widget->count(); ++index)
        {
           str += list_widget->item(index)->text();
           str +=",";
        }

        qDebug() << "Items returned are " << str;
        QString expected = "Afghanistan,Canada,Cayman Islands,Lithuania,United States,";
        ASSERT_STREQ(expected.trimmed().toStdString().c_str(),
                     str.trimmed().toStdString().c_str());

}

TEST_F(tor_config_options_test, GetExcludeNodesConfigSettingsDataTest)
{
    {
        std::shared_ptr<tor_config_options_interface> tor_options =
                std::make_shared<tor_config_options>(filename_multiline_test);

        QString settingsData;
        tor_options->read_config_settings(QString("ExcludeNodes"),settingsData);

        QString expected = "ExcludeNodes {AF},{KY},{US},{CA},{LT}\n";
        qDebug() << settingsData << " == " << expected;

        ASSERT_STREQ(expected.trimmed().toStdString().c_str(),
                     settingsData.trimmed().toStdString().c_str());
    }
}

// Tests that update_entrynodes(list_widget) updates torrc
TEST_F(tor_config_options_test, UpdateEntryNodesTest) {

    {
        {
            std::shared_ptr<tor_config_options_interface> tor_options =
                    std::make_shared<tor_config_options>(filename);

            std::shared_ptr<QListWidget> list_widget =
                    std::make_shared<QListWidget>();

            list_widget->addItem(QString("Denmark"));
            list_widget->addItem(QString("French Polynesia"));
            tor_options->update_entrynodes(list_widget.get());
            bool b;
            tor_options->save_to_configfile(b);
        }
        QFile loadFile(filename);
        if (!loadFile.open(QIODevice::ReadOnly))
        {
            qDebug() << "Couldn't open load file.";
           return;
        }
        QByteArray bytes;
        bytes = loadFile.readAll();
        loadFile.close();
        QString config_data(bytes);
        qDebug() << " config_data == " << config_data;
        EXPECT_GT(config_data.indexOf("EntryNodes {DK},{PF}"), -1);
    }
    {
        {
            std::shared_ptr<tor_config_options_interface> tor_options =
                    std::make_shared<tor_config_options>(filename);
            std::shared_ptr<QListWidget> list_widget =
                    std::make_shared<QListWidget>();

            list_widget->addItem(QString("French Guiana"));
            tor_options->update_entrynodes(list_widget.get());
            bool b;
            tor_options->save_to_configfile(b);

        }
        QFile loadFile2(filename);
        if (!loadFile2.open(QIODevice::ReadOnly))
        {
            qDebug() << "Couldn't open load file.";
            return;
        }
        QByteArray bytes = loadFile2.readAll();
        loadFile2.close();
        QString config_data = bytes;
        qDebug() << " config_data == " << config_data;
        EXPECT_GT(config_data.indexOf("EntryNodes {GF}"), -1);
    }
}

// Tests that update_exitnodes(list_widget) updates torrc
TEST_F(tor_config_options_test, UpdateExitNodesTest) {

    {
        {
            std::shared_ptr<tor_config_options_interface> tor_options =
                    std::make_shared<tor_config_options>(filename);

            std::shared_ptr<QListWidget> list_widget =
                    std::make_shared<QListWidget>();

            list_widget->addItem(QString("Denmark"));
            list_widget->addItem(QString("French Polynesia"));
            tor_options->update_exitnodes(list_widget.get());
            bool b;
            tor_options->save_to_configfile(b);
        }
        QFile loadFile(filename);
        if (!loadFile.open(QIODevice::ReadOnly))
        {
            qDebug() << "Couldn't open load file.";
            exit(1);
        }
        QByteArray bytes;
        bytes = loadFile.readAll();
        loadFile.close();
        QString config_data(bytes);
        qDebug() << " config_data == " << config_data;
        EXPECT_GT(config_data.indexOf("ExitNodes {DK},{PF}"), -1);
    }
    {
        {
            std::shared_ptr<tor_config_options_interface> tor_options =
                    std::make_shared<tor_config_options>(filename);
            std::shared_ptr<QListWidget> list_widget =
                    std::make_shared<QListWidget>();

            list_widget->addItem(QString("French Guiana"));
            tor_options->update_exitnodes(list_widget.get());
            bool b;
            tor_options->save_to_configfile(b);
        }
        QFile loadFile2(filename);
        if (!loadFile2.open(QIODevice::ReadWrite))
        {
            qDebug() << "Couldn't open load file.";
            exit(1);
        }
        QByteArray bytes = loadFile2.readAll();
        loadFile2.close();
        QString config_data = bytes;
        qDebug() << " config_data == " << config_data;
        EXPECT_GT(config_data.indexOf("ExitNodes {GF}"), -1);
    }
}

// Tests that update_excludenodes(list_widget) updates torrc
TEST_F(tor_config_options_test, UpdateExcludeNodesTest) {

    {
        {
            std::shared_ptr<tor_config_options_interface> tor_options =
                    std::make_shared<tor_config_options>(filename);

            std::shared_ptr<QListWidget> list_widget =
                    std::make_shared<QListWidget>();

            list_widget->addItem(QString("Afghanistan"));
            list_widget->addItem(QString("Cayman Islands"));
            tor_options->update_excludenodes(list_widget.get());
            bool b;
            tor_options->save_to_configfile(b);
        }
        QFile loadFile(filename);
        if (!loadFile.open(QIODevice::ReadWrite))
        {
            qDebug() << "Couldn't open load file.";
            exit(1);
        }
        QByteArray bytes;
        bytes = loadFile.readAll();
        loadFile.close();
        QString config_data(bytes);
        qDebug() << " config_data == " << config_data;
        EXPECT_GT(config_data.indexOf("ExcludeNodes {AF},{KY}"), -1);
    }
    {
        {
            std::shared_ptr<tor_config_options_interface> tor_options =
                    std::make_shared<tor_config_options>(filename);
            std::shared_ptr<QListWidget> list_widget =
                    std::make_shared<QListWidget>();

            list_widget->addItem(QString("Cayman Islands"));
            tor_options->update_excludenodes(list_widget.get());
            bool b;
            tor_options->save_to_configfile(b);
        }
        QFile loadFile2(filename);
        if (!loadFile2.open(QIODevice::ReadOnly))
        {
            qDebug() << "Couldn't open load file.";
            exit(1);
        }
        QByteArray bytes = loadFile2.readAll();
        loadFile2.close();
        QString config_data = bytes;
        qDebug() << " config_data == " << config_data;
        EXPECT_GT(config_data.indexOf("ExcludeNodes {KY}"), -1);
    }
}

// Tests update_EnforceDistinctSubnets(QString("1")) updates torrc
// Tests update_EnforceDistinctSubnets(QString("0")) updates torrc
TEST_F(tor_config_options_test, EnforceDistinctSubnetsUpdateTest) {
    {
        {
            std::shared_ptr<tor_config_options_interface> tor_options =
                    std::make_shared<tor_config_options>(filename);
            tor_options->update_EnforceDistinctSubnets(QString("1"));
            bool b;
            tor_options->save_to_configfile(b);
        }
        QFile loadFile(filename);
        if (!loadFile.open(QIODevice::ReadOnly))
        {
            qDebug() << "Couldn't open load file.";
            exit(1);
        }
        QByteArray bytes;
        bytes = loadFile.readAll();
        loadFile.close();
        QString config_data(bytes);
        qDebug() << " config_data == " << bytes;
        EXPECT_GT(config_data.indexOf("EnforceDistinctSubnets 1"), -1);
    }
    {
        {
            std::shared_ptr<tor_config_options_interface> tor_options =
                    std::make_shared<tor_config_options>(filename);
            tor_options->update_EnforceDistinctSubnets(QString("0"));
            bool b;
            tor_options->save_to_configfile(b);
        }
        QFile loadFile2(filename);
        if (!loadFile2.open(QIODevice::ReadOnly))
        {
            qDebug() << "Couldn't open load file.";
            exit(1);
        }
        QByteArray bytes = loadFile2.readAll();
        loadFile2.close();
        QString config_data = bytes;
        qDebug() << " config_data == " << config_data;
        EXPECT_GT(config_data.indexOf("EnforceDistinctSubnets 0"), -1);
    }
}



// Tests that update_useguards(QString("1"),QString("3")) updates torrc
TEST_F(tor_config_options_test, EntryGuardsNumGuardsUpdateTest) {
    {
        {
            std::shared_ptr<tor_config_options_interface> tor_options =
                    std::make_shared<tor_config_options>(filename);
            tor_options->update_useguards(QString("1"),QString("3"));
            bool b;
            tor_options->save_to_configfile(b);
        }
        QFile loadFile(filename);
        if (!loadFile.open(QIODevice::ReadWrite))
        {
            qDebug() << "Couldn't open load file.";
            exit(1);
        }
        QByteArray bytes;
        bytes = loadFile.readAll();
        loadFile.close();
        QString config_data(bytes);
        qDebug() << " config_data == " << bytes;
        EXPECT_GT(config_data.indexOf("UseEntryGuards 1"), -1);
        EXPECT_GT(config_data.indexOf("NumEntryGuards 3"), -1);
    }
    {
        {
            std::shared_ptr<tor_config_options_interface> tor_options =
                    std::make_shared<tor_config_options>(filename);
            tor_options->update_useguards(QString("0"),QString("6"));
            bool b;
            tor_options->save_to_configfile(b);
        }
        QFile loadFile2(filename);
        if (!loadFile2.open(QIODevice::ReadWrite))
        {
            qDebug() << "Couldn't open load file.";
            exit(1);
        }
        QByteArray bytes = loadFile2.readAll();
        loadFile2.close();
        QString config_data = bytes;
        qDebug() << " config_data == " << config_data;
        EXPECT_GT(config_data.indexOf("UseEntryGuards 0"), -1);
        EXPECT_GT(config_data.indexOf("NumEntryGuards 6"), -1);
    }
}


// Tests that update_strictnodes(QString("1")) updates torrc
TEST_F(tor_config_options_test, StrictNodesUpdateTest) {

    {
        {
            std::shared_ptr<tor_config_options_interface> tor_options =
                    std::make_shared<tor_config_options>(filename);
            tor_options->update_strictnodes(QString("1"));
            bool b;
            tor_options->save_to_configfile(b);
        }
        QFile loadFile(filename);
        if (!loadFile.open(QIODevice::ReadWrite))
        {
            qDebug() << "Couldn't open load file.";
            exit(1);
        }
        QByteArray bytes;
        bytes = loadFile.readAll();
        loadFile.close();
        QString config_data(bytes);
        qDebug() << " config_data == " << config_data;
        EXPECT_GT(config_data.indexOf("StrictNodes 1"), -1);
    }
    {
        {
            std::shared_ptr<tor_config_options_interface> tor_options =
                    std::make_shared<tor_config_options>(filename);
            tor_options->update_strictnodes(QString("0"));
            bool b;
            tor_options->save_to_configfile(b);
        }
        QFile loadFile2(filename);
        if (!loadFile2.open(QIODevice::ReadWrite))
        {
            qDebug() << "Couldn't open load file.";
            exit(1);
        }
        QByteArray bytes = loadFile2.readAll();
        loadFile2.close();
        QString config_data = bytes;
        qDebug() << " config_data == " << config_data;
        EXPECT_GT(config_data.indexOf("StrictNodes 0"), -1);
    }

}

int main(int argc, char *argv[])
{
    QApplication a(argc,argv);
    ::testing::InitGoogleTest(&argc, argv);   
    return RUN_ALL_TESTS();
}
