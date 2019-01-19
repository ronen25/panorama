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

#ifndef PROCLIST_H
#define PROCLIST_H

#include <iostream>
#include <fstream>
#include <future>
#include <vector>
#include <string>
#include <sstream>
#include <chrono>

#include <cstring>

#include "Utils.h"
#include "ProcessUtils.h"

#if defined(__linux__) || defined(LINUX)

// INCLUDES
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/resource.h>
#include <limits.h>
#include <unistd.h>

// Defines for parsing /proc/stat
#define PROCLIST_PROC_STAT_PID 1
#define PROCLIST_PROC_STAT_NAME 2
#define PROCLIST_PROC_STAT_NUM_THREADS 20
#define PROCLIST_PROC_STAT_UTIME 14
#define PROCLIST_PROC_STAT_STIME 15
#define PROCLIST_PROC_STAT_CUTIME 16
#define PROCLIST_PROC_STAT_CSTIME 17
#define PROCLIST_PROC_STAT_NICE   19
#define PROCLIST_PROC_STAT_STARTTIME 22
#define PROCLIST_PROC_STAT_MAX_INDEX 22

#endif // INCLUDES AND DEFINES

namespace panorama {

    struct ProcessInfo {
        PANORAMA_PROCESSID_TYPE m_nPid;
        std::string m_sName, m_sCommand;
        unsigned long long m_ullVirtMemSize, m_ullResidentMemSize, m_ullSharedMemSize;
        float m_fCpuUsagePercent;
        int m_nThreads;
        int m_nPriority;
    };

    class ProcessList {
    public:
        // Cnstr.
        ProcessList();

        // Dstr.
        ~ProcessList();

        // Task launchers
        std::future<std::vector<ProcessInfo>> launchProcessListGetterTask();
        bool isTaskReady(std::future<std::vector<ProcessInfo>> &futTask);
        void updateData(std::future<std::vector<ProcessInfo>> &futTask);

        // Getters
        inline const std::vector<ProcessInfo> &data() { return m_vProcesses; }

        // Per-platform process list getters
#       if defined(LINUX) || defined(__linux__)

        static std::vector<ProcessInfo> getProcessList_Linux();

#       define getProcessList getProcessList_Linux

#       endif // getProcessList()

    private:
        // Properties
        std::vector<ProcessInfo> m_vProcesses;
        unsigned long long m_ullUptime;
    };


}

#endif // PROCLIST_H

