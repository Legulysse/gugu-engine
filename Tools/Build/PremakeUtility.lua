
-- Template for a default project
function ProjectDefault(BuildCfg, ProjectName, DirSources, DirVersion, ProjectID)
    
	DirSources = EnsureSlash(DirSources)
	
    project (ProjectName)

        -- Base Definition
        IncludeDefaultAppDefinition(BuildCfg, ProjectName, DirVersion)

        uuid(ProjectID)
        
        -- Projects dependencies
        dependson { "GuguEngine" }
        
        -- Files
        IncludeEngineHeadersDefinition()
        
        files {
            DirSources.."**.h",
            DirSources.."**.hpp",
            DirSources.."**.cpp",
            DirSources.."**.tpp",
        }
        
        includedirs {
            DirSources,
        }
        
        -- Linker
        IncludeEngineLinkerDefinition()
        
        -- Options
        IncludeExtraWarnings()
        
        -- Finalize
        filter {}
        
end

-- Template for a default SFML project
function ProjectDefaultSFML(BuildCfg, ProjectName, DirSources, DirVersion, ProjectID)
    
	DirSources = EnsureSlash(DirSources)
	
    project (ProjectName)
    
        -- Base Definition
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
        
        includedirs {
            DirSources,
            BuildCfg.DirSourcesSfml.."include/",
        }
        
        -- Linker
        IncludeSFMLOnlyLinkerDefinition()
        
        -- Options
        IncludeExtraWarnings()
        
        -- Finalize
        filter {}
    
end

-- Project GuguEditor App
function ProjectAppGuguEditor(BuildCfg)
    
    project ("GuguEditorApp")
    
        -- Base Definition
        IncludeDefaultAppDefinition(BuildCfg, "GuguEditor", BuildCfg.DirEditorVersion)
    
        uuid "E4D3697E-E0B5-4343-B000-E895BACF446A"
        
        -- Projects dependencies
        dependson { "GuguEditor" }
        
        -- Files
        IncludeEngineHeadersDefinition()
        
        files {
            BuildCfg.DirSourcesEditorApp.."**.h",
            BuildCfg.DirSourcesEditorApp.."**.hpp",
            BuildCfg.DirSourcesEditorApp.."**.cpp",
            BuildCfg.DirSourcesEditorApp.."**.tpp",
        }
        
        includedirs {
            BuildCfg.DirSourcesEditorApp,
            BuildCfg.DirSourcesEditor,
        }
        
        -- Linker
        IncludeEngineWithEditorLinkerDefinition()
        
        -- Options
        IncludeExtraWarnings()
        
        -- Finalize
        filter {}
        
end

-- Project GuguEditor
function ProjectLibGuguEditor(BuildCfg)

    project "GuguEditor"
    
        -- Base Definition
        IncludeDefaultLibDefinition(BuildCfg, "GuguEditorLib")
        
        uuid "D56FC1A7-034F-4E7F-9DBB-B615C3C5C070"
        
        -- Projects dependencies
        dependson { "GuguEngine", "ImGui", "SFML", "PugiXml" }

        -- Files
        IncludeEngineHeadersDefinition()
        
        files {
            BuildCfg.DirSourcesEditor.."**.h",
            BuildCfg.DirSourcesEditor.."**.hpp",
            BuildCfg.DirSourcesEditor.."**.cpp",
            BuildCfg.DirSourcesEditor.."**.tpp",
        }
        
        includedirs {
            BuildCfg.DirSourcesEditor,
        }
		
        -- Options
        IncludeExtraWarnings()
        
end

-- Project GuguEngine
function ProjectLibGuguEngine(BuildCfg)

    project "GuguEngine"
    
        -- Base Definition
        IncludeDefaultLibDefinition(BuildCfg, "GuguEngine")
        
        uuid "59E650EC-0FD8-4D3C-A9E3-29DFF2AA5096"
        
        -- Projects dependencies
        dependson { "ImGui", "SFML", "PugiXml" }

        -- Files
        IncludeEngineHeadersDefinition()
        
        files {
            BuildCfg.DirSourcesEngine.."**.h",
            BuildCfg.DirSourcesEngine.."**.hpp",
            BuildCfg.DirSourcesEngine.."**.cpp",
            BuildCfg.DirSourcesEngine.."**.tpp",
        }
        
        -- Options
        IncludeExtraWarnings()
        
end

