////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Editor/Panel/OutputLogPanel.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/External/ImGuiUtility.h"

#include <map>

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

OutputLogPanel::OutputLogPanel()
    : m_showInfos(true)
    , m_showWarnings(true)
    , m_showErrors(true)
    , m_showDebugs(false)
    , m_scrollToBottom(true)
{
    m_title = "Output Log";

    GetLogEngine()->RegisterDelegate(Handle(this), STD_BIND_4(&OutputLogPanel::PrintLog, this));
}

OutputLogPanel::~OutputLogPanel()
{
    GetLogEngine()->UnregisterDelegate(Handle(this));
}

void OutputLogPanel::PrintLog(const std::string& timestamp, ELog::Type level, ELogEngine::Type category, const std::string& text)
{
    static const std::map<ELog::Type, std::string> logLevelAsStr
    {
        { ELog::Debug, "Debug" },
        { ELog::Info, "Info" },
        { ELog::Warning, "Warning" },
        { ELog::Error, "Error" },
    };

    static const std::map<ELogEngine::Type, std::string> logCategoryAsStr
    {
        { ELogEngine::Engine, "Engine" },
        { ELogEngine::Editor, "Editor" },
        { ELogEngine::Resources, "Resources" },
        { ELogEngine::Audio, "Audio" },
        { ELogEngine::Network, "Network" },
        { ELogEngine::Element, "Element" },
        { ELogEngine::Databinding, "Databinding" },
    };

    LogEntry entry;
    entry.level = level;
    entry.timestamp = timestamp;
    entry.levelStr = logLevelAsStr.at(level);
    entry.categoryStr = logCategoryAsStr.at(category);
    entry.text = text;

    m_logs.push_back(entry);
    size_t newLogEntryIndex = m_logs.size() - 1;

    if (IsLogEntryVisible(entry))
    {
        m_visibleLogIndices.push_back(newLogEntryIndex);
    }

    m_scrollToBottom = true;
}

void OutputLogPanel::RecomputeLogsVisibility()
{
    m_visibleLogIndices.clear();
    m_visibleLogIndices.reserve(m_logs.size());

    for (size_t i = 0; i < m_logs.size(); ++i)
    {
        if (IsLogEntryVisible(m_logs[i]))
        {
            m_visibleLogIndices.push_back(i);
        }
    }
}

bool OutputLogPanel::IsLogEntryVisible(const LogEntry& entry) const
{
    return (entry.level == ELog::Info && m_showInfos
        || entry.level == ELog::Warning && m_showWarnings
        || entry.level == ELog::Error && m_showErrors
        || entry.level == ELog::Debug && m_showDebugs);
}

void OutputLogPanel::UpdatePanel(const DeltaTime& dt)
{
    if (ImGui::Begin(m_title.c_str(), nullptr))
    {
        static const ImVec4 color_red(1.f, 0.f, 0.f, 1.f);
        static const ImVec4 color_yellow(1.f, 1.f, 0.f, 1.f);
        static const ImVec4 color_white(1.f, 1.f, 1.f, 1.f);
        static const ImVec4 color_grey(1.f, 1.f, 1.f, 0.6f);
        static const ImVec4 color_greyDetails(1.f, 1.f, 1.f, 0.4f);

        static const std::map<ELog::Type, ImVec4> logColorPerLevel
        {
            { ELog::Debug, color_grey },
            { ELog::Info, color_white },
            { ELog::Warning, color_yellow },
            { ELog::Error, color_red },
        };

        // Control buttons.
        {
            if (ImGui::Checkbox("Infos", &m_showInfos))
            {
                RecomputeLogsVisibility();
            }

            ImGui::SameLine();
            if (ImGui::Checkbox("Warnings", &m_showWarnings))
            {
                RecomputeLogsVisibility();
            }

            ImGui::SameLine();
            if (ImGui::Checkbox("Errors", &m_showErrors))
            {
                RecomputeLogsVisibility();
            }

            ImGui::SameLine();
            if (ImGui::Checkbox("Debug", &m_showDebugs))
            {
                RecomputeLogsVisibility();
            }

            ImGui::SameLine(0.f, 40.f);
            if (ImGui::Button("Clear logs"))
            {
                m_logs.clear();
                m_visibleLogIndices.clear();
            }
        }

        ImGuiTableFlags flags = ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable | ImGuiTableFlags_ScrollY | ImGuiTableFlags_ScrollX /* | ImGuiTableFlags_NoPadInnerX */;
        if (ImGui::BeginTable("_LOGS_TABLE", 4, flags))
        {
            ImGuiTableColumnFlags columnFlags = ImGuiTableColumnFlags_WidthFixed;
            ImGui::TableSetupColumn("time", columnFlags, 140.f);
            ImGui::TableSetupColumn("category", columnFlags, 70.f);
            ImGui::TableSetupColumn("level", columnFlags, 45.f);
            ImGui::TableSetupColumn("log", columnFlags, 0.f);
            ImGui::TableSetupScrollFreeze(0, 1);
            ImGui::TableHeadersRow();

            ImGuiListClipper clipper;
            clipper.Begin((int)m_visibleLogIndices.size());
            while (clipper.Step())
            {
                for (int rowIndex = clipper.DisplayStart; rowIndex < clipper.DisplayEnd; ++rowIndex)
                {
                    size_t logIndex = m_visibleLogIndices[rowIndex];

                    ImGui::PushID((int)logIndex);

                    float row_min_height = 0.f;
                    ImGui::TableNextRow(ImGuiTableRowFlags_None, row_min_height);

                    ELog::Type logLevel = m_logs[logIndex].level;
                    ImVec4 color = logColorPerLevel.at(logLevel);

                    int columnIndex = 0;
                    ImGui::TableSetColumnIndex(columnIndex++);
                    ImGui::TextColored(color_greyDetails, m_logs[logIndex].timestamp.c_str());

                    ImGui::TableSetColumnIndex(columnIndex++);
                    ImGui::TextColored(color_greyDetails, m_logs[logIndex].categoryStr.c_str());

                    ImGui::TableSetColumnIndex(columnIndex++);
                    ImGui::TextColored(color, m_logs[logIndex].levelStr.c_str());

                    ImGui::TableSetColumnIndex(columnIndex++);
                    ImGui::TextColored(color, m_logs[logIndex].text.c_str());

                    ImGui::PopID();
                }
            }

            if (m_scrollToBottom)
            {
                ImGui::SetScrollHereY(1.0f);
                m_scrollToBottom = false;
            }

            ImGui::EndTable();
        }
    }
    ImGui::End();
}

}   //namespace gugu
