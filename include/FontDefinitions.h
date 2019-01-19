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

#ifndef PANORAMA_FONTDEFINITIONS_H
#define PANORAMA_FONTDEFINITIONS_H

// FONT SIZES
#define PANORAMA_FONT_SIZE_REGULAR 16.0f
#define PANORAMA_FONT_SIZE_TITLE 40.0f
#define PANORAMA_FONT_SIZE_EXTRALARGE 60.0f

// FONT DEFINITIONS
#define PANORAMA_FONT_REGULAR 1
#define PANORAMA_FONT_TITLE 2
#define PANORAMA_FONT_EXTRALARGE 3

#include "imgui.h"
#include "IconsFontAwesome5.h"

namespace panorama {
    inline ImFont *getFont(int index) {
        ImGuiIO &io = ImGui::GetIO();

        return io.Fonts->Fonts[index];
    }
}

#endif //PANORAMA_FONTDEFINITIONS_H