-- Project ImGui
function ProjectLibImGui(BuildCfg)

    DirSourcesImGui = BuildCfg.DirSourcesImGui
    DirSourcesImGuiSFML = BuildCfg.DirSourcesImGuiSFML
    DirSourcesImGuiSetup = BuildCfg.DirSourcesImGuiSetup

    project "ImGui"
    
        -- Base Definition
        IncludeDefaultLibDefinition(BuildCfg, "ImGui")
      
        uuid "11A07067-A137-4C3C-B6ED-F2DC8BE3639D"
        
        -- Projects dependencies
        dependson { "SFML" }

        -- Files
        files {
            DirSourcesImGui.."*.h",
            DirSourcesImGui.."*.cpp",
            DirSourcesImGui.."misc/cpp/*.h",
            DirSourcesImGui.."misc/cpp/*.cpp",
            DirSourcesImGuiSFML.."*.h",
            DirSourcesImGuiSFML.."*.cpp",
            DirSourcesImGuiSetup.."gugu-imconfig.h",
        }
    
        includedirs {
            BuildCfg.DirSourcesSfml.."include/",
            DirSourcesImGui,
            DirSourcesImGuiSFML,
            DirSourcesImGuiSetup,
        }

end

-- Project SFML
function ProjectLibSFML(BuildCfg)

    DirSfmlHeaders = BuildCfg.DirSourcesSfml.."include/"
    DirSfmlSources = BuildCfg.DirSourcesSfml.."src/"
    DirSfmlExternals = BuildCfg.DirSourcesSfml.."extlibs/"

    project "SFML"
    
        -- Base Definition
        IncludeDefaultLibDefinition(BuildCfg, "SFML")
        
        defines { "OV_EXCLUDE_STATIC_CALLBACKS", "FLAC__NO_DLL" }  -- Avoids warnings in vorbisfile.h
        uuid "936D68B9-FF55-CA40-9A14-7C2D95524D8B"
        
        -- Files
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
            DirSfmlExternals.."headers/freetype2",
            DirSfmlExternals.."headers/glad/include",
            DirSfmlExternals.."headers/minimp3",
            DirSfmlExternals.."headers/stb_image",
            DirSfmlExternals.."headers/vulkan",
        }

        filter { "system:windows" }
			excludes { 
                DirSfmlSources.."SFML/Main/SFMLActivity.cpp",
                DirSfmlSources.."SFML/Main/MainAndroid.cpp",
				DirSfmlSources.."SFML/Network/Unix/**",
				DirSfmlSources.."SFML/System/Android/**",
				DirSfmlSources.."SFML/System/Unix/**",
                DirSfmlSources.."SFML/Window/Android/**",
				DirSfmlSources.."SFML/Window/DRM/**",
				DirSfmlSources.."SFML/Window/FreeBSD/**",
                DirSfmlSources.."SFML/Window/iOS/**",
				DirSfmlSources.."SFML/Window/NetBSD/**",
				DirSfmlSources.."SFML/Window/OpenBSD/**",
				DirSfmlSources.."SFML/Window/OSX/**",
                DirSfmlSources.."SFML/Window/Unix/**",
            }
		
        filter { "system:linux" }
			excludes { 
                DirSfmlSources.."SFML/Main/**",
				DirSfmlSources.."SFML/Network/Win32/**",
				DirSfmlSources.."SFML/System/Android/**",
				DirSfmlSources.."SFML/System/Win32/**",
                DirSfmlSources.."SFML/Window/Android/**",
				DirSfmlSources.."SFML/Window/DRM/**",
				DirSfmlSources.."SFML/Window/FreeBSD/**",
                DirSfmlSources.."SFML/Window/iOS/**",
				DirSfmlSources.."SFML/Window/NetBSD/**",
				DirSfmlSources.."SFML/Window/OpenBSD/**",
				DirSfmlSources.."SFML/Window/OSX/**",
                DirSfmlSources.."SFML/Window/Win32/**",
            }
            
        -- Finalize
        filter {}
        
end

-- Project PugiXml
function ProjectLibPugiXml(BuildCfg)

    DirPugiXmlSources = BuildCfg.DirSourcesPugiXml.."src/"

    project "PugiXml"
    
        -- Base Definition
        IncludeDefaultLibDefinition(BuildCfg, "PugiXml")
        
        uuid "2961203D-3842-4A18-9129-7295D98964CF"
        
        -- Files
        files {
            DirPugiXmlSources.."**.hpp",
            DirPugiXmlSources.."**.cpp",
        }
        
        includedirs {
            DirPugiXmlSources,
        }
end

