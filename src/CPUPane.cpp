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

#include "MainWindow.h"
#include "CPUPane.h"

panorama::CPUPane::CPUPane()
        : m_oCpuInfo{}, m_oCpuUsage{ m_oCpuInfo.threads() }, m_oColorsArray{ m_oCpuInfo.threads() },
          m_fItemSpacing{ImGui::GetStyle().ItemSpacing.y},
          m_eGraphType{GraphType::GRAPH_TYPE_INDIVIDUAL} {
    // For a single CPU we show the combined graph.
    if (m_oCpuInfo.threads() == 1)
        m_eGraphType = GraphType::GRAPH_TYPE_TOTAL;
}

panorama::CPUPane::~CPUPane() { }

void panorama::CPUPane::renderUI() {
    ImGui::BeginChild("##cpuInfoPane");

    ImGui::PushFont(panorama::getFont(PANORAMA_FONT_TITLE));

    ImGui::TextDisabled("CPU: ");

    ImGui::SameLine();

    ImGui::Text("%s", m_oCpuInfo.modelName().c_str());

    ImGui::PopFont();

    ImGui::Separator();

    // CPU details
    {
        ImGui::Text("CPU Vendor:");
        ImGui::SameLine();
        ImGui::TextDisabled("%s", m_oCpuInfo.vendorName().c_str());

        ImGui::SameLine();

        ImGui::Text("Speed:");
        ImGui::SameLine();
        ImGui::TextDisabled("%d MHz", m_oCpuInfo.speedMHZ());

        ImGui::SameLine();

        ImGui::Text("Cores:");
        ImGui::SameLine();
        ImGui::TextDisabled("%d", m_oCpuInfo.cores());

        ImGui::SameLine();

        ImGui::Text("Threads:");
        ImGui::SameLine();
        ImGui::TextDisabled("%d", m_oCpuInfo.threads());
    }

    ImGui::Separator();

    // Graph controls
    {
        ImGui::AlignTextToFramePadding();
        ImGui::Text("Graph Type: ");

        ImGui::SameLine();

        ImGui::PushItemWidth(-1);
        ImGui::Combo("##GraphType", reinterpret_cast<int *>(&m_eGraphType), "Individual\0Total\0");

        ImGui::PopItemWidth();
    }

    ImGui::Separator();

    // CPU Graphs
    {
        if (m_eGraphType == GraphType::GRAPH_TYPE_TOTAL) {
            float fLastSample = m_oCpuUsage.getCoreUsageDataVector(0).back();
            const ImVec2 v2GraphSize = ImVec2(ImGui::GetContentRegionAvailWidth(), ImGui::GetContentRegionAvail().y);

            ImGui::PlotLines("",
                             m_oCpuUsage.getCoreUsageDataArray(0),
                             m_oCpuUsage.getCoreUsageDataArrayLength(0),
                             0,
                             "Total CPU Usage", 0.0f, 100.0f,
                             v2GraphSize);

            // Draw overlay text
            panorama::guiutils::drawBackgroundTextOnGraph(panorama::getFont(PANORAMA_FONT_EXTRALARGE), v2GraphSize,
                                                          m_oCpuUsage.usageToString(fLastSample), 0.4f);
        }
        else {
            // We display a maximum of 4 CPUs in a row, so based on that, calculate every graph's
            // width.
            const int nMinCpusInLine = m_oCpuInfo.threads() > PANORAMA_CPUGRAPH_MAX_PER_ROW ?
                                       PANORAMA_CPUGRAPH_MAX_PER_ROW : m_oCpuInfo.threads();
            const int nCpuGraphRows = m_oCpuInfo.threads() / nMinCpusInLine;
            const float fGraphWidth = ImGui::GetContentRegionAvailWidth() / nMinCpusInLine;
            const float fGraphMaxHeight = std::max(ImGui::GetContentRegionAvail().y / nCpuGraphRows,
                                                   ImGui::GetContentRegionAvail().y * 0.25f) - nCpuGraphRows * m_fItemSpacing;
            const ImVec2 v2GraphSize = ImVec2(fGraphWidth, fGraphMaxHeight);

            for (int i = 1; i <= m_oCpuInfo.threads(); i++) {
                const float fLastSample = m_oCpuUsage.getCoreUsageDataVector(i).back();
                const std::string sCpuStr = "CPU " + std::to_string(i);

                ImGui::PushItemWidth(fGraphWidth);
                ImGui::PushStyleColor(ImGuiCol_PlotLines, m_oColorsArray.getColor(i - 1));
                ImGui::PlotLines("",
                                 m_oCpuUsage.getCoreUsageDataArray(i),
                                 m_oCpuUsage.getCoreUsageDataArrayLength(i),
                                 0, sCpuStr.c_str(), 0.0f, 100.0f,
                                 ImVec2(0, fGraphMaxHeight));
                ImGui::PopStyleColor();

                // Draw overlay text
                panorama::guiutils::drawBackgroundTextOnGraph(panorama::getFont(PANORAMA_FONT_EXTRALARGE), v2GraphSize,
                                                              m_oCpuUsage.usageToString(fLastSample), 0.4f);

                // Should we still render on the same line?
                if (i % nMinCpusInLine != 0)
                    ImGui::SameLine();
            }
        }
    }

    ImGui::EndChild();
}
