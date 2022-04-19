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
    class DeltaTime;
    class BaseModalDialog;
    class DocumentPanel;
    class AssetsExplorerPanel;
    class ImageSetPanel;
    class DatasheetParser;
    class ProjectSettings;
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

    void Update(const gugu::DeltaTime& dt);
    bool OnSFEvent(const sf::Event& event);

    void OpenProject(const std::string& projectPathFile);
    bool CloseProject();
    bool IsProjectOpen() const;

    bool OpenModalDialog(BaseModalDialog* modalDialog);
    void OpenDocument(const std::string& resourceID);

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

    void CloseProjectImpl();
    void CloseEditorImpl();

private:

    EditorConfig m_editorConfig;

    ProjectSettings* m_project;

    bool m_checkDirtyDocuments;
    bool m_pendingCloseEditor;
    bool m_pendingCloseProject;
    bool m_pendingCloseDocument;

    bool m_resetPanels;
    bool m_showSearchResults;
    bool m_showImGuiDemo;

    std::vector<BaseModalDialog*> m_modalDialogs;

    AssetsExplorerPanel* m_assetsExplorerPanel;
    std::vector<DocumentPanel*> m_documentPanels;

    DocumentPanel* m_lastActiveDocument;

    DatasheetParser* m_datasheetParser;
};

Editor* GetEditor();

}   //namespace gugu
