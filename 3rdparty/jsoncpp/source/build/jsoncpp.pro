#-------------------------------------------------
#
# Project created by QtCreator 2011-04-02T19:19:19
#
#-------------------------------------------------

QT       -= core gui

TARGET = jsoncpp
TEMPLATE = lib
CONFIG += staticlib

HEADERS += \
    ../include/json/writer.h \
    ../include/json/value.h \
    ../include/json/reader.h \
    ../include/json/json.h \
    ../include/json/forwards.h \
    ../include/json/features.h \
    ../include/json/config.h \
    ../include/json/autolink.h \
    ../src/lib_json/json_batchallocator.h

INCLUDEPATH += ../include
 
unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/local/lib
    }
    INSTALLS += target
}

SOURCES += \
    ../src/lib_json/json_writer.cpp \
    ../src/lib_json/json_valueiterator.inl \
    ../src/lib_json/json_value.cpp \
    ../src/lib_json/json_reader.cpp \
    ../src/lib_json/json_internalmap.inl \
    ../src/lib_json/json_internalarray.inl
