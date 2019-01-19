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

#ifndef PANORAMA_MEMORYINFO_H
#define PANORAMA_MEMORYINFO_H

#include <future>
#include <chrono>
#include <vector>
#include <exception>
#include <algorithm>

#include <iostream>

#if defined(__linux__) || defined(LINUX)

#include <fstream>
#include <string>
#include <sstream>
#include <cerrno>

#define PANORAMA_PROC_MEMINFO_MEMTOTAL 0
#define PANORAMA_PROC_MEMINFO_MEMAVAIL 2
#define PANORAMA_PROC_MEMINFO_CACHED 4
#define PANORAMA_PROC_MEMINFO_SWAPTOTAL 14
#define PANORAMA_PROC_MEMINFO_SWAPFREE 15
#define PANORAMA_PROC_MEMINFO_LAST PANORAMA_PROC_MEMINFO_SWAPFREE

#endif // Includes

#define PANORAMA_MEMINFO_SAMPLES 60

namespace panorama {

    // Note: For technical reasons on Linux, ulAvailRam means actual, total free RAM.
    struct MemoryInfoSnapshot {
        unsigned long ulTotalRam, ulAvailRam, ulCachedRam, ulTotalSwap, ulFreeSwap;
    };

    class MemoryInfo {
    public:
        // Cnstr.
        MemoryInfo();

        // Dstr.
        ~MemoryInfo();

        // Task launchers
        std::future<MemoryInfoSnapshot> launchProcessListGetterTask();
        bool isTaskReady(std::future<MemoryInfoSnapshot> &futTask);
        void updateData(std::future<MemoryInfoSnapshot> &futTask);

        // Getters
        inline const MemoryInfoSnapshot &data() { return m_oMemInfoSnapshot; }
        inline const std::vector<float> &ramUsageVector() { return m_vRamUsage; }
        inline const std::vector<float> &swapUsageVector() { return m_vSwapUsage; }

        // Static getters
#       if defined(__linux__) || defined(LINUX)

        static MemoryInfoSnapshot getMemoryInfoSnapshot_Linux();

#       define getMemoryInfoSnapshot getMemoryInfoSnapshot_Linux

#       endif

    private:
        // Properties
        MemoryInfoSnapshot m_oMemInfoSnapshot;
        std::vector<float> m_vRamUsage, m_vSwapUsage;
    };

}

#endif //PANORAMA_MEMORYINFO_H
