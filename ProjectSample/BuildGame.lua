
-- Step 1 : setup the include path of the engine build scripts directory
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
    DirLibEngine        = EnsureSlash(pathSolution.."Build"),
}


-- Build Solution
solution "GameProject"

    location (BuildCfg.DirSolution)
    package.guid = "BEBB4888-E0C7-482A-9304-DD406237C589"
    configurations { "Debug", "Release" }
    platforms { "x86", "x64" }
    
    -- GUIDs can be generated from here : https://www.guidgenerator.com/online-guid-generator.aspx

    ProjectDefault(BuildCfg, "Game", pathDev.."Sources", pathVersion, "6A983AD1-4D36-4949-B95D-B2FA4E7BC81D")
    
    ProjectLibGuguEngine(BuildCfg)
    ProjectLibSFML(BuildCfg)
    ProjectLibPugiXml(BuildCfg)
