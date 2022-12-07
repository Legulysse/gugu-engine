
-- Setup the include path of the engine build scripts directory
package.path = package.path .. ";../Tools/Build/?.lua"

require "PremakeUtility"


-- Solution Configuration
local pathDev 		= EnsureSlash("Dev")
local pathDevEngine = EnsureSlash("../Dev")
local pathVersion 	= EnsureSlash("Version")

BuildCfg = {
    -- Engine
    DirSourcesEngine    = EnsureSlash(pathDevEngine.."SourcesEngine"),
    DirSourcesSfml      = EnsureSlash(pathDevEngine.."SourcesSFML"),
    DirSourcesPugiXml   = EnsureSlash(pathDevEngine.."SourcesPugiXml"),
    DirSourcesImGui     = EnsureSlash(pathDevEngine.."SourcesImGui"),
    DirSourcesImGuiSetup = EnsureSlash(pathDevEngine.."SourcesImGuiSetup"),
    DirSourcesImGuiSFML = EnsureSlash(pathDevEngine.."SourcesImGuiSFML"),
}


-- Build Solution
solution "SampleProject"
    IncludeDefaultSolutionDefinition(BuildCfg, pathDev.."Build")

    -- GUIDs can be generated from here : https://www.guidgenerator.com/online-guid-generator.aspx
    package.guid = "BEBB4888-E0C7-482A-9304-DD406237C589"

    group "Application"
    ProjectDefault(BuildCfg, "SampleApplication", pathDev.."Sources", pathVersion, "6A983AD1-4D36-4949-B95D-B2FA4E7BC81D")
    
    group "Engine"
    ProjectLibGuguEngine(BuildCfg)
    
    group "Externals"
    ProjectLibSFML(BuildCfg)
    ProjectLibPugiXml(BuildCfg)
    ProjectLibImGui(BuildCfg)
