-- Utility
function EnsureSlash(path)
	if string.len(path) > 0 and string.sub(path, -1) ~= "/" then
		return path.."/"
	end
	return path
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
        })
		
        dependson { "SFML", "PugiXml" } -- project dependency

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
            
        filter { "system:windows", "action:vs2013 or vs2015 or vs2017" }
            warnings "Extra"
            disablewarnings { "4100", } -- 4100 = unreferenced formal parameter, 4189 = local variable is initialized but not referenced
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
