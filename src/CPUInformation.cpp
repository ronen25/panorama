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

#include "CPUInformation.h"

panorama::CPUInformation::CPUInformation() {
    getInformation();
}

panorama::CPUInformation::~CPUInformation() { }

void panorama::CPUInformation::sanitizeModelName() {
    const char *PATTERNS[] = { "(R)", "(TM)", nullptr };
    std::string::size_type szPos = 0;

    for (const char **ptr = PATTERNS; *ptr != nullptr; ptr++) {
        auto pos = m_sModelName.find(*ptr, szPos);
        if (pos != std::string::npos) {
            m_sModelName.erase(pos, std::strlen(*ptr));
            szPos = pos;
        }
    }
}

#if defined(__linux__) || defined(LINUX)

// ---------------------------------------------------------------------------------------

void panorama::CPUInformation::getInformation_Linux() {
    std::string sLine;

    // Open /proc/cpuinfo
    std::ifstream fCpuInfo{"/proc/cpuinfo"};
    if (!fCpuInfo.is_open())
        return;

    // Go through every line, splitting the key and the value
    while (std::getline(fCpuInfo, sLine)) {
        std::stringstream ssPartsStream(sLine);
        std::string sKey, sValue;

        std::getline(ssPartsStream, sKey, ':');
        std::getline(ssPartsStream, sValue, ':');

        // Trim key and value
        panorama::utils::trimString(sKey);
        panorama::utils::trimString(sValue);

        // Compare the key to anything we need to get, and store that information.
        if (sKey == PANORAMA_HEADER_VENDOR_ID)
            m_sVendorName = sValue;
        else if (sKey == PANORAMA_HEADER_MODEL_NUMBER)
            m_nModelNumber = std::stoi(sValue);
        else if (sKey == PANORAMA_HEADER_MODEL_NAME) {
            m_sModelName = sValue;

            // Strip any (R) or (TM)
            sanitizeModelName();
        }
        else if (sKey == PANORAMA_HEADER_CPU_MHZ)
            m_nCPUSpeedMHZ = std::stoi(sValue);
        else if (sKey == PANORAMA_HEADER_SIBLINGS)
            m_nThreads = std::stoi(sValue);
        else if (sKey == PANORAMA_HEADER_CACHE_SIZE)
            m_nCacheSize = std::stoi(sValue);
        else if (sKey == PANORAMA_HEADER_CPU_CORES)
            m_nCores = std::stoi(sValue);
        else if (sKey == PANORAMA_HEADER_FLAGS) {
            std::stringstream ssParts(sValue);
            std::string sFeature;

            while (std::getline(ssParts, sFeature))
                m_vFeatures.push_back(sFeature);
        }
    }

    fCpuInfo.close();
}

// ---------------------------------------------------------------------------------------

#endif
