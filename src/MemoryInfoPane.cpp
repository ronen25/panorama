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

#include "MemoryInfoPane.h"

panorama::MemoryInfoPane::MemoryInfoPane()
        : m_eUnit{MeasurementUnit::UNIT_GIB}, m_eUnitScale{MeasurementScale::MEASUREMENT_SCALE_BINARY}  { }

panorama::MemoryInfoPane::~MemoryInfoPane() { }

void panorama::MemoryInfoPane::renderUI() {
    ImGui::BeginChild("##memoryinfopane");

    // Header
    ImGui::Text("Measurement Units: ");
    ImGui::SameLine();

    if (ImGui::RadioButton("Binary", (m_eUnitScale == MeasurementScale::MEASUREMENT_SCALE_BINARY))) {
        m_eUnitScale = MeasurementScale::MEASUREMENT_SCALE_BINARY;
        m_eUnit = MeasurementUnit::UNIT_GIB;
    }

    ImGui::SameLine();

    if (ImGui::RadioButton("SI", (m_eUnitScale == MeasurementScale::MEASUREMENT_SCALE_SI))) {
        m_eUnitScale = MeasurementScale::MEASUREMENT_SCALE_SI;
        m_eUnit = MeasurementUnit::UNIT_GB;
    }

    ImGui::Separator();

    // Set the unit string
    const std::string sUnitString = MemoryUnitConverter::unitToString(m_eUnit);

    // RAM details
    ImGui::PushFont(panorama::getFont(PANORAMA_FONT_TITLE));

    ImGui::TextDisabled("Total RAM:");
    ImGui::SameLine();

    ImGui::Text("%.2f %s",
                static_cast<double>(MemoryUnitConverter::convertToMeasurement(m_oMemInfo.data().ulTotalRam, m_eUnit)),
                sUnitString.c_str());

    ImGui::PopFont();

    ImGui::SameLine();

    // Free RAM
    ImGui::TextDisabled("Free: ");
    ImGui::SameLine();
    ImGui::Text("%.2f %s (%.2f%%)",
                static_cast<double>(MemoryUnitConverter::convertToMeasurement(m_oMemInfo.data().ulAvailRam, m_eUnit)),
                sUnitString.c_str(),
                (static_cast<double>(m_oMemInfo.data().ulAvailRam) /
                 static_cast<double>(m_oMemInfo.data().ulTotalRam)) * 100);

    ImGui::Separator();

    // RAM Usage graph
    const ImVec2 v2RamUsageGraphSize = ImVec2(ImGui::GetContentRegionAvailWidth(), ImGui::GetWindowHeight() * 0.4f);
    ImGui::PlotLines("##ramplot", m_oMemInfo.ramUsageVector().data(), PANORAMA_MEMINFO_SAMPLES,
                     0, "RAM Usage (Percents)",
                     0, 100,
                     v2RamUsageGraphSize);
    panorama::guiutils::drawBackgroundTextOnGraph(panorama::getFont(PANORAMA_FONT_EXTRALARGE), v2RamUsageGraphSize,
                                                  memoryUsageToString(m_oMemInfo.ramUsageVector().back()),
                                                  0.4f);

    ImGui::Separator();

    // Swap total
    ImGui::PushFont(panorama::getFont(PANORAMA_FONT_TITLE));

    ImGui::TextDisabled("Total Swap:");
    ImGui::SameLine();

    ImGui::Text("%.2f %s",
                static_cast<double>(MemoryUnitConverter::convertToMeasurement(m_oMemInfo.data().ulTotalSwap, m_eUnit)),
                sUnitString.c_str());

    ImGui::PopFont();

    ImGui::SameLine();

    // Free Swap
    ImGui::TextDisabled("Free: ");
    ImGui::SameLine();
    ImGui::Text("%.2f %s",
                static_cast<double>(MemoryUnitConverter::convertToMeasurement(m_oMemInfo.data().ulFreeSwap, m_eUnit)),
                sUnitString.c_str());

    ImGui::Separator();

    // Swap usage graph
    ImGui::PlotLines("##swapplot", m_oMemInfo.swapUsageVector().data(), PANORAMA_MEMINFO_SAMPLES,
                     0, "Swap Usage (Percents)",
                     0, 100,
                     ImVec2(ImGui::GetContentRegionAvailWidth(), ImGui::GetWindowHeight() * 0.4f));
    panorama::guiutils::drawBackgroundTextOnGraph(panorama::getFont(PANORAMA_FONT_EXTRALARGE), v2RamUsageGraphSize,
                                                  memoryUsageToString(m_oMemInfo.swapUsageVector().back()),
                                                  0.4f);

    ImGui::EndChild();
}
