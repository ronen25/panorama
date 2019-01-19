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

#include "CPUUsage.h"

panorama::CPUUsage::CPUUsage(int nCores) {
    // Initialize previous times
    // Since "core 0" is the total usage, we loop until nCores.
    for (int i = 0; i <= nCores; i++) {
        m_vPreviousTimes.emplace_back(0.0f, 0.0f);

        std::vector<float> vfResultSet;
        vfResultSet.reserve(PANORAMA_SAMPLE_BUFFER_SIZE);
        for (int i = 0; i < PANORAMA_SAMPLE_BUFFER_SIZE; i++)
            vfResultSet.push_back(0.0f);

        m_vvfUsageSamplesPerCore.push_back(std::move(vfResultSet));
    }
}

panorama::CPUUsage::~CPUUsage() { }

std::future<panorama::UsageAndTimesTuple> panorama::CPUUsage::launchCpuUsageSampleTask() {
    return std::async(std::launch::async, CPUUsage::sampleCpuUsage, std::cref(m_vPreviousTimes));
}

bool panorama::CPUUsage::isTaskReady(std::future<panorama::UsageAndTimesTuple> &futTask) {
    return futTask.wait_for(std::chrono::seconds{0}) == std::future_status::ready;
}

void panorama::CPUUsage::updateData(std::future<panorama::UsageAndTimesTuple> &futTask) {
    // Assume here that the task is indeed done
    panorama::UsageAndTimesTuple tupResult = futTask.get();

    // Update previous times by simply swapping out the data
    m_vPreviousTimes = std::get<1>(tupResult);

    // Check if we've reached the sample limit.
    // If we did, remove the first sample.
    // Regardless, insert the samples into each vector.
    std::vector<float> vfResults = std::move(std::get<0>(tupResult));
    for (std::vector<float>::size_type i = 0; i < vfResults.size(); i++) {
        m_vvfUsageSamplesPerCore[i].erase(m_vvfUsageSamplesPerCore[i].begin());
        m_vvfUsageSamplesPerCore[i].push_back(vfResults[i]);
    }
}

std::string panorama::CPUUsage::usageToString(float fUsageSample) {
    std::stringstream sstr;

    sstr << std::setprecision(3) << fUsageSample << "%";

    return sstr.str();
}

// ---------------------------------------------------
// PER-PLATFORM IMPLEMENTATION
// ---------------------------------------------------
#if defined(__linux__) || defined(LINUX)

std::tuple<float, unsigned long, unsigned long>
panorama::CPUUsage::sampleCpuCoreUsage_Linux(const std::string &sDataLine,
                                            const std::vector<TimesTuple> &vPreviousTimes) {
    std::stringstream sstrLine{sDataLine};
    std::string sValue;
    std::vector<unsigned long> vulTimes;
    unsigned long ulIdleTime, ulTotalTime, ulIdleTimeDelta, ulTotalTimeDelta;
    float fTotalPercent = 0.0f;
    int nCore = 0;

    // Read the CPU identifier and extract the number.
    // No number = core 0 = Total usage
    sstrLine >> sValue;
    if (sValue.length() < 3)
        throw std::invalid_argument("Unexpected CPU identifier; length too short.");

    // Isolate the numeric component, if any.
    std::string sCoreNum = sValue.substr(3);
    if (sCoreNum.empty())
        nCore = 0; // Total usage
    else {
        // Try to convert the identifier.
        try {
            nCore = std::stoi(sCoreNum) + 1;
        }
        catch (const std::invalid_argument &) {
            throw std::invalid_argument("Found CPU identifier, but it\'s not numeric: " + sCoreNum);
        }
    }

    // Extract all the times
    while (sstrLine >> sValue)
        vulTimes.push_back(std::stoul(sValue));

    // Calculate times.
    ulIdleTime = vulTimes[3];
    ulTotalTime = std::accumulate(vulTimes.begin(), vulTimes.end(), 0ul);

    // If it's the first sample, we just store the times, and disregard this sample.
    unsigned long ulPreviousIdleTime = std::get<PANORAMA_COUNTER_IDLE_TIME>(vPreviousTimes[nCore]);
    unsigned long ulPreviousTotalTime = std::get<PANORAMA_COUNTER_TOTAL_TIME>(vPreviousTimes[nCore]);
    if (ulPreviousIdleTime != 0 || ulPreviousTotalTime != 0) {
        ulIdleTimeDelta = ulIdleTime - ulPreviousIdleTime;
        ulTotalTimeDelta = ulTotalTime - ulPreviousTotalTime;

        fTotalPercent = 100.0f * (1.0f - static_cast<float>(ulIdleTimeDelta) /
                                         static_cast<float>(ulTotalTimeDelta));
    }

    return std::make_tuple(fTotalPercent, ulIdleTime, ulTotalTime);
}

panorama::UsageAndTimesTuple panorama::CPUUsage::sampleCpuUsage_Linux(const std::vector<TimesTuple> &vPreviousTimes) {
    std::vector<float> vPercentages;
    std::vector<TimesTuple> vNewPreviousTimes;
    int nIndex = 0;

    // Open /proc/stat
    std::ifstream fProcStatFile{std::string("/proc/stat")};
    if (fProcStatFile.is_open()) {
        std::string sLine;

        // Process the rest of the cores
        while (std::getline(fProcStatFile, sLine)) {
            if (sLine.find("cpu") != std::string::npos) {
                auto tupValues = sampleCpuCoreUsage_Linux(sLine, vPreviousTimes);

                // Update the result and the previous times for every core
                vPercentages.push_back(std::get<0>(tupValues));

                unsigned long ulNewIdleTime = std::get<1>(tupValues),
                              ulNewTotalTime = std::get<2>(tupValues);
                vNewPreviousTimes.emplace_back(ulNewIdleTime, ulNewTotalTime);

                nIndex++;
            }
            else break;
        }

        fProcStatFile.close();
    }
    else
        throw std::runtime_error("Error: Error opening file /proc/stat, no CPU usage info generated!");

    return std::make_tuple(std::move(vPercentages), std::move(vNewPreviousTimes));
}

#elif defined(__unix__) || defined(UNIX)

void panorama::CPUUsage::getCpuUsageData_Unix() {
    std::cout << "getCpuUsageData_Unix" << std::endl;
}

#elif defined(_WIN32)

void panorama::CPUUsage::getCpuUsageData_Window() {
    std::cout << "getCpuUsageData_Window" << std::endl;
}

#endif // PER-PLATFORM IMPLEMENTATION
