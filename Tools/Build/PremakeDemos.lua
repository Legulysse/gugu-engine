-- Template for a default project
function ProjectDefault(BuildCfg, ProjectName, DirSources, DirVersion, ProjectID)
    
	DirSources = EnsureSlash(DirSources)
	DirVersion = EnsureSlash(DirVersion)
	SubDirBinaries = EnsureSlash(BuildCfg.SubDirBinaries)
	
    project (ProjectName)
        language "C++"
        defines { "SFML_STATIC", "_CRT_SECURE_NO_WARNINGS" }
        characterset "Unicode"
        targetdir   (DirVersion..SubDirBinaries)
        debugdir    (DirVersion)
        uuid        (ProjectID)
        
        -- Projects dependencies
        dependson { "GuguEngine" }
        
        -- Files
        files {
            DirSources.."**.h",
            DirSources.."**.hpp",
            DirSources.."**.cpp",
            DirSources.."**.tpp",
        }
        
        -- Includes directories
        includedirs {
            DirSources,
            BuildCfg.DirSourcesEngine,
            BuildCfg.DirSourcesSfml.."include/",
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
            
        filter { "system:windows", "action:vs2015 or vs2017", "platforms:x86" }
            libdirs { BuildCfg.DirSourcesSfml.."extlibs/libs-msvc-universal/x86", }
        filter { "system:windows", "action:vs2015 or vs2017", "platforms:x64" }
            libdirs { BuildCfg.DirSourcesSfml.."extlibs/libs-msvc-universal/x64", }
            
        -- Links
        filter { "configurations:Debug" }
            links { "GuguEngine-s-d", "SFML-s-d", "PugiXml-s-d" }
            
        filter { "configurations:Release" }
            links { "GuguEngine-s", "SFML-s", "PugiXml-s" }
            
        filter { "system:windows" }
			links { "jpeg", "freetype", "gdi32", "opengl32", "winmm", "openal32", "vorbisenc", "vorbisfile", "vorbis", "ogg", "flac", "ws2_32" }
            
        filter { "system:linux" }
            links { "pthread", "GL", "X11", "Xrandr", "freetype", "GLEW", "jpeg", "sndfile", "vorbisenc", "vorbisfile", "vorbis", "ogg", "FLAC", "openal", "udev" }
		
        -- Target
        filter { "configurations:Debug" }
            kind "ConsoleApp"
            defines { "DEBUG" }
            flags { "NoMinimalRebuild", "MultiProcessorCompile" }
            symbols "On"
            targetname (ProjectName.."-d")
            
        filter { "configurations:Release" }
            kind "WindowedApp"
            defines { "NDEBUG" }
            flags { "NoMinimalRebuild", "MultiProcessorCompile" }
            optimize "On"
            targetname (ProjectName)
            
        filter { "platforms:x86" }
            architecture "x32"
            
        filter { "platforms:x64" }
            architecture "x64"
            
        -- Options
        filter { "system:windows", "action:vs2013 or vs2015 or vs2017" }
            warnings "Extra"
            disablewarnings { "4100", "4189" } -- 4100 = unreferenced formal parameter, 4189 = local variable is initialized but not referenced
            
end

-- Template for a default SFML project
function ProjectDefaultSFML(BuildCfg, ProjectName, DirSources, DirVersion, ProjectID)
    
	DirSources = EnsureSlash(DirSources)
	DirVersion = EnsureSlash(DirVersion)
	SubDirBinaries = EnsureSlash(BuildCfg.SubDirBinaries)
	
    project (ProjectName)
        language "C++"
        defines { "SFML_STATIC", "_CRT_SECURE_NO_WARNINGS" }
        characterset "Unicode"
        targetdir   (DirVersion..SubDirBinaries)
        debugdir    (DirVersion)
        uuid        (ProjectID)
        
        -- Projects dependencies
        dependson { "SFML" }
            
        -- Files
        files {
            DirSources.."**.h",
            DirSources.."**.hpp",
            DirSources.."**.cpp",
            DirSources.."**.tpp",
        }
        
        -- Includes directories
        includedirs {
            DirSources,
            BuildCfg.DirSourcesSfml.."include/",
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
            
        filter { "system:windows", "action:vs2015 or vs2017", "platforms:x86" }
            libdirs { BuildCfg.DirSourcesSfml.."extlibs/libs-msvc-universal/x86", }
        filter { "system:windows", "action:vs2015 or vs2017", "platforms:x64" }
            libdirs { BuildCfg.DirSourcesSfml.."extlibs/libs-msvc-universal/x64", }
            
        -- Links
        filter { "configurations:Debug" }
            links { "SFML-s-d" }
            
        filter { "configurations:Release" }
            links { "SFML-s" }
            
        filter { "system:windows" }
			links { "jpeg", "freetype", "gdi32", "opengl32", "winmm", "openal32", "vorbisenc", "vorbisfile", "vorbis", "ogg", "flac", "ws2_32" }
            
        filter { "system:linux" }
            links { "pthread", "GL", "X11", "Xrandr", "freetype", "GLEW", "jpeg", "sndfile", "vorbisenc", "vorbisfile", "vorbis", "ogg", "FLAC", "openal", "udev" }
		
        -- Target
        filter { "configurations:Debug" }
            kind "ConsoleApp"
            defines { "DEBUG" }
            flags { "NoMinimalRebuild", "MultiProcessorCompile" }
            symbols "On"
            targetname (ProjectName.."-d")
            
        filter { "configurations:Release" }
            kind "WindowedApp"
            defines { "NDEBUG" }
            flags { "NoMinimalRebuild", "MultiProcessorCompile" }
            optimize "On"
            targetname (ProjectName)
            
        filter { "platforms:x86" }
            architecture "x32"
            
        filter { "platforms:x64" }
            architecture "x64"
            
end