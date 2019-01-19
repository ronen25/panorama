/*
 *  Panorama -  A simple system monitor for Linux, written using dear ImGui.
 *  Copyright (C) 2018-2019 Ronen Lapushner
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#include "ProcessListPane.h"

panorama::ProcessListPane::ProcessListPane() : m_eUnitScale{MeasurementScale::MEASUREMENT_SCALE_BINARY},
                                               m_eUnit{MeasurementUnit::UNIT_MIB},
                                               m_nCurrentlySelectedProcess{-1},
                                               m_nCurrentlyVisibleProcesses{ 0 } {
    m_arrFilterBuffer.fill(0);
}

panorama::ProcessListPane::~ProcessListPane() { }

void panorama::ProcessListPane::renderUI() {
    static bool bShowPriorityPopup = false;
    ImGui::BeginChild("##processlistpane");

    // Top header
    // Memory measurement units
    ImGui::Text("Measurement Units: ");
    ImGui::SameLine();

    if (ImGui::RadioButton("Binary", (m_eUnitScale == MeasurementScale::MEASUREMENT_SCALE_BINARY))) {
        m_eUnitScale = MeasurementScale::MEASUREMENT_SCALE_BINARY;
        m_eUnit = MeasurementUnit::UNIT_MIB;
    }

    ImGui::SameLine();

    if (ImGui::RadioButton("SI", (m_eUnitScale == MeasurementScale::MEASUREMENT_SCALE_SI))) {
        m_eUnitScale = MeasurementScale::MEASUREMENT_SCALE_SI;
        m_eUnit = MeasurementUnit::UNIT_MB;
    }

    // Separator
    ImGui::SameLine();
    ImGui::TextDisabled("|");
    ImGui::SameLine();

    // Filter
    ImGui::Text("Filter: ");
    ImGui::SameLine();

    // Kinduva cheap hack to make sure we can safely draw the two buttons.
    // IMGUI currently lacks any functionality that allows doing this in a proper way.
    m_stTextFilter.Draw("##Filter", ImGui::GetContentRegionAvailWidth() - 2 * ImGui::CalcTextSize("LONGENOUGHTEXT").x);

    // Prioity popup (if anything's selected)
    if (m_nCurrentlySelectedProcess != -1) {
        ImGui::SameLine();

        if (ImGui::Button("Priority...")) {
            if (ProcessUtils::canChangePriority_Linux())
                ImGui::OpenPopup("priopopup");
        }

        if (!ProcessUtils::canChangePriority_Linux() && ImGui::IsItemHovered()) {
            ImGui::BeginTooltip();

            ImGui::Text("You need to run Panorama in privileged mode to set priorities for processes!");

            ImGui::EndTooltip();
        }


        ImGui::SameLine();

        // Kill button
        ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4) ImColor::HSV(7.0f, 0.6f, 0.6f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4) ImColor::HSV(7.0f, 0.7f, 0.7f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4) ImColor::HSV(7.0f, 0.8f, 0.8f));

        if (ImGui::Button("Terminate"))
            ProcessUtils::terminateProcess(m_nCurrentlySelectedProcess);

        ImGui::SameLine();

        if (ImGui::Button("Kill"))
            ProcessUtils::killProcess(m_nCurrentlySelectedProcess);

        ImGui::PopStyleColor(3);
    }

    // Render columns
    ImGui::Separator();

    ImGui::BeginChild("##clmnprocesses_childframe",
                      ImVec2(0, ImGui::GetContentRegionAvail().y - ImGui::GetItemsLineHeightWithSpacing()));

    ImGui::Columns(6, "##clmnsprocesses", true);
    ImGui::Separator();

    // Render every column
    ImGui::Text("PID"); ImGui::NextColumn();
    ImGui::Text("Name"); ImGui::NextColumn();
    ImGui::Text("CPU Usage"); ImGui::NextColumn();
    ImGui::Text("Total Memory"); ImGui::NextColumn();
    ImGui::Text("Virtual Memory"); ImGui::NextColumn();
    ImGui::Text("Priority"); ImGui::NextColumn();

    ImGui::Separator();

    // Render every item
    m_nCurrentlyVisibleProcesses = 0;
    for (auto &procInfo: m_oProcessList.data()) {
        // Check if this entry passes the filter.
        if (!m_stTextFilter.PassFilter(procInfo.m_sName.c_str())) {
            // Now check if this has been selected.
            // If it did, it does not show up with the new filter, so clear the selection
            if (m_nCurrentlySelectedProcess == procInfo.m_nPid) {
                m_nCurrentlySelectedProcess = -1;
                m_nCurrentlySelectedProcessPrio = -1;
            }

            continue;
        }

        // PID
        if (ImGui::Selectable(std::to_string(procInfo.m_nPid).c_str(),
                                        (m_nCurrentlySelectedProcess == procInfo.m_nPid),
                                         ImGuiSelectableFlags_SpanAllColumns)) {
            m_nCurrentlySelectedProcess = procInfo.m_nPid;
            m_nCurrentlySelectedProcessPrio = procInfo.m_nPriority;
        }
        ImGui::NextColumn();

        // Set the unit string
        const std::string sUnitString = MemoryUnitConverter::unitToString(m_eUnit);

        // Name
        ImGui::Text("%s", procInfo.m_sName.c_str());
        ImGui::NextColumn();

        // CPU Usage
        ImGui::Text("%.2f%%", procInfo.m_fCpuUsagePercent);
        ImGui::NextColumn();

        // Total Memory
        ImGui::Text("%.2f %s",
                    (procInfo.m_ullResidentMemSize - procInfo.m_ullSharedMemSize) / static_cast<float>(m_eUnit),
                    sUnitString.c_str());
        ImGui::NextColumn();

        // Virtual Memory
        ImGui::Text("%.2f %s",
                    procInfo.m_ullVirtMemSize / static_cast<float>(m_eUnit),
                    sUnitString.c_str());
        ImGui::NextColumn();

        // Priority
        ImGui::Text("%d", procInfo.m_nPriority);
        ImGui::NextColumn();

        // Add shown processes
        m_nCurrentlyVisibleProcesses++;
    }

    ImGui::EndChild();

    // Status bar
    ImGui::Separator();

    if (m_stTextFilter.IsActive())
        ImGui::TextDisabled("Showing %d/%lu processes.", m_nCurrentlyVisibleProcesses, m_oProcessList.data().size());
    else
        ImGui::TextDisabled("Total %lu processes.", m_oProcessList.data().size());

    // Priority popup
    if (ImGui::BeginPopup("priopopup", ImGuiWindowFlags_NoMove)) {
        ImGui::Text("High");
        ImGui::SameLine();

#if     defined(LINUX) || defined(__linux__)

        ImGui::SliderInt("##range", &m_nCurrentlySelectedProcessPrio, -17, 17);
        ImGui::SameLine();

#endif

        ImGui::Text("Low");
        ImGui::SameLine();

        if (ImGui::Button("OK")) {
            ProcessUtils::changeProcessPriority_Linux(m_nCurrentlySelectedProcess, m_nCurrentlySelectedProcessPrio);
            ImGui::CloseCurrentPopup();
        }

        ImGui::SameLine();

        if (ImGui::Button("Cancel"))
            ImGui::CloseCurrentPopup();

        ImGui::EndPopup();
    }

    ImGui::EndChild();
}