-- Helper for BuildCfg setup
function SetupBuildCfg(pathEngineRoot)

    pathEngineRoot = EnsureSlash(pathEngineRoot)

    BuildCfg = {
        -- Engine
        DirSourcesEngine    = EnsureSlash(pathEngineRoot.."Dev/SourcesEngine"),
        DirSourcesImGuiSetup = EnsureSlash(pathEngineRoot.."Dev/SourcesImGuiSetup"),
        DirSourcesSfml      = EnsureSlash(pathEngineRoot.."Externals/SFML"),
        DirSourcesPugiXml   = EnsureSlash(pathEngineRoot.."Externals/PugiXml"),
        DirSourcesImGui     = EnsureSlash(pathEngineRoot.."Externals/ImGui"),
        DirSourcesImGuiSFML = EnsureSlash(pathEngineRoot.."Externals/ImGuiSFML"),
    }
    
    return BuildCfg
end

function SetupBuildCfgWithEditor(pathEngineRoot, pathEditorVersion)

    pathEngineRoot = EnsureSlash(pathEngineRoot)
    pathEditorVersion = EnsureSlash(pathEditorVersion)

    BuildCfg = {
        -- Engine
        DirSourcesEngine    = EnsureSlash(pathEngineRoot.."Dev/SourcesEngine"),
        DirSourcesImGuiSetup = EnsureSlash(pathEngineRoot.."Dev/SourcesImGuiSetup"),
        DirSourcesSfml      = EnsureSlash(pathEngineRoot.."Externals/SFML"),
        DirSourcesPugiXml   = EnsureSlash(pathEngineRoot.."Externals/PugiXml"),
        DirSourcesImGui     = EnsureSlash(pathEngineRoot.."Externals/ImGui"),
        DirSourcesImGuiSFML = EnsureSlash(pathEngineRoot.."Externals/ImGuiSFML"),
        
        -- Editor
        DirEditorVersion    = EnsureSlash(pathEditorVersion),
        DirSourcesEditorApp = EnsureSlash(pathEngineRoot.."Dev/SourcesEditorApp"),
        DirSourcesEditor    = EnsureSlash(pathEngineRoot.."Dev/SourcesEditor"),
    }
    
    return BuildCfg
end


-- Default Solution
function IncludeDefaultSolutionDefinition(BuildCfg, DirSolution)

	DirSolution = EnsureSlash(DirSolution)
    
    -- Base Definition
    location (DirSolution.._ACTION)
    configurations { "Debug", "Release" }   -- "Debug", "Release"
    platforms { "x64" }                     -- "x86", "x64"
    cppdialect "c++17"                      -- "c++14", "c++17", "c++20"
    
end

-- Default App Project
function IncludeDefaultAppDefinition(BuildCfg, TargetName, DirVersion)

	DirVersion = EnsureSlash(DirVersion)
    SubDirBinaries  = EnsureSlash("Build_%{cfg.platform}_".._ACTION)
    
    -- Base Definition
    language "C++"
    defines { "IMGUI_USER_CONFIG=\"gugu-imconfig.h\"", "IMGUI_DISABLE_INCLUDE_IMCONFIG_H" }
    defines { "SFML_STATIC" }
    defines { "_CRT_SECURE_NO_WARNINGS", "UNICODE", "_UNICODE" }
    systemversion "latest"
    characterset "Unicode"
    
    -- Build Directories
    objdir("%{wks.location}/obj")
    targetdir(DirVersion..SubDirBinaries)
    debugdir(DirVersion)
    
    libdirs { "%{wks.location}/bin/%{cfg.platform}_%{cfg.buildcfg}" }

    -- Target Definitions    
    filter { "configurations:Debug" }
        kind "ConsoleApp"
        defines { "DEBUG" }
        flags { "NoMinimalRebuild", "MultiProcessorCompile" }
        symbols "On"
        targetname (TargetName.."-d")
        
    filter { "configurations:Release" }
        kind "WindowedApp"
        defines { "NDEBUG" }
        flags { "NoMinimalRebuild", "MultiProcessorCompile" }
        optimize "On"
        targetname (TargetName)
        
    filter { "platforms:x86" }
        architecture "x86"
    filter { "platforms:x64" }
        architecture "x86_64"
        
    -- Finalize
    filter {}
    
end

