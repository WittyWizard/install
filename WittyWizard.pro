#-------------------------------------------------
#
# Witty Wizard Project 25 Sep 2014
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
    view/MenuManImpl.cpp view/MenuManView.cpp

HEADERS += WittyWizard.h WidgetFunction.h WittyWizardPlugins.h CrystalBall.h \
    model/HitCounterMan.h model/HitCounterManSession.h \
    view/HitCounterManImpl.h view/HitCounterManView.h \
    model/MenuMan.h model/MenuManSession.h \
    view/MenuManImpl.h view/MenuManView.h


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
