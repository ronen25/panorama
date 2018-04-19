/*
 *  Panorama -  A simple system monitor for Linux, written using IMGui.
 *  Copyright (C) 2018 Ronen Lapushner
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

using std::string;

panorama::MainWindow::MainWindow(SDL_Window *pSdlWindow,
                                const std::string &sTitle, int w, int h) : Window(pSdlWindow, sTitle, w, h),
        m_eMeasurementUnits{MeasurementUnits::MEASUREMENT_UNITS_BINARY},
        m_oSidebar{w * 0.1f}, m_oCpuPane{ },
        m_oProcessListPane{ }, m_oMemInfoPane{ } { }

panorama::MainWindow::~MainWindow() { }

void panorama::MainWindow::renderUI() {
    // Static updater tasks
    // TOFIX: This whole block is kindda ugly. Must be some way of unifying and templating this entire thing...
    static auto futUsageUpdaterTask = m_oCpuPane.cpuUsage().launchCpuUsageSampleTask();
    static auto futProcessListGetterTask = m_oProcessListPane.processList().launchProcessListGetterTask();
    static auto futMemoryInfoGetterTask = m_oMemInfoPane.memoryInfo().launchProcessListGetterTask();

    // Check if we need to update the data
    {
        auto tDeltaCpuMillis =
                std::chrono::duration_cast<std::chrono::milliseconds>(clock_t::now() - m_tLastCpuUsageRefresh);
        if (tDeltaCpuMillis.count() >= CPU_SAMPLE_RATE.count()) {
            // Check if the cpu usage future has been completed.
            // If it did, update the data and relaunch the task.
            if (m_oCpuPane.cpuUsage().isTaskReady(futUsageUpdaterTask)) {
                m_oCpuPane.cpuUsage().updateData(futUsageUpdaterTask);

                futUsageUpdaterTask = m_oCpuPane.cpuUsage().launchCpuUsageSampleTask();
            }

            // Update last refresh time
            m_tLastCpuUsageRefresh = std::chrono::steady_clock::now();
        }

        auto tDeltaProcessListMillis =
                std::chrono::duration_cast<std::chrono::milliseconds>(clock_t::now() - m_tLastProcessListRefresh);
        if (tDeltaProcessListMillis.count() >= PROCESS_SAMPLE_RATE.count()) {
            // Check if the task has been completed.
            // If it did, update the data and relaunch the task.
            if (m_oProcessListPane.processList().isTaskReady(futProcessListGetterTask)) {
                m_oProcessListPane.processList().updateData(futProcessListGetterTask);

                futProcessListGetterTask = m_oProcessListPane.processList().launchProcessListGetterTask();
            }

            // Update last refresh time
            m_tLastProcessListRefresh = std::chrono::steady_clock::now();
        }

        auto tDeltaMemoryListMillis =
                std::chrono::duration_cast<std::chrono::milliseconds>(clock_t::now() - m_tLastMemoryInfoRefresh);
        if (tDeltaMemoryListMillis.count() >= MEMORY_SAMPLE_RATE.count()) {
            // Check if the task has been completed.
            // If it did, update the data and relaunch the task.
            if (m_oMemInfoPane.memoryInfo().isTaskReady(futMemoryInfoGetterTask)) {
                m_oMemInfoPane.memoryInfo().updateData(futMemoryInfoGetterTask);

                futMemoryInfoGetterTask = m_oMemInfoPane.memoryInfo().launchProcessListGetterTask();
            }

            // Update last refresh time
            m_tLastMemoryInfoRefresh = std::chrono::steady_clock::now();
        }
    }

    // Begin rendering the UI
    ImGui::PushFont(panorama::getFont(PANORAMA_FONT_REGULAR));

    // Main column layout
    ImGui::BeginColumns("columns", 2, ImGuiColumnsFlags_NoResize);

    // Render the sidebar
    ImGui::SetColumnWidth(0, m_oSidebar.width());
    m_oSidebar.renderUI();

    ImGui::NextColumn();

    // Here we actually determine what pane we need to render.
    switch (m_oSidebar.currentlyVisiblePane()) {
        case PaneType::PANETYPE_CPU:
            m_oCpuPane.renderUI();
            break;
        case PaneType::PANETYPE_PROCESSES:
            m_oProcessListPane.renderUI();
            break;
        case PaneType::PANETYPE_MEMORY:
            m_oMemInfoPane.renderUI();
            break;
    }

    ImGui::EndColumns();

    ImGui::PopFont();
}
