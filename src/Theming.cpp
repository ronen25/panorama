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

#include "Theming.h"

using namespace panorama;

// Global variable that controls the theme
Theme g_Theme;

Theme panorama::theme() {
    return g_Theme;
}

void panorama::setTheme(panorama::Theme eNewTheme) {
    g_Theme = eNewTheme;

    ImGuiContext *pContext = ImGui::GetCurrentContext();

    switch (g_Theme) {
    case PANORAMA_THEME_CLASSIC:
        ImGui::StyleColorsClassic(&pContext->Style);
        break;
    case PANORAMA_THEME_DARK:
        ImGui::StyleColorsDark(&pContext->Style);
        break;
    case PANORAMA_THEME_LIGHT:
        ImGui::StyleColorsLight(&pContext->Style);
        break;
    }
}
