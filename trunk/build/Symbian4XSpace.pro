# Add files and directories to ship with the application 
# by adapting the examples below.
# file1.source = 
# dir1.source = 

dir1.source = ../data/

# DEPLOYMENTFOLDERS = dir1
# DEPLOYMENTFOLDERS = file1 dir1

symbian:TARGET.UID3 = 0xE6A4EF8A

# Allow network access on Symbian
#symbian:TARGET.CAPABILITY += NetworkServices

CONFIG += mobility
MOBILITY += multimedia

SOURCES += ../implementation/main/main.cpp \ 
    ../implementation/data/NamesData.cpp \
    ../implementation/data/AssetSerializer.cpp \
    ../implementation/game/UniverseVisitor.cpp \
    ../implementation/game/Universe.cpp \
    ../implementation/game/StarSystem.cpp \
    ../implementation/game/Star.cpp \
    ../implementation/game/Shipyard.cpp \
    ../implementation/game/ShipConfig.cpp \
    ../implementation/game/Ship.cpp \
    ../implementation/game/SectorReference.cpp \
    ../implementation/game/StarSystemReference.cpp \
    ../implementation/game/Sector.cpp \
    ../implementation/game/Player.cpp \
    ../implementation/game/AI.cpp \
    ../implementation/game/Planet.cpp \
    ../implementation/game/Parameters.cpp \
    ../implementation/game/NextTurnVisitor.cpp \
    ../implementation/game/NextPlayerVisitor.cpp \
    ../implementation/game/Events.cpp \
    ../implementation/game/DeserializeVisitor.cpp \
    ../implementation/game/Component.cpp \
    ../implementation/game/StatsVisitor.cpp \
    ../implementation/gui/UniverseViewer.cpp \
    ../implementation/gui/UniversePainter.cpp \
    ../implementation/gui/SubscribablePushButton.cpp \
    ../implementation/gui/ShipConfigDesign.cpp \
    ../implementation/gui/ShipConfigBuild.cpp \
    ../implementation/gui/ShipConfigModel.cpp \
    ../implementation/gui/SectorItemModel.cpp \
    ../implementation/gui/PlanetListModel.cpp \
    ../implementation/gui/SectorListView.cpp \
    ../implementation/gui/SetupPanel.cpp \
    ../implementation/gui/MainWindow.cpp \
    ../implementation/gui/CSSHelper.cpp \
    ../implementation/gui/ComponentSelection.cpp \
    ../implementation/gui/ComponentModel.cpp \
    ../implementation/gui/QsKineticScroller.cpp \
    ../implementation/gui/TableView.cpp \
    ../implementation/gui/Soundtrack.cpp

HEADERS += \ 
    ../implementation/data/NamesData.h \
    ../implementation/data/AssetSerializer.h \
    ../implementation/game/UniverseVisitor.h \
    ../implementation/game/Universe.h \
    ../implementation/game/StarSystem.h \
    ../implementation/game/StarSystemReference.h \
    ../implementation/game/Star.h \
    ../implementation/game/Shipyard.h \
    ../implementation/game/ShipConfig.h \
    ../implementation/game/Ship.h \
    ../implementation/game/SectorReference.h \
    ../implementation/game/Sector.h \
    ../implementation/game/Player.h \
    ../implementation/game/AI.h \
    ../implementation/game/Planet.h \
    ../implementation/game/Parameters.h \
    ../implementation/game/NextTurnVisitor.h \
    ../implementation/game/NextPlayerVisitor.h \
    ../implementation/game/Events.h \
    ../implementation/game/Event.h \
    ../implementation/game/DeserializeVisitor.h \
    ../implementation/game/Component.h \
    ../implementation/game/StatsVisitor.h \
    ../implementation/gui/UniverseViewer.h \
    ../implementation/gui/UniversePainter.h \
    ../implementation/gui/SubscribablePushButton.h \
    ../implementation/gui/ShipConfigDesign.h \
    ../implementation/gui/ShipConfigBuild.h \
    ../implementation/gui/ShipConfigModel.h \
    ../implementation/gui/SectorItemModel.h \
    ../implementation/gui/PlanetListModel.h \
    ../implementation/gui/SectorListView.h \
    ../implementation/gui/SetupPanel.h \
    ../implementation/gui/MainWindow.h \
    ../implementation/gui/CSSHelper.h \
    ../implementation/gui/ComponentSelection.h \
    ../implementation/gui/ComponentModel.h \
    ../implementation/gui/QsKineticScroller.h \
    ../implementation/gui/TableView.h \
    ../implementation/gui/Soundtrack.h

OTHER_FILES += \
    ../data/images/nebulae.png \
    ../data/images/blue.png \
    ../data/images/red.png \
    ../data/images/green.png \
    ../data/images/magenta.png \
    ../data/images/none.png \
    ../data/universe.json \
    ../data/savegame.json \
    ../data/empty.json \
    ../data/configs/parameters.json \
    ../data/configs/components.json \
    ../data/names/stars.json \
    ../data/names/empires.json \
    ../data/stylesheets/mainwindow.css \
    ../data/sounds/Ghosts3.mp3 \
    ../data/sounds/Ghosts9.mp3

INCLUDEPATH += ../3rdparty/jsoncpp/include
INCLUDEPATH += ../implementation

QT += svg

win32:debug {
    #MSVC
    LIBS += -L../3rdparty/jsoncpp/lib_debug/ -ljsoncpp
    #MINGW-GCC
    #LIBS += -L../3rdparty/jsoncpp/source/jsoncpp-build-simulator/debug/ -ljsoncpp
}

win32:release {
    #MSVC
    LIBS += -L../3rdparty/jsoncpp/lib_release/ -ljsoncpp
    #MINGW-GCC
    #LIBS += -L../3rdparty/jsoncpp/source/jsoncpp-build-simulator/release/ -ljsoncpp
}

simulator:debug {
    #MSVC
    #LIBS += -L../3rdparty/jsoncpp/lib_debug/ -ljsoncpp
    #MINGW-GCC
    LIBS += -L../3rdparty/jsoncpp/source/jsoncpp-build-simulator/debug/ -ljsoncpp
}

simulator:release {
    #MSVC
    #LIBS += -L../3rdparty/jsoncpp/lib_release/ -ljsoncpp
    #MINGW-GCC
    LIBS += -L../3rdparty/jsoncpp/source/jsoncpp-build-simulator/release/ -ljsoncpp
}

# must be found in QtSDK\Symbian\SDKs\Symbian1Qt472\epoc32\release\armv5\udeb
# if it is not there use 3rdparty\jsoncpp\source\build\jsoncpp.pro to build
symbian:debug {
    LIBS += -ljsoncpp
    DEPLOYMENTFOLDERS = dir1
}

# must be found in QtSDK\Symbian\SDKs\Symbian1Qt472\epoc32\release\armv5\urel
# if it is not there use 3rdparty\jsoncpp\source\build\jsoncpp.pro to build
symbian:release {
    LIBS += -ljsoncpp
    DEPLOYMENTFOLDERS = dir1
}
# Please do not modify the following two lines. Required for deployment.
include(deployment.pri)
qtcAddDeployment()
