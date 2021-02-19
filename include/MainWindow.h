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

#ifndef PANORAMA_MAINWINDOW_H
#define PANORAMA_MAINWINDOW_H

#include <GLFW/glfw3.h>

#include "config.h"
#include "FontDefinitions.h"

#include "Sidebar.h"
#include "CPUPane.h"
#include "ProcessListPane.h"
#include "MemoryInfoPane.h"
#include "MeasurementUnit.h"

#include "SampleRates.h"

namespace panorama {
    class MainWindow final {
    public:
        // Cnstr.
        MainWindow(GLFWwindow *glfwWindow, const std::string &sTitle, int w, int h);

        void renderUI();

    private:
        using clock_t = std::chrono::steady_clock;

        // Properties
        UnitManager m_unitManager;
        clock_t::time_point m_tLastCpuUsageRefresh, m_tLastProcessListRefresh,
                            m_tLastMemoryInfoRefresh;

        // GUI Panes
        Sidebar m_oSidebar;
        CPUPane m_oCpuPane;
        ProcessListPane m_oProcessListPane;
        MemoryInfoPane m_oMemInfoPane;
    };
}

#endif // PANORAMA_MAINWINDOW_H
