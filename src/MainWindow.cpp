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

#include "MainWindow.h"

using std::string;

using namespace panorama;

panorama::MainWindow::MainWindow(GLFWwindow *glfwWindow,
                                const std::string &sTitle, int w, int h) :
        m_eMeasurementUnits{MeasurementUnits::MEASUREMENT_UNITS_BINARY},
        m_oSidebar{w * 0.1f}, m_oCpuPane{ },
        m_oProcessListPane{ }, m_oMemInfoPane{ } { }

panorama::MainWindow::~MainWindow() { }

void panorama::MainWindow::renderUI() {
    static bool bAboutDialog = false;

    // Set window to maximum size
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);

    // Begin window
    ImGui::Begin("#mainwindow", nullptr,
                 ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                 ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_AlwaysUseWindowPadding);

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

    // Main menubar
    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Exit", "Alt+F4", false, true)) {
                glfwSetWindowShouldClose(g_glfwWindow, 1);
            }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("View")) {
            if (ImGui::BeginMenu("Theme")) {
                if (ImGui::MenuItem("Light (Default)", nullptr,
                                    ThemeManager::isThemeSet(Theme::PANORAMA_THEME_LIGHT))) {
                    ThemeManager::setTheme(Theme::PANORAMA_THEME_LIGHT);
                }

                if (ImGui::MenuItem("Dark", nullptr,
                                    ThemeManager::isThemeSet(Theme::PANORAMA_THEME_DARK))) {
                    ThemeManager::setTheme(Theme::PANORAMA_THEME_DARK);
                }

                if (ImGui::MenuItem("imgui Classic", nullptr,
                                    ThemeManager::isThemeSet(Theme::PANORAMA_THEME_CLASSIC))) {
                    ThemeManager::setTheme(Theme::PANORAMA_THEME_CLASSIC);
                }

                ImGui::EndMenu();
            }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Help")) {
            if (ImGui::MenuItem("About...", "F1", false, true)) {
                bAboutDialog = true;
            }

            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }

    if (bAboutDialog)
        ImGui::OpenPopup("About Panorama");

    if (ImGui::BeginPopupModal("About Panorama", &bAboutDialog)) {
        AboutDialog::renderUI();
        ImGui::EndPopup();
    }

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

    ImGui::End();
}
