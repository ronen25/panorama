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

#ifndef PANORAMA_MEMORYINFOPANE_H
#define PANORAMA_MEMORYINFOPANE_H

#include "MemoryInfo.h"
#include "MeasurementUnit.h"

#include "FontDefinitions.h"
#include "Utils.h"

namespace panorama {

    class MemoryInfoPane {
    public:
        // Cnstr.
        MemoryInfoPane();

        // Methods
        void renderUI(const UnitManager &unitManager);

        // Getters
        inline MemoryInfo &memoryInfo() { return m_memInfo; }

    private:
        // Properties
        MemoryInfo m_memInfo;
        MeasurementScale m_unitScale;
    };

}

#endif //PANORAMA_MEMORYINFOPANE_H
