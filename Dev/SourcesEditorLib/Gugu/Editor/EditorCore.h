#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Editor/EditorConfig.h"

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
}

namespace sf
{
    class Event;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {
    
class EditorCore : public Singleton<EditorCore>
{
public:

    EditorCore();
    virtual ~EditorCore();

    void Init(const EditorConfig& editorConfig);
    void Release();

    void Update(const gugu::DeltaTime& dt);
    bool OnSFEvent(const sf::Event& event);

    void OpenDocument(const std::string& resourceID);
    void ResetPanels();
    void CloseEditor();

private:

    bool SaveActiveDocument();
    bool SaveAllDocuments();

private:

    EditorConfig m_editorConfig;

    bool m_resetPanels;
    bool m_showSearchResults;
    bool m_showImGuiDemo;

    AssetsExplorerPanel* m_assetsExplorerPanel;
    std::vector<DocumentPanel*> m_documentPanels;

    DocumentPanel* m_lastActiveDocument;
};

EditorCore* GetEditor();

}   //namespace gugu
