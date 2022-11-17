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
        files {
            DirSources.."**.h",
            DirSources.."**.hpp",
            DirSources.."**.cpp",
            DirSources.."**.tpp",
        }
        
        includedirs {
            DirSources,
            BuildCfg.DirSourcesEngine,
            BuildCfg.DirSourcesSfml.."include/",
            BuildCfg.DirSourcesPugiXml,
            BuildCfg.DirSourcesImGui,
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
