# ***    Paths  ***#

PATH_SOLUTION_ROOT = $$_PRO_FILE_PWD_/..

INCLUDEPATH +=	$$PATH_SOLUTION_ROOT/../../SourcesEngine
INCLUDEPATH +=	$$PATH_SOLUTION_ROOT/../../SourcesSFML/include
INCLUDEPATH +=	$$PATH_SOLUTION_ROOT/../../SourcesPugiXml

VPATH += $$PATH_SOLUTION_ROOT/../../SourcesEngine


#***  GuguEngine  ***#

#Auto begin  (Do not edit or duplicate ! Will be used by a script)

HEADERS +=  \
            Gugu/Audio/MusicInstance.h \
            Gugu/Audio/MusicLayer.h \
            Gugu/Audio/SoundInstance.h \
            Gugu/Common.h \
            Gugu/Element/2D/ElementSFDrawable.h \
            Gugu/Element/2D/ElementSprite.h \
            Gugu/Element/2D/ElementSpriteAnimated.h \
            Gugu/Element/2D/ElementSpriteGroup.h \
            Gugu/Element/2D/ElementText.h \
            Gugu/Element/2D/ElementTile.h \
            Gugu/Element/2D/ElementTileMap.h \
            Gugu/Element/Element.h \
            Gugu/Element/UI/ElementButton.h \
            Gugu/Element/UI/ElementList.h \
            Gugu/Element/UI/ElementListItem.h \
            Gugu/Element/UI/ElementUILayout.h \
            Gugu/Engine.h \
            Gugu/External/PugiXmlWrap.h \
            Gugu/Manager/ManagerAudio.h \
            Gugu/Manager/ManagerConfig.h \
            Gugu/Manager/ManagerNetwork.h \
            Gugu/Manager/ManagerResources.h \
            Gugu/Misc/Application.h \
            Gugu/Misc/BaseAnimation2D.h \
            Gugu/Misc/EventListener.h \
            Gugu/Misc/Logger.h \
            Gugu/Misc/Trace.h \
            Gugu/Misc/UDim.h \
            Gugu/Network/ClientInfo.h \
            Gugu/Network/EnumsNetwork.h \
            Gugu/Network/NetworkPacket.h \
            Gugu/Render/QSFMLCanvas.h \
            Gugu/Render/Renderer.h \
            Gugu/Resources/AnimSet.h \
            Gugu/Resources/Datasheet.h \
            Gugu/Resources/EnumsResources.h \
            Gugu/Resources/Font.h \
            Gugu/Resources/ImageSet.h \
            Gugu/Resources/Music.h \
            Gugu/Resources/Resource.h \
            Gugu/Resources/ResourceInfo.h \
            Gugu/Resources/Sound.h \
            Gugu/Resources/SoundCue.h \
            Gugu/Resources/Texture.h \
            Gugu/Utility/Action.h \
            Gugu/Utility/Delegate.h \
            Gugu/Utility/DeltaTime.h \
            Gugu/Utility/FileInfo.h \
            Gugu/Utility/FixedFloat.h \
            Gugu/Utility/Hash.h \
            Gugu/Utility/Math.h \
            Gugu/Utility/Math.tpp \
            Gugu/Utility/Random.h \
            Gugu/Utility/Singleton.h \
            Gugu/Utility/State.h \
            Gugu/Utility/StateMachine.h \
            Gugu/Utility/System.h \
            Gugu/Utility/System.tpp \
            Gugu/Utility/Types.h \
            Gugu/Version.h \
            Gugu/Window/Camera.h \
            Gugu/Window/HandlerEvents.h \
            Gugu/Window/Window.h \
            Gugu/World/Actor.h \
            Gugu/World/ActorComponent.h \
            Gugu/World/Grid/BaseGrid.h \
            Gugu/World/Grid/GridCell.h \
            Gugu/World/Grid/HexGrid.h \
            Gugu/World/Grid/SquareGrid.h \
            Gugu/World/Level.h \
            Gugu/World/World.h

