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

#ifndef PANORAMA_PLOTCOLORSARRAY_H
#define PANORAMA_PLOTCOLORSARRAY_H

#include <memory>
#include <random>
#include <functional>

#include "imgui.h"

namespace panorama {

    class PlotColorsArray {
    public:
        // Cnstr.
        explicit PlotColorsArray(int nColors);

        // Dstr.
        ~PlotColorsArray();

        // Getters
        inline ImU32 getColor(int nIndex) { return m_upColorsArray[nIndex]; }

    private:
        // Methods
        ImU32 generateRandomColor();

        // Properties
        std::random_device m_randDevice;
        std::mt19937 m_randEngine;
        std::uniform_real_distribution<float> m_dist;
        std::unique_ptr<ImU32[]> m_upColorsArray;
    };

}

#endif //PANORAMA_PLOTCOLORSARRAY_H
