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

#include "SettingsDialog.h"

using namespace panorama;

void SettingsDialog::renderUI() {
    MeasurementUnit m_eUnit;
    MeasurementScale m_eUnitScale;

    // Measurement units
    ImGui::Text("Measurement Units: ");
    ImGui::NewLine();

    if (ImGui::RadioButton("Binary", (m_eUnitScale == MeasurementScale::MEASUREMENT_SCALE_BINARY))) {
        m_eUnitScale = MeasurementScale::MEASUREMENT_SCALE_BINARY;
        m_eUnit = MeasurementUnit::UNIT_MIB;
    }

    ImGui::SameLine();

    if (ImGui::RadioButton("SI", (m_eUnitScale == MeasurementScale::MEASUREMENT_SCALE_SI))) {
        m_eUnitScale = MeasurementScale::MEASUREMENT_SCALE_SI;
        m_eUnit = MeasurementUnit::UNIT_MB;
    }

    ImGui::NewLine();

    ImGui::Separator();

    // Theme settings
    ImGui::Text("Theme: ");
    ImGui::NewLine();

    int current = 0;
    const char *themes = "ImGui Light\0ImGui Dark\0ImGui Classic\0";
    ImGui::PushItemWidth(-1);
    ImGui::AlignTextToFramePadding();
    if (ImGui::Combo("Theme", &current, themes)) {
        switch (current) {
            case 0:
                panorama::setTheme(PANORAMA_THEME_LIGHT);
                break;
            case 1:
                panorama::setTheme(PANORAMA_THEME_DARK);
                break;
            case 2:
                panorama::setTheme(PANORAMA_THEME_CLASSIC);
                break;
        }
    }

    ImGui::Separator();

    ImGui::Button("OK");
}