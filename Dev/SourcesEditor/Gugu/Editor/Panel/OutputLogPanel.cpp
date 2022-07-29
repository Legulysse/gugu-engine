////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Editor/Panel/OutputLogPanel.h"

////////////////////////////////////////////////////////////////
// Includes

#include <imgui.h>
#include <imgui_stdlib.h>

#include <map>

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

OutputLogPanel::OutputLogPanel()
    : m_scrollToBottom(true)
{
    m_title = "Output Log";

    GetLogEngine()->RegisterDelegate(this, std::bind(
        &OutputLogPanel::PrintLog,
        this,
        std::placeholders::_1,
        std::placeholders::_2,
        std::placeholders::_3,
        std::placeholders::_4));
}

OutputLogPanel::~OutputLogPanel()
{
    GetLogEngine()->UnregisterDelegate(this);
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
        { ELogEngine::Resources, "Resources" },
        { ELogEngine::Audio, "Audio" },
        { ELogEngine::Network, "Network" },
        { ELogEngine::Element, "Element" },
    };

    LogEntry entry;
    entry.level = level;
    entry.timestamp = timestamp;
    entry.levelStr = logLevelAsStr.at(level);
    entry.categoryStr = logCategoryAsStr.at(category);
    entry.text = text;

    m_logs.push_back(entry);
    m_scrollToBottom = true;
}

void OutputLogPanel::UpdatePanel(const DeltaTime& dt)
{
    if (ImGui::Begin(m_title.c_str(), false))
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

        ImGuiTableFlags flags = ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable | ImGuiTableFlags_ScrollY | ImGuiTableFlags_ScrollX /* | ImGuiTableFlags_NoPadInnerX */;
        if (ImGui::BeginTable("Logs", 4, flags))
        {
            ImGuiTableColumnFlags columnFlags = ImGuiTableColumnFlags_WidthFixed;
            ImGui::TableSetupColumn("time", columnFlags, 140.f);
            ImGui::TableSetupColumn("category", columnFlags, 70.f);
            ImGui::TableSetupColumn("level", columnFlags, 45.f);
            ImGui::TableSetupColumn("log", columnFlags, 0.f);
            ImGui::TableSetupScrollFreeze(0, 1);
            ImGui::TableHeadersRow();

            for (size_t rowIndex = 0; rowIndex < m_logs.size(); ++rowIndex)
            {
                ImGui::PushID(rowIndex);

                float row_min_height = 0.f;
                ImGui::TableNextRow(ImGuiTableRowFlags_None, row_min_height);

                if (rowIndex == 0)
                {
                    // Setup ItemWidth once.
                    int headerIndex = 0;

                    ImGui::TableSetColumnIndex(headerIndex++);
                    ImGui::PushItemWidth(-1);
                    ImGui::TableSetColumnIndex(headerIndex++);
                    ImGui::PushItemWidth(-1);
                    ImGui::TableSetColumnIndex(headerIndex++);
                    ImGui::PushItemWidth(-1);
                    ImGui::TableSetColumnIndex(headerIndex++);
                    ImGui::PushItemWidth(-1);
                }

                ELog::Type logLevel = m_logs[rowIndex].level;
                ImVec4 color = logColorPerLevel.at(logLevel);

                int columnIndex = 0;
                ImGui::TableSetColumnIndex(columnIndex++);
                ImGui::TextColored(color_greyDetails, m_logs[rowIndex].timestamp.c_str());

                ImGui::TableSetColumnIndex(columnIndex++);
                ImGui::TextColored(color_greyDetails, m_logs[rowIndex].categoryStr.c_str());

                ImGui::TableSetColumnIndex(columnIndex++);
                ImGui::TextColored(color, m_logs[rowIndex].levelStr.c_str());

                ImGui::TableSetColumnIndex(columnIndex++);
                ImGui::TextColored(color, m_logs[rowIndex].text.c_str());

                ImGui::PopID();
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
