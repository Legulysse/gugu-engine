-- Utility
function EnsureSlash(path)
	if string.len(path) > 0 and string.sub(path, -1) ~= "/" then
		return path.."/"
	end
	return path
end

-- Project GuguEditor App
function ProjectAppGuguEditor(BuildCfg)
    
	SubDirBinaries = EnsureSlash(BuildCfg.SubDirBinaries)
	
    project ("GuguEditorApp")
        language "C++"
        defines { "SFML_STATIC", "_CRT_SECURE_NO_WARNINGS" }
        systemversion "latest"
        characterset "Unicode"
        targetdir   (BuildCfg.DirEditorVersion..SubDirBinaries)
        debugdir    (BuildCfg.DirEditorVersion)
        uuid        ("E4D3697E-E0B5-4343-B000-E895BACF446A")
        
        -- Projects dependencies
        dependson { "GuguEditor" }
        
        -- Files
        files {
            BuildCfg.DirSourcesEditorApp.."**.h",
            BuildCfg.DirSourcesEditorApp.."**.hpp",
            BuildCfg.DirSourcesEditorApp.."**.cpp",
            BuildCfg.DirSourcesEditorApp.."**.tpp",
        }
        
        -- Includes directories
        includedirs {
            BuildCfg.DirSourcesEditorApp,
            BuildCfg.DirSourcesEditor,
            BuildCfg.DirSourcesEngine,
            BuildCfg.DirSourcesSfml.."include/",
            BuildCfg.DirSourcesPugiXml,
            BuildCfg.DirSourcesImGui,
        }
        
        -- Libs directories
        libdirs {
            BuildCfg.DirLibEngine,
        }
		
        filter { "system:windows", "action:codelite", "platforms:x86" }
			libdirs { BuildCfg.DirSourcesSfml.."extlibs/libs-mingw/x86", }
        filter { "system:windows", "action:codelite", "platforms:x64" }
			libdirs { BuildCfg.DirSourcesSfml.."extlibs/libs-mingw/x64", }

        filter { "system:windows", "action:vs2013", "platforms:x86" }
            libdirs { BuildCfg.DirSourcesSfml.."extlibs/libs-msvc/x86", }
        filter { "system:windows", "action:vs2013", "platforms:x64" }
            libdirs { BuildCfg.DirSourcesSfml.."extlibs/libs-msvc/x64", }
            
        filter { "system:windows", "action:vs2015 or vs2017 or vs2019", "platforms:x86" }
            libdirs { BuildCfg.DirSourcesSfml.."extlibs/libs-msvc-universal/x86", }
        filter { "system:windows", "action:vs2015 or vs2017 or vs2019", "platforms:x64" }
            libdirs { BuildCfg.DirSourcesSfml.."extlibs/libs-msvc-universal/x64", }
            
        -- Links
        filter { "configurations:Debug" }
            links { "GuguEditorLib-s-d", "GuguEngine-s-d", "ImGui-s-d", "SFML-s-d", "PugiXml-s-d" }
            
        filter { "configurations:Release" }
            links { "GuguEditorLib-s", "GuguEngine-s", "ImGui-s", "SFML-s", "PugiXml-s" }
            
        filter { "system:windows" }
			links { "freetype", "gdi32", "opengl32", "winmm", "openal32", "vorbisenc", "vorbisfile", "vorbis", "ogg", "flac", "ws2_32" }
            
        filter { "system:linux" }
            links { "pthread", "GL", "X11", "Xrandr", "freetype", "GLEW", "sndfile", "vorbisenc", "vorbisfile", "vorbis", "ogg", "FLAC", "openal", "udev" }
		
        -- Target
        filter { "configurations:Debug" }
            kind "ConsoleApp"
            defines { "DEBUG" }
            flags { "NoMinimalRebuild", "MultiProcessorCompile" }
            symbols "On"
            targetname ("GuguEditor-d")
            
        filter { "configurations:Release" }
            kind "WindowedApp"
            defines { "NDEBUG" }
            flags { "NoMinimalRebuild", "MultiProcessorCompile" }
            optimize "On"
            targetname ("GuguEditor")
            
        filter { "platforms:x86" }
            architecture "x32"
            
        filter { "platforms:x64" }
            architecture "x64"
            
        -- Options
        filter { "system:windows", "action:vs2013 or vs2015 or vs2017 or vs2019" }
            warnings "Extra"
            disablewarnings { "4100", "4189" } -- 4100 = unreferenced formal parameter, 4189 = local variable is initialized but not referenced
            
end

