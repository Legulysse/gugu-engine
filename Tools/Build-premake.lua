
-- Setup the include path of the engine build scripts directory
package.path = package.path .. ";./Build/?.lua"

require "PremakeUtility"


-- Solution Configuration
local pathExternals = EnsureSlash("../Externals")
local pathDev 		= EnsureSlash("../Dev")
local pathVersion 	= EnsureSlash("../Version")
local pathEditorVersion = EnsureSlash("../Tools/GuguEditor")

BuildCfg = {
    -- Engine
    DirSourcesEngine    = EnsureSlash(pathDev.."SourcesEngine"),
    DirSourcesSfml      = EnsureSlash(pathExternals.."SFML"),
    DirSourcesPugiXml   = EnsureSlash(pathExternals.."PugiXml"),
    DirSourcesImGui     = EnsureSlash(pathExternals.."ImGui"),
    DirSourcesImGuiSFML = EnsureSlash(pathExternals.."ImGuiSFML"),
    DirSourcesImGuiSetup = EnsureSlash(pathDev.."SourcesImGuiSetup"),
    
    -- Editor
    DirEditorVersion    = EnsureSlash(pathEditorVersion),
    DirSourcesEditorApp = EnsureSlash(pathDev.."SourcesEditorApp"),
    DirSourcesEditor    = EnsureSlash(pathDev.."SourcesEditor"),
}


-- Build Solution
solution "GuguEngine"
    IncludeDefaultSolutionDefinition(BuildCfg, pathDev.."Build")
    
    -- GUIDs can be generated from here : https://www.guidgenerator.com/online-guid-generator.aspx
    package.guid = "8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942"
    
    group "Demos/Game"
    ProjectDefault(BuildCfg, "DemoGame"         , pathDev.."SourcesDemos/Game/DemoGame"         	, pathVersion.."DemoGame", "9F765D56-4450-4839-9060-DF16801FA23C")
    
    group "Demos/HelloWorld"
    ProjectDefault(BuildCfg, "DemoHelloWorld"   , pathDev.."SourcesDemos/HelloWorld/DemoHelloWorld" , pathVersion.."DemoSystems", "3CA4F3A3-45C4-4DE3-96EA-C3E8CCD90501")
    
    group "Demos/Systems"
    ProjectDefault(BuildCfg, "DemoAnimation"    , pathDev.."SourcesDemos/Systems/DemoAnimation"     , pathVersion.."DemoSystems", "19F60F81-6283-40CE-A731-9B2FC34332AF")
    ProjectDefault(BuildCfg, "DemoAudio"        , pathDev.."SourcesDemos/Systems/DemoAudio"         , pathVersion.."DemoSystems", "151D215C-C0B5-4A5A-84D2-D006E7F2EADD")
    ProjectDefault(BuildCfg, "DemoCamera"       , pathDev.."SourcesDemos/Systems/DemoCamera"        , pathVersion.."DemoSystems", "332387A0-C90D-4716-8BC2-9514691771E0")
    ProjectDefault(BuildCfg, "DemoInterpolation", pathDev.."SourcesDemos/Systems/DemoInterpolation" , pathVersion.."DemoSystems", "303BEAEB-CF4F-4FE9-A8CD-A1643829ADEC")
    ProjectDefault(BuildCfg, "DemoUI"           , pathDev.."SourcesDemos/Systems/DemoUI"           	, pathVersion.."DemoSystems", "F7DC2C88-6154-47B4-8796-B04DCFE3DB08")
    ProjectDefault(BuildCfg, "DemoNetwork"      , pathDev.."SourcesDemos/Systems/DemoNetwork"      	, pathVersion.."DemoSystems", "72F68763-17C9-4CB2-8588-2C2A92DEFB1D")
    ProjectDefault(BuildCfg, "DemoRenderer"     , pathDev.."SourcesDemos/Systems/DemoRenderer"      , pathVersion.."DemoSystems", "A26326A1-4C97-40EA-BD03-D199EA97580A")
    ProjectDefault(BuildCfg, "DemoDatasheet"    , pathDev.."SourcesDemos/Systems/DemoDatasheet"    	, pathVersion.."DemoSystems", "07697378-509F-4FB9-9F09-D0EB4B45923D")
    ProjectDefault(BuildCfg, "DemoImGui"        , pathDev.."SourcesDemos/Systems/DemoImGui"   	    , pathVersion.."DemoSystems", "27168220-7E03-4AFA-8196-4821B7EDE8A5")
    ProjectDefault(BuildCfg, "DemoGrid"         , pathDev.."SourcesDemos/Systems/DemoGrid"   	    , pathVersion.."DemoSystems", "8D7C0EBA-3EC7-4C66-A56C-E2AC18633890")
    ProjectDefault(BuildCfg, "DemoParticles"    , pathDev.."SourcesDemos/Systems/DemoParticles"     , pathVersion.."DemoSystems", "16BBB10F-D922-4B4E-8F30-F28D743E0AF3")
    
    group "Demos/Tests"
    ProjectDefaultSFML(BuildCfg, "DemoSFML"         , pathDev.."SourcesDemos/Tests/DemoSFML"            , pathVersion.."DemoTests", "7F2F4292-8762-4C16-AB8B-6CA75D56169D")
    ProjectDefault(BuildCfg, "DemoBlackboard"   , pathDev.."SourcesDemos/Tests/DemoBlackboard"     	, pathVersion.."DemoTests", "A6F5963E-901A-4CE5-9A56-38F8FE448267")
    ProjectDefault(BuildCfg, "DemoMathUtility"  , pathDev.."SourcesDemos/Tests/DemoMathUtility"  	, pathVersion.."DemoTests", "65FF27AD-5B47-4B7D-AB52-7AC6FF39DCC3")
    ProjectDefault(BuildCfg, "DemoSystemUtility", pathDev.."SourcesDemos/Tests/DemoSystemUtility"   , pathVersion.."DemoTests", "C651D7AF-5018-438B-8508-E2353C82AC7C")
    ProjectDefault(BuildCfg, "UnitTests"        , pathDev.."SourcesDemos/Tests/UnitTests"           , pathVersion.."DemoTests", "86CC4BC0-7B66-4AA8-9038-0F791FC0A195")
    
    group "Editor App"
    ProjectAppGuguEditor(BuildCfg)
    
    group "Engine"
    ProjectLibGuguEngine(BuildCfg)
    ProjectLibGuguEditor(BuildCfg)
    
    group "Externals"
    ProjectLibSFML(BuildCfg)
    ProjectLibPugiXml(BuildCfg)
    ProjectLibImGui(BuildCfg)