SOURCES +=  \
            Gugu/Audio/MusicInstance.cpp \
            Gugu/Audio/MusicLayer.cpp \
            Gugu/Audio/SoundInstance.cpp \
            Gugu/Element/2D/ElementSFDrawable.cpp \
            Gugu/Element/2D/ElementSprite.cpp \
            Gugu/Element/2D/ElementSpriteAnimated.cpp \
            Gugu/Element/2D/ElementSpriteGroup.cpp \
            Gugu/Element/2D/ElementText.cpp \
            Gugu/Element/2D/ElementTile.cpp \
            Gugu/Element/2D/ElementTileMap.cpp \
            Gugu/Element/Element.cpp \
            Gugu/Element/UI/ElementButton.cpp \
            Gugu/Element/UI/ElementList.cpp \
            Gugu/Element/UI/ElementListItem.cpp \
            Gugu/Element/UI/ElementUILayout.cpp \
            Gugu/Engine.cpp \
            Gugu/External/PugiXmlWrap.cpp \
            Gugu/Manager/ManagerAudio.cpp \
            Gugu/Manager/ManagerConfig.cpp \
            Gugu/Manager/ManagerNetwork.cpp \
            Gugu/Manager/ManagerResources.cpp \
            Gugu/Misc/Application.cpp \
            Gugu/Misc/BaseAnimation2D.cpp \
            Gugu/Misc/EventListener.cpp \
            Gugu/Misc/Logger.cpp \
            Gugu/Misc/Trace.cpp \
            Gugu/Misc/UDim.cpp \
            Gugu/Network/ClientInfo.cpp \
            Gugu/Network/NetworkPacket.cpp \
            Gugu/Render/QSFMLCanvas.cpp \
            Gugu/Render/Renderer.cpp \
            Gugu/Resources/AnimSet.cpp \
            Gugu/Resources/Datasheet.cpp \
            Gugu/Resources/Font.cpp \
            Gugu/Resources/ImageSet.cpp \
            Gugu/Resources/Music.cpp \
            Gugu/Resources/Resource.cpp \
            Gugu/Resources/ResourceInfo.cpp \
            Gugu/Resources/Sound.cpp \
            Gugu/Resources/SoundCue.cpp \
            Gugu/Resources/Texture.cpp \
            Gugu/Utility/DeltaTime.cpp \
            Gugu/Utility/FileInfo.cpp \
            Gugu/Utility/FixedFloat.cpp \
            Gugu/Utility/Hash.cpp \
            Gugu/Utility/Math.cpp \
            Gugu/Utility/Random.cpp \
            Gugu/Utility/State.cpp \
            Gugu/Utility/StateMachine.cpp \
            Gugu/Utility/System.cpp \
            Gugu/Window/Camera.cpp \
            Gugu/Window/HandlerEvents.cpp \
            Gugu/Window/Window.cpp \
            Gugu/World/Actor.cpp \
            Gugu/World/ActorComponent.cpp \
            Gugu/World/Grid/BaseGrid.cpp \
            Gugu/World/Grid/GridCell.cpp \
            Gugu/World/Grid/HexGrid.cpp \
            Gugu/World/Grid/SquareGrid.cpp \
            Gugu/World/Level.cpp \
            Gugu/World/World.cpp

