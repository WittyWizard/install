#-------------------------------------------------
#
# Witty Wizard Project 16 Sep 2014
#
# run --docroot . --http-address 0.0.0.0 --http-port 8088
# --gdb --docroot "./doc_root" --approot "./app_root" --deploy-path /ww --http-address 0.0.0.0 --http-port 8088
# sudo /usr/bin/qtcreator
#-------------------------------------------------

QT       += core
QT       -= gui
QT       += sql

TARGET = WittyWizard.wt
CONFIG   += console
CONFIG   -= app_bundle
TEMPLATE = app

LIBS += -L/usr/lib -L/usr/local/lib -I/usr/local/include -lwt -lwthttp -lwtdbo -lcrypt -ltiff -lpng
LIBS += -lboost_signals -lboost_filesystem -lboost_system -lboost_regex -lboost_date_time -lboost_thread

SOURCES += main.cpp WittyWizard.cpp WidgetFunction.cpp WittyWizardPlugins.cpp CrystalBall.cpp \
    model/HitCounterMan.cpp model/HitCounterManSession.cpp \
    view/HitCounterManImpl.cpp view/HitCounterManView.cpp \
    model/MenuMan.cpp model/MenuManSession.cpp \
    view/MenuManImpl.cpp view/MenuManView.cpp \

HEADERS += WittyWizard.h WidgetFunction.h WittyWizardPlugins.h CrystalBall.h \
    model/HitCounterMan.h model/HitCounterManSession.h \
    view/HitCounterManImpl.h view/HitCounterManView.h \
    model/MenuMan.h model/MenuManSession.h \
    view/MenuManImpl.h view/MenuManView.h \


# PostgreSql
DEFINES += POSTGRES
LIBS += -lwtdbopostgres

# Sqlite3
#DEFINES += SQLITE3
#LIBS += -lwtdbosqlite3

# MySQL
# svn co https://wtdbomysql.svn.sourceforge.net/svnroot/wtdbomysql wtdbomysql
#DEFINES += MYSQL
#LIBS += -lwtdbomysql

# Firebird
#DEFINES += FIREBIRD
#LIBS += -lwtdbofirebird

# Debug mode
DEFINES += NDEBUG
QMAKE_CXXFLAGS += -DNDEBUG
# C++11 compatable compile modes
#QMAKE_CXXFLAGS += "-std=c++0x"
#CXXFLAGS="-std=c++0x"
QMAKE_CXXFLAGS += "-std=c++11"
CXXFLAGS="-std=c++11"
# Theme
DEFINES += THEME3
# Hit Counter Manager
DEFINES += HITCOUNTERMAN
# ### End Of File #############################################################

