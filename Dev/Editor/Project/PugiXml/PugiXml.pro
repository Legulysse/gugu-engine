# ***    Paths  ***#

PATH_SOLUTION_ROOT = $$_PRO_FILE_PWD_/..

INCLUDEPATH +=	$$PATH_SOLUTION_ROOT/../../SourcesPugiXml

VPATH += $$PATH_SOLUTION_ROOT/../../SourcesPugiXml


#***  PugiXml  ***#

#Auto begin  (Do not edit or duplicate ! Will be used by a script)

HEADERS +=  \
            PugiXml/pugiconfig.hpp \
            PugiXml/pugixml.hpp

SOURCES +=  \
            PugiXml/pugixml.cpp

OTHER_FILES +=  \
            PugiXml/pugiconfig.hpp \
            PugiXml/pugixml.cpp \
            PugiXml/pugixml.hpp

#Auto end  (Do not edit or duplicate ! Will be used by a script)


# ***    Config  ***#

TEMPLATE = lib

CONFIG += staticlib

DEFINES += UNICODE
DEFINES += _UNICODE
DEFINES += QT_NO_QT_INCLUDE_WARN

#QMAKE_CXXFLAGS += -std=c++0x
QMAKE_CXXFLAGS += -U__STRICT_ANSI__
QMAKE_CXXFLAGS_WARN_ON += -Wno-unused-parameter

CONFIG(debug, debug|release) {
    # -- DEBUG
    TARGET = PugiXml-s-d
    win32:DESTDIR = $$PATH_SOLUTION_ROOT/../Libs
} else {
    # -- RELEASE
    TARGET = PugiXml-s
    win32:DESTDIR = $$PATH_SOLUTION_ROOT/../Libs
}
