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

#include "MeasurementUnit.h"

using std::uintmax_t;
using std::string;

using namespace panorama;

float UnitManager::convertTo(uintmax_t raw, const MeasurementUnit &unit) {
    return raw / static_cast<float>(unit);
}

string UnitManager::unitString(const MeasurementUnit &unit) {
    switch (unit) {
        case MeasurementUnit::KB:
            return "KB";
        case MeasurementUnit::MB:
            return "MB";
        case MeasurementUnit::GB:
            return "GB";
        case MeasurementUnit::TB:
            return "TB";
        case MeasurementUnit::KIB:
            return "KiB";
        case MeasurementUnit::MIB:
            return "MiB";
        case MeasurementUnit::GIB:
            return "GiB";
        case MeasurementUnit::TIB:
            return "TiB";
    }

    return "Unknown";
}

UnitManager::UnitManager() : m_scale{MeasurementScale::BINARY} {
    // Set default units for use in various applications
    m_processMemUnit = MeasurementUnit::MIB;
    m_memUnit = m_storageUnit = MeasurementUnit::GIB;
}

void UnitManager::setScale(MeasurementScale measurementScale) {
    m_scale = measurementScale;

    if (measurementScale == MeasurementScale::BINARY) {
        m_processMemUnit = MeasurementUnit::MIB;
        m_memUnit = m_storageUnit = MeasurementUnit::GIB;
    }
    else if (measurementScale == MeasurementScale::SI) {
        m_processMemUnit = MeasurementUnit::MB;
        m_memUnit = m_storageUnit = MeasurementUnit::GB;
    }
}

MeasurementScale UnitManager::currentScale() const {
    return m_scale;
}

bool UnitManager::isScaleSet(MeasurementScale scale) const {
    return m_scale == scale;
}

MeasurementUnit UnitManager::unitProcessMemory() const {
    return m_processMemUnit;
}

MeasurementUnit UnitManager::unitMemory() const {
    return m_memUnit;
}

MeasurementUnit UnitManager::unitStorage() const {
    return m_storageUnit;
}
