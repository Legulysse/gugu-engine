#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Editor/Core/EditorConfig.h"

#include "Gugu/Misc/Pattern/Singleton.h"
#include "Gugu/Core/DeltaTime.h"

#include <vector>
#include <string>

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    class DocumentPanel;
    class AssetsExplorerPanel;
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

    void Update(const gugu::DeltaTime& dt);
    bool OnSFEvent(const sf::Event& event);

    void OpenProject(const std::string& assetsPath, const std::string& bindingPath);
    void CloseProject();

    void OpenDocument(const std::string& resourceID);
    void ResetPanels();
    void CloseEditor();

    DatasheetParser* GetDatasheetParser() const;

private:

    bool SaveActiveDocument();
    bool SaveAllDocuments();

private:

    EditorConfig m_editorConfig;
    bool m_isProjectOpen;
    std::string m_projectAssetsPath;
    std::string m_projectBindingPath;

    bool m_resetPanels;
    bool m_showSearchResults;
    bool m_showImGuiDemo;

    AssetsExplorerPanel* m_assetsExplorerPanel;
    std::vector<DocumentPanel*> m_documentPanels;

    DocumentPanel* m_lastActiveDocument;

    DatasheetParser* m_datasheetParser;
};

Editor* GetEditor();

}   //namespace gugu