-- Project GuguEditor
function ProjectLibGuguEditor(BuildCfg)

    project "GuguEditor"
        kind "StaticLib"
        language "C++"
        defines { "SFML_STATIC", "_CRT_SECURE_NO_WARNINGS" }
        systemversion "latest"
        characterset "Unicode"
        targetdir (BuildCfg.DirLibEngine)
        uuid "D56FC1A7-034F-4E7F-9DBB-B615C3C5C070"
        
        files {
            BuildCfg.DirSourcesEditor.."**.h",
            BuildCfg.DirSourcesEditor.."**.hpp",
            BuildCfg.DirSourcesEditor.."**.cpp",
            BuildCfg.DirSourcesEditor.."**.tpp",
        }
        includedirs ({
            BuildCfg.DirSourcesEditor,
            BuildCfg.DirSourcesEngine,
            BuildCfg.DirSourcesSfml.."include/",
            BuildCfg.DirSourcesPugiXml,
            BuildCfg.DirSourcesImGui,
        })
		
        dependson { "GuguEngine", "ImGui", "SFML", "PugiXml" } -- project dependency

        filter { "configurations:Debug" }
            defines { "DEBUG" }
            flags { "NoMinimalRebuild", "MultiProcessorCompile" }
            symbols "On"
            targetname "GuguEditorLib-s-d"

        filter { "configurations:Release" }
            defines { "NDEBUG" }
            flags { "NoMinimalRebuild", "MultiProcessorCompile" }
            optimize "On" -- Off, On, Debug, Size, Speed, Full
            targetname "GuguEditorLib-s"
            
        filter { "platforms:x86" }
            architecture "x32"
            
        filter { "platforms:x64" }
            architecture "x64"
            
        filter { "system:windows", "action:vs2013 or vs2015 or vs2017 or vs2019" }
            warnings "Extra"
            disablewarnings { "4100", } -- 4100 = unreferenced formal parameter, 4189 = local variable is initialized but not referenced
end

-- Project GuguEngine
function ProjectLibGuguEngine(BuildCfg)

    project "GuguEngine"
        kind "StaticLib"
        language "C++"
        defines { "SFML_STATIC", "_CRT_SECURE_NO_WARNINGS" }
        systemversion "latest"
        characterset "Unicode"
        targetdir (BuildCfg.DirLibEngine)
        uuid "59E650EC-0FD8-4D3C-A9E3-29DFF2AA5096"
        
        files {
            BuildCfg.DirSourcesEngine.."**.h",
            BuildCfg.DirSourcesEngine.."**.hpp",
            BuildCfg.DirSourcesEngine.."**.cpp",
            BuildCfg.DirSourcesEngine.."**.tpp",
        }
        includedirs ({
            BuildCfg.DirSourcesEngine,
            BuildCfg.DirSourcesSfml.."include/",
            BuildCfg.DirSourcesPugiXml,
            BuildCfg.DirSourcesImGui,
        })
		
        dependson { "ImGui", "SFML", "PugiXml" } -- project dependency

        filter { "configurations:Debug" }
            defines { "DEBUG" }
            flags { "NoMinimalRebuild", "MultiProcessorCompile" }
            symbols "On"
            targetname "GuguEngine-s-d"

        filter { "configurations:Release" }
            defines { "NDEBUG" }
            flags { "NoMinimalRebuild", "MultiProcessorCompile" }
            optimize "On" -- Off, On, Debug, Size, Speed, Full
            targetname "GuguEngine-s"
            
        filter { "platforms:x86" }
            architecture "x32"
            
        filter { "platforms:x64" }
            architecture "x64"
            
        filter { "system:windows", "action:vs2013 or vs2015 or vs2017 or vs2019" }
            warnings "Extra"
            disablewarnings { "4100", } -- 4100 = unreferenced formal parameter, 4189 = local variable is initialized but not referenced
end

-- Project ImGui
function ProjectLibImGui(BuildCfg)

    DirImGuiSources = BuildCfg.DirSourcesImGui

    project "ImGui"
        kind "StaticLib"
        language "C++"
        defines { "SFML_STATIC", "_CRT_SECURE_NO_WARNINGS", "UNICODE", "_UNICODE" }
        systemversion "latest"
        characterset "Unicode"
        targetdir (BuildCfg.DirLibEngine)
        uuid "11A07067-A137-4C3C-B6ED-F2DC8BE3639D"
        
        files {
            DirImGuiSources.."**.h",
            DirImGuiSources.."**.cpp",
        }
        includedirs {
            DirImGuiSources,
            BuildCfg.DirSourcesSfml.."include/",
        }

        dependson { "SFML" } -- project dependency

        filter { "configurations:Debug" }
            defines { "DEBUG", "_DEBUG" }
            flags { "NoMinimalRebuild", "MultiProcessorCompile" }
            symbols "On"
            targetname "ImGui-s-d"

        filter { "configurations:Release" }
            defines { "NDEBUG" }
            flags { "NoMinimalRebuild", "MultiProcessorCompile" }
            optimize "On"
            targetname "ImGui-s"
            
        filter { "platforms:x86" }
            architecture "x32"
            
        filter { "platforms:x64" }
            architecture "x64"
end

