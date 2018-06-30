require "PremakeEngine"
require "PremakeDemos"


-- Solution Configuration
local pathDev 		= EnsureSlash("../../Dev")
local pathSolution 	= EnsureSlash(pathDev.."Build/".._ACTION)
local pathVersion 	= EnsureSlash("../../Version")

BuildCfg = {
    -- Solution
    DirSolution     = EnsureSlash(pathSolution),
    SubDirBinaries  = EnsureSlash("Build_".._ACTION),
    
    -- Engine
    DirExternals        = EnsureSlash(pathDev.."External"),
    DirSourcesEngine    = EnsureSlash(pathDev.."SourcesEngine"),
    DirSourcesSfml      = EnsureSlash(pathDev.."SourcesSFML"),
    DirSourcesPugiXml   = EnsureSlash(pathDev.."SourcesPugiXml"),
    DirLibEngine        = EnsureSlash(pathSolution.."Build"),
}


-- Build Solution
solution "GuguEngine"

    location (BuildCfg.DirSolution)
    package.guid = "8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942"
    configurations { "Debug", "Release" }
    platforms { "x86", "x64" }
    
    -- GUIDs can be generated from here : https://www.guidgenerator.com/online-guid-generator.aspx

    group "Demo Game"
    ProjectDefault(BuildCfg, "DemoGame"       , pathDev.."SourcesDemos/DemoGame"         	, pathVersion.."DemoGame", "9F765D56-4450-4839-9060-DF16801FA23C")
    
    group "Demo Tests"
    ProjectDefault(BuildCfg, "DemoHelloWorld" , pathDev.."SourcesDemos/DemoHelloWorld"   	, pathVersion.."DemoTests", "3CA4F3A3-45C4-4DE3-96EA-C3E8CCD90501")
    ProjectDefault(BuildCfg, "DemoAnimation"  , pathDev.."SourcesDemos/DemoAnimation"      	, pathVersion.."DemoTests", "19F60F81-6283-40CE-A731-9B2FC34332AF")
    ProjectDefault(BuildCfg, "DemoAudio"      , pathDev.."SourcesDemos/DemoAudio"        	, pathVersion.."DemoTests", "151D215C-C0B5-4A5A-84D2-D006E7F2EADD")
    ProjectDefault(BuildCfg, "DemoCamera"     , pathDev.."SourcesDemos/DemoCamera"       	, pathVersion.."DemoTests", "332387A0-C90D-4716-8BC2-9514691771E0")
    ProjectDefault(BuildCfg, "DemoUI"         , pathDev.."SourcesDemos/DemoUI"           	, pathVersion.."DemoTests", "F7DC2C88-6154-47B4-8796-B04DCFE3DB08")
    ProjectDefault(BuildCfg, "DemoNetwork"    , pathDev.."SourcesDemos/DemoNetwork"      	, pathVersion.."DemoTests", "72F68763-17C9-4CB2-8588-2C2A92DEFB1D")
    ProjectDefault(BuildCfg, "DemoRenderer"   , pathDev.."SourcesDemos/DemoRenderer"      	, pathVersion.."DemoTests", "A26326A1-4C97-40EA-BD03-D199EA97580A")
    ProjectDefault(BuildCfg, "DemoDatasheet"  , pathDev.."SourcesDemos/DemoDatasheet"    	, pathVersion.."DemoTests", "07697378-509F-4FB9-9F09-D0EB4B45923D")
    ProjectDefault(BuildCfg, "DemoMathUtility", pathDev.."SourcesDemos/DemoMathUtility"  	, pathVersion.."DemoTests", "65FF27AD-5B47-4B7D-AB52-7AC6FF39DCC3")
    ProjectDefault(BuildCfg, "DemoSystemUtility", pathDev.."SourcesDemos/DemoSystemUtility" , pathVersion.."DemoTests", "C651D7AF-5018-438B-8508-E2353C82AC7C")
    ProjectDefault(BuildCfg, "DemoTestSizes"  , pathDev.."SourcesDemos/DemoTestSizes"       , pathVersion.."DemoTests", "A9A0A24C-014D-4EA3-AA4F-FCCCCC9B0F26")
    ProjectDefault(BuildCfg, "DemoCallback"   , pathDev.."SourcesDemos/DemoCallback"     	, pathVersion.."DemoTests", "A6F5963E-901A-4CE5-9A56-38F8FE448267")
    
    group "Engine"
    ProjectLibGuguEngine(BuildCfg)
    
    group "External"
    ProjectLibSFML(BuildCfg)
    ProjectLibPugiXml(BuildCfg)
