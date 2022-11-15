-- Template for a default project
function ProjectDefault(BuildCfg, ProjectName, DirSources, DirVersion, ProjectID)
    
	DirSources = EnsureSlash(DirSources)
	
    project (ProjectName)
        IncludeDefaultAppDefinition(BuildCfg, ProjectName, DirVersion)

        uuid(ProjectID)
        
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
            BuildCfg.DirSourcesPugiXml,
            BuildCfg.DirSourcesImGui,
        }
        
        -- Libs directories
        filter { "system:windows", "action:codelite", "platforms:x86" }
			libdirs { BuildCfg.DirSourcesSfml.."extlibs/libs-mingw/x86", }
        filter { "system:windows", "action:codelite", "platforms:x64" }
			libdirs { BuildCfg.DirSourcesSfml.."extlibs/libs-mingw/x64", }

        filter { "system:windows", "action:vs2015 or vs2017 or vs2019", "platforms:x86" }
            libdirs { BuildCfg.DirSourcesSfml.."extlibs/libs-msvc-universal/x86", }
        filter { "system:windows", "action:vs2015 or vs2017 or vs2019", "platforms:x64" }
            libdirs { BuildCfg.DirSourcesSfml.."extlibs/libs-msvc-universal/x64", }
            
        -- Links
        filter { "configurations:Debug" }
            links { "GuguEngine-s-d", "ImGui-s-d", "SFML-s-d", "PugiXml-s-d" }
            
        filter { "configurations:Release" }
            links { "GuguEngine-s", "ImGui-s", "SFML-s", "PugiXml-s" }
            
        filter { "system:windows" }
			links { "freetype", "gdi32", "opengl32", "winmm", "openal32", "vorbisenc", "vorbisfile", "vorbis", "ogg", "flac", "ws2_32" }
            
        filter { "system:linux" }
            links { "pthread", "GL", "X11", "Xrandr", "freetype", "GLEW", "sndfile", "vorbisenc", "vorbisfile", "vorbis", "ogg", "FLAC", "openal", "udev" }
		
        -- Finalize
        filter {}
        
end

-- Template for a default SFML project
function ProjectDefaultSFML(BuildCfg, ProjectName, DirSources, DirVersion, ProjectID)
    
	DirSources = EnsureSlash(DirSources)
	
    project (ProjectName)
        IncludeDefaultAppDefinition(BuildCfg, ProjectName, DirVersion)
        
        uuid(ProjectID)
        
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
        filter { "system:windows", "action:codelite", "platforms:x86" }
			libdirs { BuildCfg.DirSourcesSfml.."extlibs/libs-mingw/x86", }
        filter { "system:windows", "action:codelite", "platforms:x64" }
			libdirs { BuildCfg.DirSourcesSfml.."extlibs/libs-mingw/x64", }

        filter { "system:windows", "action:vs2015 or vs2017 or vs2019", "platforms:x86" }
            libdirs { BuildCfg.DirSourcesSfml.."extlibs/libs-msvc-universal/x86", }
        filter { "system:windows", "action:vs2015 or vs2017 or vs2019", "platforms:x64" }
            libdirs { BuildCfg.DirSourcesSfml.."extlibs/libs-msvc-universal/x64", }
            
        -- Links
        filter { "configurations:Debug" }
            links { "SFML-s-d" }
            
        filter { "configurations:Release" }
            links { "SFML-s" }
            
        filter { "system:windows" }
			links { "freetype", "gdi32", "opengl32", "winmm", "openal32", "vorbisenc", "vorbisfile", "vorbis", "ogg", "flac", "ws2_32" }
            
        filter { "system:linux" }
            links { "pthread", "GL", "X11", "Xrandr", "freetype", "GLEW", "sndfile", "vorbisenc", "vorbisfile", "vorbis", "ogg", "FLAC", "openal", "udev" }

        -- Finalize
        filter {}
    
end
