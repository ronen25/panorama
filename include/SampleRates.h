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

#ifndef PANORAMA_SAMPLERATES_H
#define PANORAMA_SAMPLERATES_H

#include <chrono>

#define PANORAMA_CPU_DATA_SAMPLE_PER_SEC 1
#define PANORAMA_PROCESS_SAMPLE_PER_SEC  1
#define PANORAMA_MEMORY_SAMPLE_PER_SEC 1
#define PANORAMA_DISKSINFO_SAMPLE_PER_SEC 1

namespace panorama {
    // Sample rate definitions
    constexpr auto CPU_SAMPLE_RATE = std::chrono::duration_cast<std::chrono::seconds>(
            std::chrono::duration<float>(1000.0f / PANORAMA_CPU_DATA_SAMPLE_PER_SEC));
    constexpr auto PROCESS_SAMPLE_RATE = std::chrono::duration_cast<std::chrono::seconds>(
            std::chrono::duration<float>(1000.0f / PANORAMA_PROCESS_SAMPLE_PER_SEC));
    constexpr auto MEMORY_SAMPLE_RATE = std::chrono::duration_cast<std::chrono::seconds>(
            std::chrono::duration<float>(1000.0f / PANORAMA_MEMORY_SAMPLE_PER_SEC));
}

#endif //PANORAMA_SAMPLERATES_H
