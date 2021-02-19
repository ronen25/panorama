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

#ifndef PANORAMA_MEASUREMENTUNIT_H
#define PANORAMA_MEASUREMENTUNIT_H

#include <string>
#include <cstdint>

namespace panorama {
    enum class MeasurementScale {
        SI,
        BINARY
    };

    // Units
    enum class MeasurementUnit : intmax_t {
        // SI
        KB = 1000,
        MB = 1000000,
        GB = 1000000000,
        TB = 1000000000000,

        // Binary
        KIB = 1024,
        MIB = 1048576,
        GIB = 1073741824,
        TIB = 1099511627776
    };

    class UnitManager {
    public:
        // Cnstr.
        UnitManager();

        // Getters for scale
        void setScale(MeasurementScale measurementScale);
        MeasurementScale currentScale() const;
        bool isScaleSet(MeasurementScale scale) const;

        // Getters for the various units
        MeasurementUnit unitProcessMemory() const;
        MeasurementUnit unitMemory() const;
        MeasurementUnit unitStorage() const;

        // Utility methods
        static float convertTo(std::uintmax_t raw, const MeasurementUnit &unit);
        static std::string unitString(const MeasurementUnit &unit);

    private:
        MeasurementScale m_scale;
        MeasurementUnit m_processMemUnit, m_memUnit, m_storageUnit;
    };
}

#endif //PANORAMA_MEASUREMENTUNIT_H
