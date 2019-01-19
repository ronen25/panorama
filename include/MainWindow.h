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

#ifndef PANORAMA_MAINWINDOW_H
#define PANORAMA_MAINWINDOW_H

#include <iostream>
#include <string>
#include <sstream>
#include <chrono>
#include <future>
#include <cstring>
#include <iomanip>
#include <algorithm>

#include "config.h"
#include "Window.h"
#include "FontDefinitions.h"

#include "Sidebar.h"
#include "CPUPane.h"
#include "ProcessListPane.h"
#include "MemoryInfoPane.h"

#include "SampleRates.h"
#include "PlotColorsArray.h"

namespace panorama {
    class MainWindow : public Window {
    public:
        // Cnstr.
        MainWindow(SDL_Window *pSdlWindow, const std::string &sTitle, int w, int h);

        // Dstr.
        ~MainWindow();

    private:

        enum MeasurementUnits {
            MEASUREMENT_UNITS_SI,
            MEASUREMENT_UNITS_BINARY
        };

        // Overridden methods
        void renderUI() override;

        // Properties
        MeasurementUnits m_eMeasurementUnits;
        std::chrono::steady_clock::time_point m_tLastCpuUsageRefresh, m_tLastProcessListRefresh,
                                              m_tLastMemoryInfoRefresh, m_tLastDisksInfoRefresh;

        // GUI Panes
        Sidebar m_oSidebar;
        CPUPane m_oCpuPane;
        ProcessListPane m_oProcessListPane;
        MemoryInfoPane m_oMemInfoPane;

        // Typedefs for std::chrono
        typedef std::chrono::steady_clock clock_t;
        typedef std::chrono::duration<float, std::milli> milli_t;
    };
}

#endif // PANORAMA_MAINWINDOW_H
