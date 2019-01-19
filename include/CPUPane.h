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

#ifndef PANORAMA_CPUPANE_H
#define PANORAMA_CPUPANE_H

#include <vector>
#include <string>
#include <future>
#include <chrono>

#include "imgui.h"
#include "imgui_internal.h"

#include "FontDefinitions.h"
#include "CPUInformation.h"
#include "CPUUsage.h"
#include "PlotColorsArray.h"

#define PANORAMA_CPUGRAPH_MAX_PER_ROW 4
#define PANORAMA_CPU_USAGE_SAMPLE_RATE 1

namespace panorama {

    // Enums
    enum GraphType : int {
        GRAPH_TYPE_INDIVIDUAL = 0,
        GRAPH_TYPE_TOTAL
    };

    class CPUPane {
    public:
        // Cnstr.
        CPUPane();

        // Dstr.
        ~CPUPane();

        // Renderer
        void renderUI();

        // Getter for the information and the usage
        inline CPUInformation &cpuInformation() { return m_oCpuInfo; }
        inline CPUUsage &cpuUsage() { return m_oCpuUsage; }

    private:
        // Information getters
        CPUInformation m_oCpuInfo;
        CPUUsage m_oCpuUsage;
        const float m_fItemSpacing;
        GraphType m_eGraphType;

        PlotColorsArray m_oColorsArray;
    };

}
#endif //PANORAMA_CPUPANE_H
