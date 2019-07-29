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

#ifndef PANORAMA_MEMORYUNITS_H
#define PANORAMA_MEMORYUNITS_H

#include <string>

namespace panorama {

    // Units
    enum class MeasurementUnit : uintmax_t {
        // SI
        UNIT_KB = 1000,
        UNIT_MB = 1000000,
        UNIT_GB = 1000000000,
        UNIT_TB = 1000000000000,

        // Binary
        UNIT_KIB = 1024,
        UNIT_MIB = 1048576,
        UNIT_GIB = 1073741824,
        UNIT_TIB = 1099511627776
    };

    // Enums
    enum MeasurementScale {
        MEASUREMENT_SCALE_BINARY,
        MEASUREMENT_SCALE_SI
    };

    class MemoryUnitConverter {
    public:
        static float convertToMeasurement(unsigned long long ullMeasurement, MeasurementUnit eUnit) {
            return ullMeasurement / static_cast<float>(eUnit);
        }

        static std::string unitToString(const MeasurementUnit &eUnit) {
            switch (eUnit) {
                case MeasurementUnit::UNIT_KB:
                    return "KB";
                case MeasurementUnit::UNIT_MB:
                    return "MB";
                case MeasurementUnit::UNIT_GB:
                    return "GB";
                case MeasurementUnit::UNIT_TB:
                    return "TB";
                case MeasurementUnit::UNIT_KIB:
                    return "KiB";
                case MeasurementUnit::UNIT_MIB:
                    return "MiB";
                case MeasurementUnit::UNIT_GIB:
                    return "GiB";
                case MeasurementUnit::UNIT_TIB:
                    return "TiB";
            }

            return "Unknown";
        }
    };
}

#endif //PANORAMA_MEMORYUNITS_H
