/*
 *  Panorama -  A simple system monitor for Linux, written using dear ImGui.
 *  Copyright (C) 2018-2022 Ronen Lapushner
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

#include "Sidebar.h"

using namespace panorama;

Sidebar::Sidebar()
        : m_eCurrentlyVisiblePane{PaneType::PANETYPE_CPU} { }
Sidebar::~Sidebar() { }

void Sidebar::renderUI() {
    static bool bIsAboutOpen = false;

    auto v2ButtonSize = ImVec2(0, 70 * guiutils::getScalingFactor());

    // CPU Pane button
    if (ImGui::Selectable(ICON_FA_MICROCHIP " CPU",
                          (m_eCurrentlyVisiblePane == PaneType::PANETYPE_CPU), 0, v2ButtonSize))
        m_eCurrentlyVisiblePane = PaneType::PANETYPE_CPU;

    // Processes Pane button
    if (ImGui::Selectable(ICON_FA_WINDOW_RESTORE " PROCESSES",
                          (m_eCurrentlyVisiblePane == PaneType::PANETYPE_PROCESSES), 0, v2ButtonSize))
        m_eCurrentlyVisiblePane = PaneType::PANETYPE_PROCESSES;

    // Resources Pane button
    if (ImGui::Selectable(ICON_FA_MEMORY " MEMORY",
                          (m_eCurrentlyVisiblePane == PaneType::PANETYPE_MEMORY), 0, v2ButtonSize))
        m_eCurrentlyVisiblePane = PaneType::PANETYPE_MEMORY;

    // Separator
    //const float fAvailSpace = ImGui::GetContentRegionAvail().y - ITEMS_AT_BOTTOM * ImGui::GetTextLineHeight();
    //ImGui::InvisibleButton("##siderbar_sep", ImVec2(-1, fAvailSpace));

    // Spacing
    ImGui::Spacing();

    // About
    if (ImGui::Selectable("About...")) {
        bIsAboutOpen = true;

        ImGui::SetNextWindowSize(ImVec2(-1, 270 * guiutils::getScalingFactor()), 0);
        ImGui::OpenPopup("About Panorama...");
    }

    // Exit
    if (ImGui::Selectable("Exit")) {
        SDL_Event event;
        event.type = SDL_QUIT;

        SDL_PushEvent(&event);
    }

    if (ImGui::BeginPopupModal("About Panorama...", &bIsAboutOpen, ImGuiWindowFlags_NoResize)) {
        AboutDialog::renderUI();
        ImGui::EndPopup();
    }

    //ImGui::EndChild();
}
