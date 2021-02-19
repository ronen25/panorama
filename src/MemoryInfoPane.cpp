/*
 *  Panorama -  A simple system monitor for Linux, written using dear ImGui.
 *  Copyright (C) 2018-2021 Ronen Lapushner
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

#include <string>
#include <sstream>
#include <iomanip>

#include "imgui.h"

#include "MemoryInfoPane.h"
#include "MeasurementUnit.h"

using std::string;

using namespace panorama;

static string memoryUsageToString(float fSample) {
    std::stringstream sstr;

    sstr << std::setprecision(3) << fSample << "%";

    return sstr.str();
}

MemoryInfoPane::MemoryInfoPane() { }

void MemoryInfoPane::renderUI(const UnitManager &unitManager) {
    ImGui::BeginChild("##memoryinfopane");

    // Set the unit string
    // TOFIX: Hardcoded to GIB
    const std::string sUnitString = UnitManager::unitString(unitManager.unitMemory());

    // RAM details
    ImGui::PushFont(panorama::getFont(PANORAMA_FONT_TITLE));

    ImGui::TextDisabled("Total RAM:");
    ImGui::SameLine();

    ImGui::Text("%.2f %s",
                static_cast<float>(UnitManager::convertTo(m_memInfo.data().ulTotalRam, unitManager.unitMemory())),
                sUnitString.c_str());

    ImGui::PopFont();

    ImGui::SameLine();

    // Free RAM
    ImGui::TextDisabled("Free: ");
    ImGui::SameLine();
    ImGui::Text("%.2f %s (%.2f%%)",
                static_cast<float>(UnitManager::convertTo(m_memInfo.data().ulAvailRam, unitManager.unitMemory())),
                sUnitString.c_str(),
                (static_cast<float>(m_memInfo.data().ulAvailRam) /
                 static_cast<float>(m_memInfo.data().ulTotalRam)) * 100);

    ImGui::Separator();

    // RAM Usage graph
    const ImVec2 v2RamUsageGraphSize = ImVec2(ImGui::GetContentRegionAvailWidth(), ImGui::GetWindowHeight() * 0.4f);
    ImGui::PlotLines("##ramplot", m_memInfo.ramUsageVector().data(), PANORAMA_MEMINFO_SAMPLES,
                     0, "RAM Usage (Percents)",
                     0, 100,
                     v2RamUsageGraphSize);
    guiutils::drawBackgroundTextOnGraph(panorama::getFont(PANORAMA_FONT_EXTRALARGE), v2RamUsageGraphSize,
                                                  memoryUsageToString(m_memInfo.ramUsageVector().back()),
                                                  0.4f);

    ImGui::Separator();

    // Swap total
    ImGui::PushFont(panorama::getFont(PANORAMA_FONT_TITLE));

    ImGui::TextDisabled("Total Swap:");
    ImGui::SameLine();

    ImGui::Text("%.2f %s",
                static_cast<float>(UnitManager::convertTo(m_memInfo.data().ulTotalSwap, unitManager.unitMemory())),
                sUnitString.c_str());

    ImGui::PopFont();

    ImGui::SameLine();

    // Free Swap
    ImGui::TextDisabled("Free: ");
    ImGui::SameLine();
    ImGui::Text("%.2f %s",
                static_cast<float>(UnitManager::convertTo(m_memInfo.data().ulFreeSwap, unitManager.unitMemory())),
                sUnitString.c_str());

    ImGui::Separator();

    // Swap usage graph
    ImGui::PlotLines("##swapplot", m_memInfo.swapUsageVector().data(), PANORAMA_MEMINFO_SAMPLES,
                     0, "Swap Usage (Percents)",
                     0, 100,
                     ImVec2(ImGui::GetContentRegionAvailWidth(), ImGui::GetWindowHeight() * 0.4f));
    guiutils::drawBackgroundTextOnGraph(panorama::getFont(PANORAMA_FONT_EXTRALARGE), v2RamUsageGraphSize,
                                        memoryUsageToString(m_memInfo.swapUsageVector().back()),
                                        0.4f);

    ImGui::EndChild();
}
