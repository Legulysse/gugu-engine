# ***    Paths  ***#

PATH_SOLUTION_ROOT = $$_PRO_FILE_PWD_/..

INCLUDEPATH +=	$$PATH_SOLUTION_ROOT/../Sources \
                $$PATH_SOLUTION_ROOT/../../SourcesEngine \
                $$PATH_SOLUTION_ROOT/../../SourcesSFML/include \
                $$PATH_SOLUTION_ROOT/../../SourcesPugiXml

VPATH +=	$$PATH_SOLUTION_ROOT/../Sources \
            $$PATH_SOLUTION_ROOT/../../SourcesEngine


# ***    Files  ***#

SOURCES +=  Window/MainWindow.cpp \
            Window/ToolWindow.cpp \
            Window/AnimationPanel.cpp \
            Window/ImageSetPanel.cpp \
            Dialog/DialogGenerateAnimRange.cpp \
            Dialog/DialogRenameResource.cpp \
            Dialog/DialogRename.cpp \
            Dialog/DialogAbout.cpp

HEADERS  += Window/MainWindow.h \
            Window/ToolWindow.h \
            Window/AnimationPanel.h \
            Window/ImageSetPanel.h \
            Dialog/DialogGenerateAnimRange.h \
            Dialog/DialogRenameResource.h \
            Dialog/DialogRename.h \
            Dialog/DialogAbout.h

FORMS    += Design/MainWindow.ui \
            Design/AnimationPanel.ui \
            Design/ImageSetPanel.ui

OTHER_FILES +=  Window/MainWindow.h \
                Window/MainWindow.cpp \
                Window/ToolWindow.h \
                Window/ToolWindow.cpp \
                Window/AnimationPanel.h \
                Window/AnimationPanel.cpp \
                Window/ImageSetPanel.h \
                Window/ImageSetPanel.cpp \
                Dialog/DialogGenerateAnimRange.h \
                Dialog/DialogGenerateAnimRange.cpp \
                Dialog/DialogRenameResource.h \
                Dialog/DialogRenameResource.cpp \
                Dialog/DialogRename.h \
                Dialog/DialogRename.cpp \
                Dialog/DialogAbout.h \
                Dialog/DialogAbout.cpp \
                Design/MainWindow.ui \
                Design/AnimationPanel.ui \
                Design/ImageSetPanel.ui


# ***    Config  ***#

TEMPLATE = lib

CONFIG += staticlib

DEFINES += GUGU_QT
DEFINES += SFML_STATIC
DEFINES += GLEW_STATIC
DEFINES += QT_NO_QT_INCLUDE_WARN

#QMAKE_CXXFLAGS += -std=c++0x
QMAKE_CXXFLAGS += -U__STRICT_ANSI__
QMAKE_CXXFLAGS_WARN_ON += -Wno-unused-parameter

CONFIG(debug, debug|release) {
    # -- DEBUG
    TARGET = EditorBase-s-d
    win32:DESTDIR = $$PATH_SOLUTION_ROOT/../Libs
} else {
    # -- RELEASE
    TARGET = EditorBase-s
    win32:DESTDIR = $$PATH_SOLUTION_ROOT/../Libs
}
