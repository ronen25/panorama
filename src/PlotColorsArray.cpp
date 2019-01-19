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

#include "PlotColorsArray.h"

panorama::PlotColorsArray::PlotColorsArray(int nColors)
        : m_upColorsArray{new ImU32[nColors]}, m_randEngine{m_randDevice()}, m_dist{0.3f, 1.0f} {
    // Initialize colors
    for (int i = 0; i < nColors; i++)
        m_upColorsArray[i] = generateRandomColor();
}

panorama::PlotColorsArray::~PlotColorsArray() { }

ImU32 panorama::PlotColorsArray::generateRandomColor() {
    ImVec4 vec4(m_dist(m_randEngine), m_dist(m_randEngine), m_dist(m_randEngine), 1.0f);

    return ImGui::ColorConvertFloat4ToU32(vec4);
}