-- Project SFML
function ProjectLibSFML(BuildCfg)

    DirSfmlHeaders = BuildCfg.DirSourcesSfml.."include/"
    DirSfmlSources = BuildCfg.DirSourcesSfml.."src/"
    DirSfmlExternals = BuildCfg.DirSourcesSfml.."extlibs/"

    project "SFML"
        kind "StaticLib"
        language "C++"
        defines { "SFML_STATIC", "OV_EXCLUDE_STATIC_CALLBACKS", "FLAC__NO_DLL", "_CRT_SECURE_NO_WARNINGS", "UNICODE", "_UNICODE" } -- OV_EXCLUDE_STATIC_CALLBACKS and FLAC__NO_DLL are parts of modifications made in the SFML repo, see the Notes.txt there
        systemversion "latest"
        characterset "Unicode"
        targetdir (BuildCfg.DirLibEngine)
        uuid "936D68B9-FF55-CA40-9A14-7C2D95524D8B"
        
        files {
            DirSfmlHeaders.."**.hpp",
            DirSfmlHeaders.."**.inl",
            DirSfmlSources.."**.hpp",
            DirSfmlSources.."**.cpp",
        }
        includedirs {
            DirSfmlHeaders,
            DirSfmlSources,
            DirSfmlExternals.."headers",
            DirSfmlExternals.."headers/AL",
            DirSfmlExternals.."headers/FLAC",
            DirSfmlExternals.."headers/freetype2",
            DirSfmlExternals.."headers/jpeg",
            DirSfmlExternals.."headers/ogg",
            DirSfmlExternals.."headers/stb_image",
            DirSfmlExternals.."headers/vorbis",
        }

        filter { "configurations:Debug" }
            defines { "DEBUG", "_DEBUG" }
            flags { "NoMinimalRebuild", "MultiProcessorCompile" }
            symbols "On"
            targetname "SFML-s-d"

        filter { "configurations:Release" }
            defines { "NDEBUG" }
            flags { "NoMinimalRebuild", "MultiProcessorCompile" }
            optimize "On"
            targetname "SFML-s"
            
        filter { "platforms:x86" }
            architecture "x32"
            
        filter { "platforms:x64" }
            architecture "x64"
            
        filter { "system:windows" }
			includedirs {
                --DirSfmlExternals.."headers/libfreetype/windows",
            }
			excludes { 
                DirSfmlSources.."SFML/Main/SFMLActivity.cpp",
                DirSfmlSources.."SFML/Main/MainAndroid.cpp",
                DirSfmlSources.."SFML/Window/EGLCheck.**",
                DirSfmlSources.."SFML/Window/EglContext.**",
                DirSfmlSources.."SFML/Window/Android/**",
				DirSfmlSources.."SFML/Window/FreeBSD/**",
                DirSfmlSources.."SFML/Window/iOS/**",
				DirSfmlSources.."SFML/Window/OpenBSD/**",
				DirSfmlSources.."SFML/Window/OSX/**",
                DirSfmlSources.."SFML/Window/Unix/**",
				DirSfmlSources.."SFML/System/Android/**",
				DirSfmlSources.."SFML/System/Unix/**",
				DirSfmlSources.."SFML/Network/Unix/**",
            }
		
        filter { "system:linux" }
			excludes { 
                DirSfmlSources.."SFML/Main/**",
                DirSfmlSources.."SFML/Window/EGLCheck.**",
                DirSfmlSources.."SFML/Window/EglContext.**",
                DirSfmlSources.."SFML/Window/Android/**",
				DirSfmlSources.."SFML/Window/FreeBSD/**",
                DirSfmlSources.."SFML/Window/iOS/**",
				DirSfmlSources.."SFML/Window/OpenBSD/**",
				DirSfmlSources.."SFML/Window/OSX/**",
                DirSfmlSources.."SFML/Window/Win32/**",
				DirSfmlSources.."SFML/System/Android/**",
				DirSfmlSources.."SFML/System/Win32/**",
				DirSfmlSources.."SFML/Network/Win32/**",
            }
end

-- Project PugiXml
function ProjectLibPugiXml(BuildCfg)

    DirPugiXmlSources = BuildCfg.DirSourcesPugiXml

    project "PugiXml"
        kind "StaticLib"
        language "C++"
        defines { "_CRT_SECURE_NO_WARNINGS", "UNICODE", "_UNICODE" }
        systemversion "latest"
        characterset "Unicode"
        targetdir (BuildCfg.DirLibEngine)
        uuid "2961203D-3842-4A18-9129-7295D98964CF"
        
        files {
            DirPugiXmlSources.."**.hpp",
            DirPugiXmlSources.."**.cpp",
        }
        includedirs {
            DirPugiXmlSources,
        }

        filter { "configurations:Debug" }
            defines { "DEBUG", "_DEBUG" }
            flags { "NoMinimalRebuild", "MultiProcessorCompile" }
            symbols "On"
            targetname "PugiXml-s-d"

        filter { "configurations:Release" }
            defines { "NDEBUG" }
            flags { "NoMinimalRebuild", "MultiProcessorCompile" }
            optimize "On"
            targetname "PugiXml-s"
            
        filter { "platforms:x86" }
            architecture "x32"
            
        filter { "platforms:x64" }
            architecture "x64"
end
