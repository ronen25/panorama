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

#ifndef PANORAMA_CPUUSAGE_H
#define PANORAMA_CPUUSAGE_H

#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <exception>
#include <tuple>
#include <iomanip>
#include <future>
#include <chrono>
#include <numeric>
#include <cmath>

#define PANORAMA_SAMPLE_BUFFER_SIZE 90
#define PANORAMA_COUNTER_IDLE_TIME 0
#define PANORAMA_COUNTER_TOTAL_TIME 1

namespace panorama {
    // Usages
    typedef std::tuple<unsigned long, unsigned long> TimesTuple;
    typedef std::tuple<std::vector<float>, std::vector<panorama::TimesTuple>> UsageAndTimesTuple;

    class CPUUsage {
    public:
        // Cnstr.
        CPUUsage(int nCores);

        // Dstr.
        ~CPUUsage();

        // Methods
        std::future<panorama::UsageAndTimesTuple> launchCpuUsageSampleTask();
        bool isTaskReady(std::future<panorama::UsageAndTimesTuple> &futTask);
        void updateData(std::future<panorama::UsageAndTimesTuple> &futTask);

        // Getters
        inline const std::vector<float> &getCoreUsageDataVector(int nCore) { return m_vvfUsageSamplesPerCore[nCore]; }
        inline const float *getCoreUsageDataArray(int nCore) { return m_vvfUsageSamplesPerCore[nCore].data(); }
        inline const int getCoreUsageDataArrayLength(int nCore) { return m_vvfUsageSamplesPerCore[nCore].size(); }

        std::string usageToString(float fUsageSample);

        // Per-platform implementation for the actual fetching of the data
#       if defined(__linux__) || defined(LINUX)
            // Main function
            static UsageAndTimesTuple sampleCpuUsage_Linux(const std::vector<TimesTuple> &vPreviousTimes);

            // Per-core information retriever
            static std::tuple<float, unsigned long, unsigned long>
            sampleCpuCoreUsage_Linux(const std::string &sDataLine,
                                     const std::vector<TimesTuple> &vPreviousTimes);
#           define sampleCpuUsage sampleCpuUsage_Linux
#       endif

    private:
        // Properties
        std::vector<TimesTuple> m_vPreviousTimes;
        std::vector<std::vector<float>> m_vvfUsageSamplesPerCore;
    };

}

#endif // PANORAMA_CPUUSAGE_H
