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

#include "ProcessList.h"

panorama::ProcessList::ProcessList() { }
panorama::ProcessList::~ProcessList() { }

std::future<std::vector<panorama::ProcessInfo>> panorama::ProcessList::launchProcessListGetterTask() {
    return std::async(std::launch::async, ProcessList::getProcessList);
}

bool panorama::ProcessList::isTaskReady(std::future<std::vector<panorama::ProcessInfo>> &futTask) {
    return futTask.wait_for(std::chrono::seconds{0}) == std::future_status::ready;
}

void panorama::ProcessList::updateData(std::future<std::vector<panorama::ProcessInfo>> &futTask) {
    m_vProcesses = futTask.get();
}

// Static factory method
std::vector<panorama::ProcessInfo> panorama::ProcessList::getProcessList_Linux() {
    std::vector<ProcessInfo> vProcessList;
    float fTotalUptime;

    // Get the total system uptime.
    // We'll use this to calculate the CPU usage by every process.
    std::ifstream fProcUptime{"/proc/uptime"};
    if (fProcUptime.is_open()) {
        std::string sValue;

        std::getline(fProcUptime, sValue, ' ');
        fTotalUptime = std::stof(sValue);

        fProcUptime.close();
    }

    // Open the /proc directory
    auto dProcDir = opendir("/proc");

    while (struct dirent *stDirEnt = readdir(dProcDir)) {
        ProcessInfo oProcInfo;
        std::string const sProcInfoDir = "/proc/" + std::string(stDirEnt->d_name);
        unsigned long long ullUTime, ullSTime, ullCUTime, ullCSTime, ullStartTime;

        // Don't process anything that is not a directory, not a PID,
        // or a dot/dot-dot.
        if (stDirEnt->d_type != DT_DIR ||
            !panorama::utils::isNumber(stDirEnt->d_name) ||
            std::strcmp(stDirEnt->d_name, ".") == 0 ||
            std::strcmp(stDirEnt->d_name, "..") == 0)
            continue;
        
        // Get the executable's path, which would serve as the process name.
        {
            std::string sExeLink = sProcInfoDir + "/exe";

            // Follow the link
            char *cstrRealPath = realpath(sExeLink.c_str(), nullptr);
            if (cstrRealPath != nullptr) {
                oProcInfo.m_sName = std::string(cstrRealPath);
                std::free(cstrRealPath);
            }
        }

        // Try and locate the cmdline file.
        std::ifstream fProcCmdlineFile{sProcInfoDir + "/cmdline"};
        if (fProcCmdlineFile.is_open()) {
            std::string sValue;
            while (std::getline(fProcCmdlineFile, sValue, '\0'))
                oProcInfo.m_sCommand += sValue + " ";

            // If the process' name wasn't detected from the exe file,
            // extract it fromm the command.
            if (oProcInfo.m_sName.empty())
                oProcInfo.m_sName = oProcInfo.m_sCommand.substr(0, oProcInfo.m_sCommand.find(' '));

            fProcCmdlineFile.close();
        }

        // Right not, the executable name should somehow be populated, so we want to
        // erase the executable's path.
        if (!oProcInfo.m_sName.empty()) {
            auto sztLastSlashPos = oProcInfo.m_sName.rfind('/');
            if (sztLastSlashPos != std::string::npos)
                oProcInfo.m_sName.erase(0, sztLastSlashPos + 1);
        }

        // Open the "stat" file and read all data from it
        std::ifstream fProcStatFile{sProcInfoDir + "/stat"};
        if (fProcStatFile.is_open()) {
            int nIndex = 1;
            std::string sLine, sValue;

            std::getline(fProcStatFile, sLine);
            std::stringstream sstrData{sLine};

            while (sstrData >> sValue && nIndex <= PROCLIST_PROC_STAT_MAX_INDEX) {
                switch (nIndex) {
                    case PROCLIST_PROC_STAT_PID:
                        oProcInfo.m_nPid = static_cast<PANORAMA_PROCESSID_TYPE>(std::stoul(sValue));
                        break;
                    case PROCLIST_PROC_STAT_NAME:
                        if (oProcInfo.m_sName.empty())
                            oProcInfo.m_sName = sValue;
                        break;
                    case PROCLIST_PROC_STAT_NUM_THREADS:
                        oProcInfo.m_nThreads = std::stoi(sValue);
                        break;
                    case PROCLIST_PROC_STAT_UTIME:
                        ullUTime = std::stoull(sValue);
                        break;
                    case PROCLIST_PROC_STAT_STIME:
                        ullSTime = std::stoull(sValue);
                        break;
                    case PROCLIST_PROC_STAT_CUTIME:
                        ullCUTime = std::stoull(sValue);
                        break;
                    case PROCLIST_PROC_STAT_CSTIME:
                        ullCSTime = std::stoull(sValue);
                        break;
                    case PROCLIST_PROC_STAT_NICE:
                        oProcInfo.m_nPriority = std::stoi(sValue);
                        break;
                    case PROCLIST_PROC_STAT_STARTTIME:
                        ullStartTime = std::stoull(sValue);
                        break;
                }

                nIndex++;
            }

            // Close file
            fProcStatFile.close();
        }
        else {
            std::cerr << "Error opening " << stDirEnt->d_name << "/stat" << std::endl;
        }

        // If it's a "special" process, remove it from the list.
        if (oProcInfo.m_nPid == 1)
            continue;

        // Get process memory usage
        std::ifstream fProcStatmFile{sProcInfoDir + "/statm"};
        if (fProcStatmFile.is_open()) {
            std::string sValue;
            const long lPageSize = sysconf(_SC_PAGESIZE);

            fProcStatmFile >> sValue;
            oProcInfo.m_ullVirtMemSize = std::stoull(sValue) * lPageSize;

            fProcStatmFile >> sValue;
            oProcInfo.m_ullResidentMemSize = std::stoull(sValue) * lPageSize;

            fProcStatmFile >> sValue;
            oProcInfo.m_ullSharedMemSize = std::stoull(sValue) * lPageSize;

            fProcStatmFile.close();
        }

        // Get process CPU usage
        unsigned long long ullTotalProcessTime = ullSTime + ullUTime + ullCSTime + ullCUTime;
        float fSecondsSinceStarted = fTotalUptime - (static_cast<float>(ullStartTime) / sysconf(_SC_CLK_TCK));
        oProcInfo.m_fCpuUsagePercent = 10 * ((static_cast<float>(ullTotalProcessTime) / sysconf(_SC_CLK_TCK))
                                               / fSecondsSinceStarted);

        // Get niceness
        oProcInfo.m_nPriority = getpriority(PRIO_PROCESS, oProcInfo.m_nPid);

        // Push to the process list if it's not a kernel process
        if (oProcInfo.m_sName.find('(') == std::string::npos)
            vProcessList.push_back(oProcInfo);
    }

    // Close the directory
    closedir(dProcDir);

    // Sort the list
    std::stable_sort(vProcessList.begin(), vProcessList.end(),
                     [](const ProcessInfo &procInfo1, const ProcessInfo &procInfo2) {
        return procInfo1.m_fCpuUsagePercent > procInfo2.m_fCpuUsagePercent;
    });

    // Return
    return std::move(vProcessList);
}

