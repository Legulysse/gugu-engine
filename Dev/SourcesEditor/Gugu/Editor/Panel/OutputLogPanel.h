#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Editor/Panel/BasePanel.h"

#include "Gugu/Debug/Logger.h"

#include <string>
#include <vector>

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    class DeltaTime;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

class OutputLogPanel : public BasePanel
{
public:

    OutputLogPanel();
    virtual ~OutputLogPanel();

    virtual void UpdatePanel(const DeltaTime& dt) override;

private:

    void PrintLog(const std::string& timestamp, ELog::Type level, ELogEngine::Type category, const std::string& text);

private:

    struct LogEntry
    {
        ELog::Type level;
        std::string timestamp;
        std::string levelStr;
        std::string categoryStr;
        std::string text;
    };
    std::vector<LogEntry> m_logs;

    bool m_scrollToBottom;
};

}   //namespace gugu
