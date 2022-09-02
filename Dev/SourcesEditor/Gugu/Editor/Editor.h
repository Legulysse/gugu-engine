#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Editor/Core/EditorConfig.h"

#include "Gugu/Misc/Pattern/Singleton.h"

#include <vector>
#include <string>

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    struct ProjectSettings;
    class DeltaTime;
    class BaseModalDialog;
    class DocumentPanel;
    class AssetsExplorerPanel;
    class OutputLogPanel;
    class DependenciesPanel;
    class ImageSetPanel;
    class DatasheetParser;
}

namespace sf
{
    class Event;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {
    
class Editor : public Singleton<Editor>
{
public:

    Editor();
    virtual ~Editor();

    void Init(const EditorConfig& editorConfig);
    void Release();

    const EditorConfig& GetEditorConfig() const;

    void Update(const DeltaTime& dt);
    bool OnSFEvent(const sf::Event& event);

    void OpenProject(const std::string& projectPathFile);
    bool CloseProject();
    bool IsProjectOpen() const;
    const ProjectSettings* GetProjectSettings() const;

    bool OpenModalDialog(BaseModalDialog* modalDialog);
    bool OpenDocument(const std::string& resourceID);
    bool CloseDocument(const std::string& resourceID, bool forceIgnoreDirty);
    const std::vector<DocumentPanel*>& GetDocuments() const;
    DocumentPanel* GetLastActiveDocument() const;

    void RefreshAssets();
    void ResetPanels();

    bool CloseEditor();

    DatasheetParser* GetDatasheetParser() const;

private:

    bool RaiseCheckDirtyDocuments();
    void CancelClosingDirtyDocuments();
    void ValidateClosingDirtyDocuments();

    bool SaveActiveDocument();
    bool SaveAllDirtyDocuments();
    bool SaveAllClosingDirtyDocuments();

    bool UndoActiveDocument();
    bool RedoActiveDocument();

    void OpenProjectImpl(const std::string& projectPathFile);
    void CloseProjectImpl();
    void CloseEditorImpl();

private:

    EditorConfig m_editorConfig;

    ProjectSettings* m_project;
    std::string m_pendingOpenProjectPathFile;

    bool m_checkDirtyDocuments;
    bool m_pendingCloseEditor;
    bool m_pendingCloseProject;
    bool m_pendingCloseDocument;

    bool m_resetPanels;
    bool m_showImGuiDemo;

    AssetsExplorerPanel* m_assetsExplorerPanel;
    OutputLogPanel* m_outputLogPanel;
    DependenciesPanel* m_dependenciesPanel;

    std::vector<BaseModalDialog*> m_modalDialogs;

    std::vector<DocumentPanel*> m_documentPanels;
    DocumentPanel* m_lastActiveDocument;

    DatasheetParser* m_datasheetParser;
};

Editor* GetEditor();

}   //namespace gugu
