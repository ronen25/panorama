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

#ifndef PANORAMA_PROCESSUTILS_H
#define PANORAMA_PROCESSUTILS_H

#if defined(__linux__) || defined(LINUX)
#   include <sys/time.h>
#   include <sys/resource.h>
#   include <sys/types.h>
#   include <sys/capability.h>
#   include <signal.h>
#   include <errno.h>
#endif // Includes

namespace panorama {

    class ProcessUtils {
    public:
#       if defined(__linux__) || defined(LINUX)

        // Definitions
#       define PANORAMA_PROCESSID_TYPE pid_t

        // Library utility functions
        static int terminateProcess_Linux(PANORAMA_PROCESSID_TYPE pid);
        static int killProcess_Linux(PANORAMA_PROCESSID_TYPE pid);
        static int changeProcessPriority_Linux(PANORAMA_PROCESSID_TYPE pid, int prio);
        static int getPriority_Linux(PANORAMA_PROCESSID_TYPE pid);

        static bool canChangePriority_Linux();

#       define terminateProcess terminateProcess_Linux
#       define killProcess killProcess_Linux
#       define changeProcessPriority changeProcessPriority_Linux
#       define getPriority getPriority_Linux
#       define canChangePriority canChangePriority_Linux

#       endif // Functions and definitions
    };

}

#endif //PANORAMA_PROCESSUTILS_H