OTHER_FILES += \
    ../deploy/appRoot/home/greywizzard.com/WittyWizard.xml \
    ../deploy/appRoot/home/greywizzard.com/WittyWizard_ru.xml \
    ../deploy/appRoot/home/greywizzard.com/WittyWizard_cn.xml \
    ../deploy/appRoot/home/greywizzard.com/videoman/videoman.xml \
    ../deploy/appRoot/home/thedarkwizzard.com/WittyWizard.xml \
    ../deploy/appRoot/home/thedarkwizzard.com/WittyWizard_ru.xml \
    ../deploy/appRoot/home/thedarkwizzard.com/WittyWizard_cn.xml \
    ../deploy/appRoot/home/thedarkwizzard.com/videoman/videoman.xml \
    ../deploy/appRoot/home/wittywizard.org/WittyWizard.xml \
    ../deploy/appRoot/home/wittywizard.org/WittyWizard_ru.xml \
    ../deploy/appRoot/home/wittywizard.org/WittyWizard_cn.xml \
    ../deploy/appRoot/home/wittywizard.org/videoman/videoman.xml \
    Notes/git-update.sh \
    Notes/ubuntu-install.txt \
    Notes/More-Notes.txt \
    Notes/README.md \
    ../build-WittyWizard-Desktop-Debug/app_root/domains.xml \
    ../build-WittyWizard-Desktop-Debug/app_root/home/localhost/WittyWizard.xml \
    ../build-WittyWizard-Desktop-Debug/app_root/home/localhost/WittyWizard_ru.xml \
    ../build-WittyWizard-Desktop-Debug/app_root/home/localhost/WittyWizard_cn.xml \
    ../deploy/appRoot/home/localhost/WittyWizard.xml \
    ../deploy/appRoot/home/localhost/WittyWizard_ru.xml \
    ../deploy/appRoot/home/localhost/WittyWizard_cn.xml \
    ../deploy/appRoot/home/localhost/videoman/videoman.xml \
    ../deploy/appRoot/home/localhost/videoman/video.xml \
    ../build-WittyWizard-Desktop-Debug/app_root/home/localhost/videoman/videoman.xml \
    ../deploy/appRoot/home/greywizzard.com/videoman/videoman-db-import.xml \
    ../deploy/appRoot/home/greywizzard.com/videoman/videoman-xml-import.xml \
    ../deploy/appRoot/home/greywizzard.com/videoman/videoman.xml \
    ../deploy/appRoot/home/thedarkwizzard.com/videoman/videoman-db-import.xml \
    ../deploy/appRoot/home/thedarkwizzard.com/videoman/videoman-xml-import.xml \
    ../deploy/appRoot/home/thedarkwizzard.com/videoman/videoman.xml \
    ../deploy/appRoot/home/wittywizard.org/videoman/videoman.xml \
    ../deploy/appRoot/home/wittywizard.org/videoman/videoman-xml-import.xml \
    ../deploy/appRoot/home/wittywizard.org/videoman/videoman-db-import.xml \
    ../deploy/appRoot/home/mad-news.net/videoman/videoman-db-import.xml \
    ../deploy/appRoot/home/mad-news.net/videoman/videoman-xml-import.xml \
    ../deploy/appRoot/home/mad-news.net/videoman/videoman.xml \
    ../deploy/appRoot/home/lightwizzard.com/videoman/video.xml \
    ../deploy/appRoot/home/lightwizzard.com/videoman/videoman-db-import.xml \
    ../deploy/appRoot/home/lightwizzard.com/videoman/videoman-xml-import.xml \
    ../build-WittyWizard-Desktop-Debug/doc_root/resources/themes/wittywizard/blue/ww-blue.css \
    ../build-WittyWizard-Desktop-Debug/doc_root/resources/themes/wittywizard/default/ww-default.css \
    ../build-WittyWizard-Desktop-Debug/doc_root/resources/themes/wittywizard/green/ww-green.css \
    ../build-WittyWizard-Desktop-Debug/doc_root/resources/themes/wittywizard/red/ww-red.css \
    ../build-WittyWizard-Desktop-Debug/doc_root/resources/themes/wittywizard/tan/ww-tan.css \
    ../build-WittyWizard-Desktop-Debug/doc_root/resources/themes/wittywizard/white/ww-white.css \
    ../build-WittyWizard-Desktop-Debug/doc_root/resources/css/wittywizard.css \
    ../deploy/appRoot/home/lightwizzard.com/WittyWizard.xml \
    ../deploy/appRoot/home/lightwizzard.com/WittyWizard_ru.xml \
    ../deploy/appRoot/home/lightwizzard.com/WittyWizard_cn.xml \
    ../deploy/appRoot/home/mad-news.net/WittyWizard.xml \
    ../deploy/appRoot/home/mad-news.net/WittyWizard_ru.xml \
    ../deploy/appRoot/home/mad-news.net/WittyWizard_cn.xml \
    ../../../../../../../etc/wt/wt_config.xml \
    ../deploy/appRoot/home/greywizzard.com/menuman/menuman-xml-import.xml \
    ../deploy/appRoot/home/greywizzard.com/menuman/menuman-db-import.xml \
    ../deploy/appRoot/home/lightwizzard.com/menuman/menuman-xml-import.xml \
    ../deploy/appRoot/home/lightwizzard.com/menuman/menuman-db-import.xml \
    ../deploy/appRoot/home/localhost/menuman/menuman-xml-import.xml \
    ../deploy/appRoot/home/localhost/menuman/menuman-db-import.xml \
    ../deploy/appRoot/home/mad-news.net/menuman/menuman-db-import.xml \
    ../deploy/appRoot/home/mad-news.net/menuman/menuman-xml-import.xml \
    ../deploy/appRoot/home/thedarkwizzard.com/menuman/menuman-xml-import.xml \
    ../deploy/appRoot/home/thedarkwizzard.com/menuman/menuman-db-import.xml \
    ../deploy/appRoot/home/wittywizard.org/menuman/menuman-xml-import.xml \
    ../deploy/appRoot/home/wittywizard.org/menuman/menuman-db-import.xml \
    ../deploy/appRoot/home/lightwizzard.com/pages/about.html \
    ../deploy/appRoot/home/lightwizzard.com/pages/home.html \
    ../build-WittyWizard-Desktop-Debug/app_root/home/localhost/blog.xml \
    ../build-WittyWizard-Desktop-Debug/doc_root/resources/themes/wittywizard/blue/SfMenuHoriz.css \
    ../build-WittyWizard-Desktop-Debug/doc_root/resources/themes/wittywizard/green/SfMenuHoriz.css \
    ../build-WittyWizard-Desktop-Debug/doc_root/resources/themes/wittywizard/red/SfMenuHoriz.css \
    ../build-WittyWizard-Desktop-Debug/doc_root/resources/themes/wittywizard/tan/SfMenuHoriz.css \
    ../build-WittyWizard-Desktop-Debug/doc_root/resources/themes/wittywizard/white/SfMenuHoriz.css \
    ../build-WittyWizard-Desktop-Debug/doc_root/resources/themes/wittywizard/default/SfMenuHoriz.css \
    ../build-WittyWizard-Desktop-Debug/doc_root/resources/themes/wittywizard/red/SfVerticalLeft.css \
    ../build-WittyWizard-Desktop-Debug/doc_root/resources/themes/wittywizard/red/SfMenuVertLeft.css \
    ../build-WittyWizard-Desktop-Debug/doc_root/resources/themes/wittywizard/red/SfHoriz.css \
    ../build-WittyWizard-Desktop-Debug/doc_root/resources/themes/wittywizard/red/SfMenuHoriz-old.css \
    ../build-WittyWizard-Desktop-Debug/doc_root/resources/themes/wittywizard/white/SfMenuHoriz-old.css \
    ../build-WittyWizard-Desktop-Debug/app_root/home/localhost/videoman/db/videoman-db-import.xml \
    ../build-WittyWizard-Desktop-Debug/app_root/home/localhost/menuman/db/menuman-db-import.xml \
    ../deploy/appRoot/home/greywizzard.com/menuman/db/menuman-db-import.xml \
    ../deploy/appRoot/home/greywizzard.com/videoman/db/videoman-db-import.xml \
    ../deploy/appRoot/home/lightwizzard.com/videoman/db/videoman-db-import.xml \
    ../deploy/appRoot/home/lightwizzard.com/videoman/xml/videoman-xml-import.xml \
    ../deploy/appRoot/home/lightwizzard.com/menuman/db/menuman-db-import.xml \
    ../deploy/appRoot/home/lightwizzard.com/menuman/xml/menuman-xml-import.xml \
    ../deploy/appRoot/home/greywizzard.com/menuman/xml/menuman-xml-import.xml \
    ../deploy/appRoot/home/mad-news.net/menuman/db/menuman-db-import.xml \
    ../deploy/appRoot/home/mad-news.net/menuman/xml/menuman-xml-import.xml \
    ../deploy/appRoot/home/mad-news.net/videoman/db/videoman-db-import.xml \
    ../deploy/appRoot/home/mad-news.net/videoman/xml/videoman-xml-import.xml \
    ../deploy/appRoot/home/thedarkwizzard.com/videoman/db/videoman-db-import.xml \
    ../deploy/appRoot/home/thedarkwizzard.com/videoman/xml/videoman-xml-import.xml \
    ../deploy/appRoot/home/wittywizard.org/videoman/db/videoman-db-import.xml \
    ../deploy/appRoot/home/wittywizard.org/videoman/xml/videoman-xml-import.xml \
    ../deploy/appRoot/home/wittywizard.org/menuman/db/menuman-db-import.xml \
    ../deploy/appRoot/home/wittywizard.org/menuman/xml/menuman-xml-import.xml \
    ../deploy/appRoot/home/thedarkwizzard.com/menuman/db/menuman-db-import.xml \
    ../deploy/appRoot/home/thedarkwizzard.com/menuman/xml/menuman-xml-import.xml \
    ../deploy/appRoot/home/greywizzard.com/videoman/xml/videoman-xml-import.xml \
    ../build-WittyWizard-Desktop-Debug/app_root/home/localhost/videoman/xml/videoman-xml-import.xml \
    ../deploy/appRoot/home/wittywizard.org/pages/witty-wizard-00-00.html \
    ../deploy/appRoot/home/lightwizzard.com/pages/about_cn.html \
    ../deploy/appRoot/home/lightwizzard.com/pages/about_de.html \
    ../deploy/appRoot/home/lightwizzard.com/pages/about_ru.html \
    ../deploy/appRoot/home/lightwizzard.com/pages/home_cn.html \
    ../deploy/appRoot/home/lightwizzard.com/pages/home_de.html \
    ../deploy/appRoot/home/lightwizzard.com/pages/home_ru.html \
    ../deploy/appRoot/home/lightwizzard.com/WittyWizard_de.xml \
    ../deploy/appRoot/home/wittywizard.org/WittyWizard_de.xml \
    ../deploy/appRoot/home/wittywizard.org/pages/About.html \
    ../deploy/appRoot/home/wittywizard.org/pages/About_cn.html \
    ../deploy/appRoot/home/wittywizard.org/pages/About_de.html \
    ../deploy/appRoot/home/wittywizard.org/pages/About_ru.html \
    ../deploy/appRoot/home/wittywizard.org/pages/Home_cn.html \
    ../deploy/appRoot/home/wittywizard.org/pages/Home_de.html \
    ../deploy/appRoot/home/wittywizard.org/pages/Home_ru.html \
    ../deploy/appRoot/home/wittywizard.org/pages/Features-Menu.html \
    ../deploy/appRoot/home/wittywizard.org/pages/Features-Menu_cn.html \
    ../deploy/appRoot/home/wittywizard.org/pages/Features-Menu_de.html \
    ../deploy/appRoot/home/wittywizard.org/pages/Features-Menu_ru.html \
    ../deploy/appRoot/home/wittywizard.org/pages/Home.html \
    ../deploy/appRoot/home/lightwizzard.com/videoman/db/videoman-db-import_ru.xml \
    ../deploy/appRoot/home/lightwizzard.com/videoman/db/videoman-db-import_cn.xml \
    ../deploy/appRoot/home/lightwizzard.com/videoman/db/videoman-db-import_de.xml \
    ../build-WittyWizard-Desktop-Debug/app_root/home/localhost/WittyWizard_de.xml \
    ../build-WittyWizard-Desktop-Debug/app_root/home/localhost/menuman/db/menuman-db-import_cn.xml \
    ../build-WittyWizard-Desktop-Debug/app_root/home/localhost/menuman/db/menuman-db-import_de.xml \
    ../build-WittyWizard-Desktop-Debug/app_root/home/localhost/menuman/db/menuman-db-import_ru.xml \
    ../build-WittyWizard-Desktop-Debug/app_root/home/localhost/menuman/xml/00-00-G-IAM.xml \
    ../build-WittyWizard-Desktop-Debug/app_root/home/localhost/menuman/xml/00-01-G-IAM.xml \
    ../build-WittyWizard-Desktop-Debug/app_root/home/localhost/menuman/xml/00-01-N-IAM.xml \
    ../build-WittyWizard-Desktop-Debug/app_root/home/localhost/menuman/xml/00-01-P-IAM.xml \
    ../build-WittyWizard-Desktop-Debug/app_root/home/localhost/menuman/xml/00-02-G-IAM.xml \
    ../build-WittyWizard-Desktop-Debug/app_root/home/localhost/menuman/xml/00-02-N-IAM.xml \
    ../build-WittyWizard-Desktop-Debug/app_root/home/localhost/menuman/xml/00-02-P-IAM.xml \
    ../build-WittyWizard-Desktop-Debug/app_root/home/localhost/menuman/xml/00-03-G-IAM.xml \
    ../build-WittyWizard-Desktop-Debug/app_root/home/localhost/menuman/xml/00-03-N-IAM.xml \
    ../build-WittyWizard-Desktop-Debug/app_root/home/localhost/menuman/xml/00-03-P-IAM.xml \
    ../build-WittyWizard-Desktop-Debug/app_root/home/localhost/menuman/xml/00-04-G-IAM.xml \
    ../build-WittyWizard-Desktop-Debug/app_root/home/localhost/menuman/xml/00-05-G-IAM.xml \
    ../build-WittyWizard-Desktop-Debug/app_root/home/localhost/menuman/xml/01-00-G-Emotions.xml \
    ../build-WittyWizard-Desktop-Debug/app_root/home/localhost/menuman/xml/01-01-G-Emotions.xml \
    ../build-WittyWizard-Desktop-Debug/app_root/home/localhost/menuman/xml/02-00-G-Universe.xml \
    ../build-WittyWizard-Desktop-Debug/app_root/home/localhost/menuman/xml/02-01-G-Universe.xml \
    ../build-WittyWizard-Desktop-Debug/app_root/home/localhost/menuman/xml/03-00-G-Dimensions.xml \
    ../build-WittyWizard-Desktop-Debug/app_root/home/localhost/menuman/xml/04-00-G-Game.xml \
    ../build-WittyWizard-Desktop-Debug/app_root/home/localhost/menuman/xml/05-00-G-Road-Map.xml \
    ../build-WittyWizard-Desktop-Debug/app_root/home/localhost/menuman/xml/06-00-G-Rant.xml \
    ../build-WittyWizard-Desktop-Debug/app_root/home/localhost/menuman/xml/06-0.1-G-Rant-Child-Murder.xml \
    ../build-WittyWizard-Desktop-Debug/app_root/home/localhost/menuman/xml/07-00-G-Science.xml \
    ../build-WittyWizard-Desktop-Debug/app_root/home/localhost/menuman/xml/07-01-G-Science.xml \
    ../build-WittyWizard-Desktop-Debug/app_root/home/localhost/menuman/xml/08-00-G-Sanctuary.xml \
    ../build-WittyWizard-Desktop-Debug/app_root/home/localhost/menuman/xml/09-00-G-Trinary-Energy.xml \
    ../build-WittyWizard-Desktop-Debug/app_root/home/localhost/menuman/xml/10-01-G-HisStory.xml \
    ../build-WittyWizard-Desktop-Debug/app_root/home/localhost/menuman/xml/11-00-G-Society.xml \
    ../build-WittyWizard-Desktop-Debug/app_root/home/localhost/menuman/xml/12-00-G-JustUs.xml \
    ../build-WittyWizard-Desktop-Debug/app_root/home/localhost/menuman/xml/about_cn.xml \
    ../build-WittyWizard-Desktop-Debug/app_root/home/localhost/menuman/xml/about_de.xml \
    ../build-WittyWizard-Desktop-Debug/app_root/home/localhost/menuman/xml/about_ru.xml \
    ../build-WittyWizard-Desktop-Debug/app_root/home/localhost/menuman/xml/about.xml \
    ../build-WittyWizard-Desktop-Debug/app_root/home/localhost/menuman/xml/home_cn.xml \
    ../build-WittyWizard-Desktop-Debug/app_root/home/localhost/menuman/xml/home_de.xml \
    ../build-WittyWizard-Desktop-Debug/app_root/home/localhost/menuman/xml/home_ru.xml \
    ../build-WittyWizard-Desktop-Debug/app_root/home/localhost/menuman/xml/home.xml