-- Default Lib Project
function IncludeDefaultLibDefinition(BuildCfg, TargetName)

    -- Base Definition
    kind "StaticLib"
    language "C++"
    defines { "IMGUI_USER_CONFIG=\"gugu-imconfig.h\"", "IMGUI_DISABLE_INCLUDE_IMCONFIG_H" }
    defines { "SFML_STATIC" }
    defines { "_CRT_SECURE_NO_WARNINGS", "UNICODE", "_UNICODE" }
    systemversion "latest"
    characterset "Unicode"
    
    -- Build Directories
    objdir("%{wks.location}/obj")
    targetdir("%{wks.location}/bin/%{cfg.platform}_%{cfg.buildcfg}")

    libdirs { "%{wks.location}/bin/%{cfg.platform}_%{cfg.buildcfg}" }

    -- Target Definitions
    filter { "configurations:Debug" }
        defines { "DEBUG", "_DEBUG" }
        flags { "NoMinimalRebuild", "MultiProcessorCompile" }
        symbols "On"
        targetname (TargetName.."-s-d")

    filter { "configurations:Release" }
        defines { "NDEBUG" }
        flags { "NoMinimalRebuild", "MultiProcessorCompile" }
        optimize "On"
        targetname (TargetName.."-s")
        
    filter { "platforms:x86" }
        architecture "x86"
    filter { "platforms:x64" }
        architecture "x86_64"
        
    -- Finalize
    filter {}
    
end

function IncludeEngineHeadersDefinition()

    includedirs {
        BuildCfg.DirSourcesEngine,
        BuildCfg.DirSourcesSfml.."include/",
        BuildCfg.DirSourcesPugiXml.."src/",
        BuildCfg.DirSourcesImGui,
        BuildCfg.DirSourcesImGuiSFML,
        BuildCfg.DirSourcesImGuiSetup,
    }
    
end

function IncludeEngineWithEditorLinkerDefinition()
    IncludeLinkerDefinitions(true, true)
end

function IncludeEngineLinkerDefinition()
    IncludeLinkerDefinitions(true, false)
end

function IncludeSFMLOnlyLinkerDefinition()
    IncludeLinkerDefinitions(false, false)
end

function IncludeLinkerDefinitions(IncludeEngine, IncludeEditor)

    -- Link directories
    filter { "system:windows", "action:codelite", "platforms:x86" }
        libdirs { BuildCfg.DirSourcesSfml.."extlibs/libs-mingw/x86", }
    filter { "system:windows", "action:codelite", "platforms:x64" }
        libdirs { BuildCfg.DirSourcesSfml.."extlibs/libs-mingw/x64", }

    filter { "system:windows", "action:vs2013", "platforms:x86" }
        libdirs { BuildCfg.DirSourcesSfml.."extlibs/libs-msvc/x86", }
    filter { "system:windows", "action:vs2013", "platforms:x64" }
        libdirs { BuildCfg.DirSourcesSfml.."extlibs/libs-msvc/x64", }
        
    filter { "system:windows", "action:vs2015 or vs2017 or vs2019 or vs2022", "platforms:x86" }
        libdirs { BuildCfg.DirSourcesSfml.."extlibs/libs-msvc-universal/x86", }
    filter { "system:windows", "action:vs2015 or vs2017 or vs2019 or vs2022", "platforms:x64" }
        libdirs { BuildCfg.DirSourcesSfml.."extlibs/libs-msvc-universal/x64", }
        
    -- Link libraries
    if IncludeEditor then
        filter { "configurations:Debug" }
            links { "GuguEditorLib-s-d" }
        filter { "configurations:Release" }
            links { "GuguEditorLib-s" }
    end
    
    if IncludeEngine then
        filter { "configurations:Debug" }
            links { "GuguEngine-s-d", "ImGui-s-d","PugiXml-s-d" }
        filter { "configurations:Release" }
            links { "GuguEngine-s", "ImGui-s", "PugiXml-s" }
    end
    
    filter { "configurations:Debug" }
        links { "SFML-s-d" }
    filter { "configurations:Release" }
        links { "SFML-s" }

    filter { "system:windows" }
        links { "legacy_stdio_definitions" }  -- fix: error LNK2019: unresolved external symbol _sprintf
        links { "freetype", "gdi32", "opengl32", "winmm", "openal32", "vorbisenc", "vorbisfile", "vorbis", "ogg", "flac", "ws2_32" }
        
    filter { "system:linux" }
        links { "pthread", "GL", "X11", "Xrandr", "freetype", "GLEW", "sndfile", "vorbisenc", "vorbisfile", "vorbis", "ogg", "FLAC", "openal", "udev" }
    
    -- Finalize
    filter {}
    
end

-- Extra warnings (only for visual studio)
function IncludeExtraWarnings()

    -- Options
    filter { "system:windows", "action:vs2013 or vs2015 or vs2017 or vs2019 or vs2022" }
        warnings "Extra"
        disablewarnings { "4100", "4189" } -- 4100 = unreferenced formal parameter, 4189 = local variable is initialized but not referenced

    -- Finalize
    filter {}
    
end

-- Path Utility
function EnsureSlash(path)
	if string.len(path) > 0 and string.sub(path, -1) ~= "/" then
		return path.."/"
	end
	return path
end
