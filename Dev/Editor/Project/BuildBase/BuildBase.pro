# ***    Paths  ***#

PATH_SOLUTION_ROOT = $$_PRO_FILE_PWD_/..

INCLUDEPATH +=	$$PATH_SOLUTION_ROOT/../Sources \
				$$PATH_SOLUTION_ROOT/../../SourcesEngine \
				$$PATH_SOLUTION_ROOT/../../SourcesSFML/include \
                $$PATH_SOLUTION_ROOT/../../SourcesPugiXml

DEPENDPATH +=   $$PATH_SOLUTION_ROOT/../Sources \
                $$PATH_SOLUTION_ROOT/../../SourcesEngine \
                $$PATH_SOLUTION_ROOT/../../SourcesSFML/include \
                $$PATH_SOLUTION_ROOT/../../SourcesPugiXml

VPATH +=	$$PATH_SOLUTION_ROOT/../Sources

# ***    Files  ***#

SOURCES +=  main.cpp

OTHER_FILES +=  main.cpp


# ***    Config  ***#

TEMPLATE = app

DEFINES += GUGU_QT
DEFINES += SFML_STATIC
DEFINES += GLEW_STATIC
DEFINES += QT_NO_QT_INCLUDE_WARN

CONFIG += qt
QT = core widgets

#QMAKE_CXXFLAGS += -std=c++0x
QMAKE_CXXFLAGS += -U__STRICT_ANSI__
QMAKE_CXXFLAGS_WARN_ON += -Wno-unused-parameter

CONFIG(debug, debug|release) {
	# -- DEBUG
    POST_TARGETDEPS +=  $$PATH_SOLUTION_ROOT/../Libs/libEditorBase-s-d.a \
                        $$PATH_SOLUTION_ROOT/../Libs/libGuguEngine-s-d.a \
                        $$PATH_SOLUTION_ROOT/../Libs/libSFML-s-d.a

    QMAKE_LIBDIR += -L $$PATH_SOLUTION_ROOT/../Libs
    LIBS += -lEditorBase-s-d \
            -lGuguEngine-s-d \
            -lSFML-s-d \
            -lPugiXml-s-d

    QMAKE_LIBDIR += $$PATH_SOLUTION_ROOT/../../SourcesSFML/extlibs/libs-mingw/x86
    LIBS += -lfreetype \
            -lgdi32 \
            -lopengl32 \
            -lwinmm \
            -lopenal32 \
            -lvorbisenc \
            -lvorbisfile \
            -lvorbis \
            -logg \
            -lflac \
            -lws2_32

	TARGET = GuguEditorDebug
    win32:DESTDIR = $$PATH_SOLUTION_ROOT/../../../Tools/GuguEditor
} else {
	# -- RELEASE
    POST_TARGETDEPS +=  $$PATH_SOLUTION_ROOT/../Libs/libEditorBase-s.a \
                        $$PATH_SOLUTION_ROOT/../Libs/libGuguEngine-s.a \
                        $$PATH_SOLUTION_ROOT/../Libs/libSFML-s.a

    QMAKE_LIBDIR += -L $$PATH_SOLUTION_ROOT/../Libs
    LIBS += -lEditorBase-s \
            -lGuguEngine-s \
            -lSFML-s \
            -lPugiXml-s

    QMAKE_LIBDIR += $$PATH_SOLUTION_ROOT/../../SourcesSFML/extlibs/libs-mingw/x86
    LIBS += -lfreetype \
            -lgdi32 \
            -lopengl32 \
            -lwinmm \
            -lopenal32 \
            -lvorbisenc \
            -lvorbisfile \
            -lvorbis \
            -logg \
            -lflac \
            -lws2_32
        
	TARGET = GuguEditor
    win32:DESTDIR = $$PATH_SOLUTION_ROOT/../../../Tools/GuguEditor
}
