
-- Setup the include path of the engine build scripts directory
package.path = package.path .. ";../Tools/Build/?.lua"

require "PremakeEngine"
require "PremakeDemos"


-- Solution Configuration
local pathDev 		= EnsureSlash("Dev")
local pathDevEngine = EnsureSlash("../Dev")
local pathSolution 	= EnsureSlash(pathDev.."Build/".._ACTION)
local pathVersion 	= EnsureSlash("Version")

BuildCfg = {
    -- Solution
    DirSolution     = EnsureSlash(pathSolution),
    SubDirBinaries  = EnsureSlash("Build_".._ACTION),
    
    -- Engine
    DirSourcesEngine    = EnsureSlash(pathDevEngine.."SourcesEngine"),
    DirSourcesSfml      = EnsureSlash(pathDevEngine.."SourcesSFML"),
    DirSourcesPugiXml   = EnsureSlash(pathDevEngine.."SourcesPugiXml"),
    DirSourcesImGui     = EnsureSlash(pathDevEngine.."SourcesImGui"),
    DirLibEngine        = EnsureSlash(pathSolution.."Build"),
}


-- Build Solution
solution "SampleProject"

    location (BuildCfg.DirSolution)
    package.guid = "BEBB4888-E0C7-482A-9304-DD406237C589"
    configurations { "Debug", "Release" }
    platforms { "x86", "x64" }
    cppdialect "c++14"
    
    -- GUIDs can be generated from here : https://www.guidgenerator.com/online-guid-generator.aspx

    group "Application"
    ProjectDefault(BuildCfg, "SampleApplication", pathDev.."Sources", pathVersion, "6A983AD1-4D36-4949-B95D-B2FA4E7BC81D")
    
    group "Engine"
    ProjectLibGuguEngine(BuildCfg)
    
    group "Externals"
    ProjectLibSFML(BuildCfg)
    ProjectLibPugiXml(BuildCfg)
    ProjectLibImGui(BuildCfg)
