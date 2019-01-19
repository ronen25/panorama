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

#include "ProcessUtils.h"

// Per-platform implementations of ProcessUtil's functions

#if defined(__linux__) || defined(LINUX)

int panorama::ProcessUtils::terminateProcess_Linux(PANORAMA_PROCESSID_TYPE pid) {
    int nKillResult = kill(pid, SIGTERM);

    return nKillResult;
}

int panorama::ProcessUtils::killProcess_Linux(PANORAMA_PROCESSID_TYPE pid) {
    int nKillResult = kill(pid, SIGKILL);

    return nKillResult;
}

int panorama::ProcessUtils::changeProcessPriority_Linux(PANORAMA_PROCESSID_TYPE pid, int prio) {
    int nReturn = setpriority(PRIO_PROCESS, pid, prio);

    return nReturn;
}

int panorama::ProcessUtils::getPriority_Linux(PANORAMA_PROCESSID_TYPE pid) {
    errno = 0;
    int nPrio = getpriority(PRIO_PROCESS, pid);
    if (errno != 0)
        nPrio = -1;

    return nPrio;
}

bool panorama::ProcessUtils::canChangePriority_Linux() {
    cap_t caps = cap_get_proc();
    cap_flag_value_t flag;

    cap_get_flag(caps, CAP_SYS_NICE, CAP_EFFECTIVE, &flag);

    return flag == 1;
}

#endif