OTHER_FILES +=  \
            Gugu/Audio/MusicInstance.cpp \
            Gugu/Audio/MusicInstance.h \
            Gugu/Audio/MusicLayer.cpp \
            Gugu/Audio/MusicLayer.h \
            Gugu/Audio/SoundInstance.cpp \
            Gugu/Audio/SoundInstance.h \
            Gugu/Common.h \
            Gugu/Element/2D/ElementSFDrawable.cpp \
            Gugu/Element/2D/ElementSFDrawable.h \
            Gugu/Element/2D/ElementSprite.cpp \
            Gugu/Element/2D/ElementSprite.h \
            Gugu/Element/2D/ElementSpriteAnimated.cpp \
            Gugu/Element/2D/ElementSpriteAnimated.h \
            Gugu/Element/2D/ElementSpriteGroup.cpp \
            Gugu/Element/2D/ElementSpriteGroup.h \
            Gugu/Element/2D/ElementText.cpp \
            Gugu/Element/2D/ElementText.h \
            Gugu/Element/2D/ElementTile.cpp \
            Gugu/Element/2D/ElementTile.h \
            Gugu/Element/2D/ElementTileMap.cpp \
            Gugu/Element/2D/ElementTileMap.h \
            Gugu/Element/Element.cpp \
            Gugu/Element/Element.h \
            Gugu/Element/UI/ElementButton.cpp \
            Gugu/Element/UI/ElementButton.h \
            Gugu/Element/UI/ElementList.cpp \
            Gugu/Element/UI/ElementList.h \
            Gugu/Element/UI/ElementListItem.cpp \
            Gugu/Element/UI/ElementListItem.h \
            Gugu/Element/UI/ElementUILayout.cpp \
            Gugu/Element/UI/ElementUILayout.h \
            Gugu/Engine.cpp \
            Gugu/Engine.h \
            Gugu/External/PugiXmlWrap.cpp \
            Gugu/External/PugiXmlWrap.h \
            Gugu/Manager/ManagerAudio.cpp \
            Gugu/Manager/ManagerAudio.h \
            Gugu/Manager/ManagerConfig.cpp \
            Gugu/Manager/ManagerConfig.h \
            Gugu/Manager/ManagerNetwork.cpp \
            Gugu/Manager/ManagerNetwork.h \
            Gugu/Manager/ManagerResources.cpp \
            Gugu/Manager/ManagerResources.h \
            Gugu/Misc/Application.cpp \
            Gugu/Misc/Application.h \
            Gugu/Misc/BaseAnimation2D.cpp \
            Gugu/Misc/BaseAnimation2D.h \
            Gugu/Misc/EventListener.cpp \
            Gugu/Misc/EventListener.h \
            Gugu/Misc/Logger.cpp \
            Gugu/Misc/Logger.h \
            Gugu/Misc/Trace.cpp \
            Gugu/Misc/Trace.h \
            Gugu/Misc/UDim.cpp \
            Gugu/Misc/UDim.h \
            Gugu/Network/ClientInfo.cpp \
            Gugu/Network/ClientInfo.h \
            Gugu/Network/EnumsNetwork.h \
            Gugu/Network/NetworkPacket.cpp \
            Gugu/Network/NetworkPacket.h \
            Gugu/Render/QSFMLCanvas.cpp \
            Gugu/Render/QSFMLCanvas.h \
            Gugu/Render/Renderer.cpp \
            Gugu/Render/Renderer.h \
            Gugu/Resources/AnimSet.cpp \
            Gugu/Resources/AnimSet.h \
            Gugu/Resources/Datasheet.cpp \
            Gugu/Resources/Datasheet.h \
            Gugu/Resources/EnumsResources.h \
            Gugu/Resources/Font.cpp \
            Gugu/Resources/Font.h \
            Gugu/Resources/ImageSet.cpp \
            Gugu/Resources/ImageSet.h \
            Gugu/Resources/Music.cpp \
            Gugu/Resources/Music.h \
            Gugu/Resources/Resource.cpp \
            Gugu/Resources/Resource.h \
            Gugu/Resources/ResourceInfo.cpp \
            Gugu/Resources/ResourceInfo.h \
            Gugu/Resources/Sound.cpp \
            Gugu/Resources/Sound.h \
            Gugu/Resources/SoundCue.cpp \
            Gugu/Resources/SoundCue.h \
            Gugu/Resources/Texture.cpp \
            Gugu/Resources/Texture.h \
            Gugu/Utility/Action.h \
            Gugu/Utility/Delegate.h \
            Gugu/Utility/DeltaTime.cpp \
            Gugu/Utility/DeltaTime.h \
            Gugu/Utility/FileInfo.cpp \
            Gugu/Utility/FileInfo.h \
            Gugu/Utility/FixedFloat.cpp \
            Gugu/Utility/FixedFloat.h \
            Gugu/Utility/Hash.cpp \
            Gugu/Utility/Hash.h \
            Gugu/Utility/Math.cpp \
            Gugu/Utility/Math.h \
            Gugu/Utility/Math.tpp \
            Gugu/Utility/Random.cpp \
            Gugu/Utility/Random.h \
            Gugu/Utility/Singleton.h \
            Gugu/Utility/State.cpp \
            Gugu/Utility/State.h \
            Gugu/Utility/StateMachine.cpp \
            Gugu/Utility/StateMachine.h \
            Gugu/Utility/System.cpp \
            Gugu/Utility/System.h \
            Gugu/Utility/System.tpp \
            Gugu/Utility/Types.h \
            Gugu/Version.h \
            Gugu/Window/Camera.cpp \
            Gugu/Window/Camera.h \
            Gugu/Window/HandlerEvents.cpp \
            Gugu/Window/HandlerEvents.h \
            Gugu/Window/Window.cpp \
            Gugu/Window/Window.h \
            Gugu/World/Actor.cpp \
            Gugu/World/Actor.h \
            Gugu/World/ActorComponent.cpp \
            Gugu/World/ActorComponent.h \
            Gugu/World/Grid/BaseGrid.cpp \
            Gugu/World/Grid/BaseGrid.h \
            Gugu/World/Grid/GridCell.cpp \
            Gugu/World/Grid/GridCell.h \
            Gugu/World/Grid/HexGrid.cpp \
            Gugu/World/Grid/HexGrid.h \
            Gugu/World/Grid/SquareGrid.cpp \
            Gugu/World/Grid/SquareGrid.h \
            Gugu/World/Level.cpp \
            Gugu/World/Level.h \
            Gugu/World/World.cpp \
            Gugu/World/World.h

#Auto end  (Do not edit or duplicate ! Will be used by a script)


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
    TARGET = GuguEngine-s-d
    win32:DESTDIR = $$PATH_SOLUTION_ROOT/../Libs
} else {
    # -- RELEASE
    TARGET = GuguEngine-s
    win32:DESTDIR = $$PATH_SOLUTION_ROOT/../Libs
}
