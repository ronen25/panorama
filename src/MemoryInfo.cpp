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

#include "MemoryInfo.h"

panorama::MemoryInfo::MemoryInfo() {
    // Reserve space on all vectors
    m_vRamUsage.reserve(PANORAMA_MEMINFO_SAMPLES);
    m_vSwapUsage.reserve(PANORAMA_MEMINFO_SAMPLES);

    for (int i = 0; i < PANORAMA_MEMINFO_SAMPLES; i++) {
        m_vRamUsage.push_back(0);
        m_vSwapUsage.push_back(0);
    }
}

panorama::MemoryInfo::~MemoryInfo() { }

panorama::MemoryInfoSnapshot panorama::MemoryInfo::getMemoryInfoSnapshot_Linux() {
    panorama::MemoryInfoSnapshot oResult = { 0 };

    std::ifstream fProcMeminfo{"/proc/meminfo"};
    if (fProcMeminfo.is_open()) {
        std::string sLine, sValue;
        int nIndex = 0;

        while (std::getline(fProcMeminfo, sLine) && nIndex <= PANORAMA_PROC_MEMINFO_LAST) {
            std::stringstream sstrLine{sLine};

            // Discard the header, since we know all locations.
            sstrLine >> sValue;

            // This value is the actual value we want.
            sstrLine >> sValue;

            // Check what this value is
            switch (nIndex) {
                case PANORAMA_PROC_MEMINFO_MEMTOTAL:
                    oResult.ulTotalRam = std::stoul(sValue) * 1000;
                    break;
                case PANORAMA_PROC_MEMINFO_MEMAVAIL:
                    oResult.ulAvailRam = std::stoul(sValue) * 1000;
                    break;
                case PANORAMA_PROC_MEMINFO_CACHED:
                    oResult.ulCachedRam = std::stoul(sValue) * 1000;
                    break;
                case PANORAMA_PROC_MEMINFO_SWAPTOTAL:
                    oResult.ulTotalSwap = std::stoul(sValue) * 1000;
                    break;
                case PANORAMA_PROC_MEMINFO_SWAPFREE:
                    oResult.ulFreeSwap = std::stoul(sValue) * 1000;
                    break;
            }

            nIndex++;
        }
    }
    else
        throw std::system_error(errno, std::system_category(), "Could not open /proc/meminfo!");

    return oResult;
}

std::future<panorama::MemoryInfoSnapshot> panorama::MemoryInfo::launchProcessListGetterTask() {
    return std::async(std::launch::async, &MemoryInfo::getMemoryInfoSnapshot);
}

bool panorama::MemoryInfo::isTaskReady(std::future<MemoryInfoSnapshot> &futTask) {
    return futTask.wait_for(std::chrono::seconds{0}) == std::future_status::ready;
}

void panorama::MemoryInfo::updateData(std::future<MemoryInfoSnapshot> &futTask) {
    // Update the current snapshot
    m_oMemInfoSnapshot = futTask.get();

    // Remove the first element of every vector and push back the current one
    m_vRamUsage.erase(m_vRamUsage.cbegin());
    m_vRamUsage.push_back(100.0f - (static_cast<float>(m_oMemInfoSnapshot.ulAvailRam)
                                  / m_oMemInfoSnapshot.ulTotalRam) * 100);

    m_vSwapUsage.erase(m_vSwapUsage.cbegin());
    m_vSwapUsage.push_back(100.0f - (static_cast<float>(m_oMemInfoSnapshot.ulFreeSwap)
                                   / m_oMemInfoSnapshot.ulTotalSwap) * 100);
}

