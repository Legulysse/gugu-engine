
-- Template for a default project
function ProjectDefault(BuildCfg, ProjectName, DirSources, DirVersion, ProjectID)
    
    DirSources = EnsureSlash(DirSources)
    
    project (ProjectName)

        -- Base Definition
        IncludeDefaultAppDefinition(BuildCfg, ProjectName, DirSources, DirVersion)
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
        IncludeDefaultAppDefinition(BuildCfg, ProjectName, DirSources, DirVersion)
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
        IncludeDefaultAppDefinition(BuildCfg, "GuguEditor", BuildCfg.DirSourcesEditorApp, BuildCfg.DirEditorVersion)
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
    DirSfmlDependencies = BuildCfg.DirSourcesSfml.."../Dependencies/"

    project "SFML"
    
        -- Base Definition
        IncludeDefaultLibDefinition(BuildCfg, "SFML")
        uuid "936D68B9-FF55-CA40-9A14-7C2D95524D8B"
        
        defines { "OV_EXCLUDE_STATIC_CALLBACKS", "FLAC__NO_DLL" }       -- Avoids warnings in vorbisfile.h
        defines { "MA_NO_MP3", "MA_NO_FLAC", "MA_NO_ENCODING", 
                    "MA_NO_RESOURCE_MANAGER", "MA_NO_GENERATION" }      -- Disable miniaudio features we do not use
        defines { "MA_USE_STDINT" }                                     -- use standard fixed-width integer types
        defines { "STBI_FAILURE_USERMSG" }                              -- Add preprocessor symbols (Graphics module)
        defines { "SFML_IS_BIG_ENDIAN=0" }                              -- Detect the endianness as required by Ogg (big endian seems to be used only on systems I wont need)
        defines { "FT2_BUILD_LIBRARY" }                                 -- Freetype setup
        
        --# define NDEBUG when building FLAC to suppress console debug output
        --target_compile_definitions(FLAC PRIVATE NDEBUG)

        -- Projects dependencies
        dependson { "Freetype" }

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
            DirSfmlExternals.."headers/glad/include",
            DirSfmlExternals.."headers/mingw",
            DirSfmlExternals.."headers/miniaudio",
            DirSfmlExternals.."headers/minimp3",
            DirSfmlExternals.."headers/stb_image",
            DirSfmlExternals.."headers/vulkan",
            DirSfmlDependencies.."Flac/include/",
            DirSfmlDependencies.."Freetype/include/",
            DirSfmlDependencies.."Ogg/include/",
            DirSfmlDependencies.."Vorbis/include/",
        }

        filter { "system:windows" }
            excludes { 
                DirSfmlSources.."SFML/Main/MainAndroid.cpp",
                DirSfmlSources.."SFML/Network/Unix/**",
                DirSfmlSources.."SFML/System/Android/**",
                DirSfmlSources.."SFML/System/Unix/**",
                DirSfmlSources.."SFML/Window/Android/**",
                DirSfmlSources.."SFML/Window/DRM/**",
                DirSfmlSources.."SFML/Window/FreeBSD/**",
                DirSfmlSources.."SFML/Window/iOS/**",
                DirSfmlSources.."SFML/Window/macOS/**",
                DirSfmlSources.."SFML/Window/NetBSD/**",
                DirSfmlSources.."SFML/Window/OpenBSD/**",
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
                DirSfmlSources.."SFML/Window/macOS/**",
                DirSfmlSources.."SFML/Window/NetBSD/**",
                DirSfmlSources.."SFML/Window/OpenBSD/**",
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

-- Project Freetype
function ProjectLibFreetype(BuildCfg)

    DirSfmlDependencies = BuildCfg.DirSourcesSfml.."../Dependencies/"

    project "Freetype"
    
        -- Base Definition
        IncludeDependencyLibDefinition(BuildCfg, "Freetype")
        uuid "650FEA3C-9149-444B-AE31-89BE7949C919"
        
        defines { "FT2_BUILD_LIBRARY" }     -- Freetype setup
        
        -- Files
        files {
            DirSfmlDependencies.."Freetype/src/autofit/autofit.c",
            DirSfmlDependencies.."Freetype/src/base/ftbase.c",
            DirSfmlDependencies.."Freetype/src/base/ftbbox.c",
            DirSfmlDependencies.."Freetype/src/base/ftbdf.c",
            DirSfmlDependencies.."Freetype/src/base/ftbitmap.c",
            DirSfmlDependencies.."Freetype/src/base/ftcid.c",
            DirSfmlDependencies.."Freetype/src/base/ftfstype.c",
            DirSfmlDependencies.."Freetype/src/base/ftgasp.c",
            DirSfmlDependencies.."Freetype/src/base/ftglyph.c",
            DirSfmlDependencies.."Freetype/src/base/ftgxval.c",
            DirSfmlDependencies.."Freetype/src/base/ftinit.c",
            DirSfmlDependencies.."Freetype/src/base/ftmm.c",
            DirSfmlDependencies.."Freetype/src/base/ftotval.c",
            DirSfmlDependencies.."Freetype/src/base/ftpatent.c",
            DirSfmlDependencies.."Freetype/src/base/ftpfr.c",
            DirSfmlDependencies.."Freetype/src/base/ftstroke.c",
            DirSfmlDependencies.."Freetype/src/base/ftsynth.c",
            DirSfmlDependencies.."Freetype/src/base/fttype1.c",
            DirSfmlDependencies.."Freetype/src/base/ftwinfnt.c",
            DirSfmlDependencies.."Freetype/src/bdf/bdf.c",
            DirSfmlDependencies.."Freetype/src/bzip2/ftbzip2.c",
            DirSfmlDependencies.."Freetype/src/cache/ftcache.c",
            DirSfmlDependencies.."Freetype/src/cff/cff.c",
            DirSfmlDependencies.."Freetype/src/cid/type1cid.c",
            DirSfmlDependencies.."Freetype/src/gzip/ftgzip.c",
            DirSfmlDependencies.."Freetype/src/lzw/ftlzw.c",
            DirSfmlDependencies.."Freetype/src/pcf/pcf.c",
            DirSfmlDependencies.."Freetype/src/pfr/pfr.c",
            DirSfmlDependencies.."Freetype/src/psaux/psaux.c",
            DirSfmlDependencies.."Freetype/src/pshinter/pshinter.c",
            DirSfmlDependencies.."Freetype/src/psnames/psnames.c",
            DirSfmlDependencies.."Freetype/src/raster/raster.c",
            DirSfmlDependencies.."Freetype/src/sdf/sdf.c",
            DirSfmlDependencies.."Freetype/src/sfnt/sfnt.c",
            DirSfmlDependencies.."Freetype/src/smooth/smooth.c",
            DirSfmlDependencies.."Freetype/src/svg/svg.c",
            DirSfmlDependencies.."Freetype/src/truetype/truetype.c",
            DirSfmlDependencies.."Freetype/src/type1/type1.c",
            DirSfmlDependencies.."Freetype/src/type42/type42.c",
            DirSfmlDependencies.."Freetype/src/winfonts/winfnt.c",
        }

        filter { "system:windows" }
            -- "builds/unix/ftsystem.c"
            -- "builds/windows/ftsystem.c"
            -- "src/base/ftsystem.c"
            files {
                DirSfmlDependencies.."Freetype/builds/windows/ftsystem.c",
            }
        filter { "system:windows" }
            -- "builds/windows/ftdebug.c"
            -- "src/base/ftdebug.c"
            files {
                DirSfmlDependencies.."Freetype/builds/windows/ftdebug.c",
            }
        filter {}
        
        includedirs {
            DirSfmlDependencies.."Freetype/include/",
        }

        -- Finalize
        filter {}
        
end

-- Helper for BuildCfg setup
function SetupBuildCfg(pathEngineRoot)

    pathEngineRoot = EnsureSlash(pathEngineRoot)

    BuildCfg = {
        -- Build options.
        ImGui_DisableObsoleteFunctions = false,

        -- Engine directories.
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
        -- Build options.
        ImGui_DisableObsoleteFunctions = false,

        -- Engine directories.
        DirSourcesEngine    = EnsureSlash(pathEngineRoot.."Dev/SourcesEngine"),
        DirSourcesImGuiSetup = EnsureSlash(pathEngineRoot.."Dev/SourcesImGuiSetup"),
        DirSourcesSfml      = EnsureSlash(pathEngineRoot.."Externals/SFML"),
        DirSourcesPugiXml   = EnsureSlash(pathEngineRoot.."Externals/PugiXml"),
        DirSourcesImGui     = EnsureSlash(pathEngineRoot.."Externals/ImGui"),
        DirSourcesImGuiSFML = EnsureSlash(pathEngineRoot.."Externals/ImGuiSFML"),
        
        -- Editor directories.
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

    configurations { "DevDebug", "DevRelease", "ProdMaster" } 	-- "Debug", "Release", "Master" (Visual sorts configurations alphabetically)
    platforms { "x64" }  	-- "x86", "x64"
    cppdialect "c++17"   	-- "c++14", "c++17", "c++20"
    
end

-- Default App Project
function IncludeDefaultAppDefinition(BuildCfg, TargetName, DirSources, DirVersion)

    DirVersion = EnsureSlash(DirVersion)
    DirSources = EnsureSlash(DirSources)
    SubDirBinaries  = EnsureSlash("Build_%{cfg.platform}_".._ACTION)
    
    -- Base Definition
    language "C++"
    defines { "IMGUI_USER_CONFIG=\"gugu-imconfig.h\"" }
    if BuildCfg.ImGui_DisableObsoleteFunctions then
        defines { "IMGUI_DISABLE_OBSOLETE_FUNCTIONS" }
    end
    defines { "SFML_STATIC" }                                       -- specify if SFML is used as a static lib
    defines { "_CRT_SECURE_NO_WARNINGS", "_CRT_NONSTDC_NO_WARNINGS", "UNICODE", "_UNICODE" }
    defines { "_WINSOCK_DEPRECATED_NO_WARNINGS" }                   -- disable winsock deprecation warnings
    flags { "MultiProcessorCompile" }   -- /MP
    systemversion "latest"
    characterset "Unicode"
	staticruntime "Off"     -- Use /MD[d] instead of /MT[d]
    
    -- Build Directories
    objdir("%{wks.location}/obj")
    targetdir(DirVersion..SubDirBinaries)
    debugdir(DirVersion)
    
    libdirs { "%{wks.location}/bin/%{cfg.platform}_%{cfg.buildcfg}" }

    -- Target Definitions    
    filter { "configurations:DevDebug" }
        kind "ConsoleApp"
        defines { "GUGU_DEBUG", "_DEBUG" }
        symbols "On"
        runtime "Debug"     -- /MDd
        targetname (TargetName.."-d")
        
    filter { "configurations:DevRelease" }
        kind "WindowedApp"
        defines { "GUGU_RELEASE", "NDEBUG" }
        symbols "On"
        optimize "On"
        runtime "Release"   -- /MD
        targetname (TargetName.."-r")
        
    filter { "configurations:ProdMaster" }
        kind "WindowedApp"
        defines { "GUGU_MASTER", "NDEBUG" }
        symbols "Off"
        optimize "On"
        runtime "Release"   -- /MD
        targetname (TargetName)
        
    filter { "platforms:x86" }
        architecture "x86"
    filter { "platforms:x64" }
        architecture "x86_64"


    -- Resources (Icon)
    filter { 'system:windows' }
        files {
            DirSources.."**.rc",
            DirSources.."**.ico",
        }
        vpaths { ["Resources/*"] = { "**.rc", "**.ico" } }

    -- Finalize
    filter {}
    
end

-- Default Lib Project
function IncludeDefaultLibDefinition(BuildCfg, TargetName)

    -- Base Definition
    kind "StaticLib"
    language "C++"
    defines { "IMGUI_USER_CONFIG=\"gugu-imconfig.h\"" }
    if BuildCfg.ImGui_DisableObsoleteFunctions then
        defines { "IMGUI_DISABLE_OBSOLETE_FUNCTIONS" }
    end
    defines { "SFML_STATIC" }                                       -- specify if SFML is used as a static lib
    defines { "_CRT_SECURE_NO_WARNINGS", "_CRT_NONSTDC_NO_WARNINGS", "UNICODE", "_UNICODE" }
    defines { "_WINSOCK_DEPRECATED_NO_WARNINGS" }                   -- disable winsock deprecation warnings
    flags { "MultiProcessorCompile" }   -- /MP
    systemversion "latest"
    characterset "Unicode"
	staticruntime "Off"     -- Use /MD[d] instead of /MT[d]
    
    -- Build Directories
    objdir("%{wks.location}/obj")
    targetdir("%{wks.location}/bin/%{cfg.platform}_%{cfg.buildcfg}")

    libdirs { "%{wks.location}/bin/%{cfg.platform}_%{cfg.buildcfg}" }

    -- Target Definitions
    filter { "configurations:DevDebug" }
        defines { "GUGU_DEBUG", "_DEBUG" }
        symbols "On"
        runtime "Debug"     -- /MDd
        targetname (TargetName.."-s-d")

    filter { "configurations:DevRelease" }
        defines { "GUGU_RELEASE", "NDEBUG" }
        symbols "On"
        optimize "On"
        runtime "Release"   -- /MD
        targetname (TargetName.."-s-r")
        
    filter { "configurations:ProdMaster" }
        defines { "GUGU_MASTER", "NDEBUG" }
        symbols "Off"
        optimize "On"
        runtime "Release"   -- /MD
        targetname (TargetName.."-s")
        
    filter { "platforms:x86" }
        architecture "x86"
    filter { "platforms:x64" }
        architecture "x86_64"
        
    -- Finalize
    filter {}
    
end

-- Dependency Lib Project
function IncludeDependencyLibDefinition(BuildCfg, TargetName)

    -- Base Definition
    kind "StaticLib"
    language "C++"
    defines { "_CRT_SECURE_NO_WARNINGS", "_CRT_NONSTDC_NO_WARNINGS", "UNICODE", "_UNICODE" }
    defines { "_WINSOCK_DEPRECATED_NO_WARNINGS" }                   -- disable winsock deprecation warnings
    flags { "MultiProcessorCompile" }   -- /MP
    systemversion "latest"
    characterset "Unicode"
	staticruntime "Off"     -- Use /MD[d] instead of /MT[d]
    
    -- Build Directories
    objdir("%{wks.location}/obj")
    targetdir("%{wks.location}/bin/%{cfg.platform}_%{cfg.buildcfg}")

    libdirs { "%{wks.location}/bin/%{cfg.platform}_%{cfg.buildcfg}" }

    -- Target Definitions
    filter { "configurations:DevDebug" }
        defines { "NDEBUG" }
        symbols "Off"
        optimize "On"
        runtime "Release"   -- /MD
        targetname (TargetName.."-s-d")

    filter { "configurations:DevRelease" }
        defines { "NDEBUG" }
        symbols "Off"
        optimize "On"
        runtime "Release"   -- /MD
        targetname (TargetName.."-s-r")
        
    filter { "configurations:ProdMaster" }
        defines { "NDEBUG" }
        symbols "Off"
        optimize "On"
        runtime "Release"   -- /MD
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
        filter { "configurations:DevDebug" }
            links { "GuguEditorLib-s-d" }
        filter { "configurations:DevRelease" }
            links { "GuguEditorLib-s-r" }
        filter { "configurations:ProdMaster" }
            links { "GuguEditorLib-s" }
    end
    
    if IncludeEngine then
        filter { "configurations:DevDebug" }
            links { "GuguEngine-s-d", "ImGui-s-d","PugiXml-s-d" }
        filter { "configurations:DevRelease" }
            links { "GuguEngine-s-r", "ImGui-s-r", "PugiXml-s-r" }
        filter { "configurations:ProdMaster" }
            links { "GuguEngine-s", "ImGui-s", "PugiXml-s" }
    end
    
    filter { "configurations:DevDebug" }
        links { "SFML-s-d", "Freetype-s-d" }
    filter { "configurations:DevRelease" }
        links { "SFML-s-r", "Freetype-s-r" }
    filter { "configurations:ProdMaster" }
        links { "SFML-s", "Freetype-s" }

    filter { "system:windows" }
        links { "legacy_stdio_definitions" }  -- fix: error LNK2019: unresolved external symbol _sprintf
        --links { "freetype" }
        links { "gdi32", "opengl32", "winmm" }
        --links { "openal32" }
        links { "vorbisenc", "vorbisfile", "vorbis", "ogg", "flac" }
        links { "ws2_32" }
        
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
        disablewarnings { "4100" } -- unreferenced formal parameter
        disablewarnings { "4189" } -- local variable is initialized but not referenced

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
