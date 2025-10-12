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
private:

    struct LogEntry
    {
        ELog::Type level = ELog::Debug;
        std::string timestamp;
        std::string levelStr;
        std::string categoryStr;
        std::string text;
    };

public:

    OutputLogPanel();
    virtual ~OutputLogPanel();

    virtual void UpdatePanel(const DeltaTime& dt) override;

private:

    void PrintLog(const std::string& timestamp, ELog::Type level, ELogEngine::Type category, const std::string& text);

    void RecomputeLogsVisibility();
    bool IsLogEntryVisible(const LogEntry& entry) const;

private:

    std::vector<LogEntry> m_logs;

    std::vector<size_t> m_visibleLogIndices;
    bool m_showInfos;
    bool m_showWarnings;
    bool m_showErrors;
    bool m_showDebugs;

    bool m_scrollToBottom;
};

}   //namespace gugu
