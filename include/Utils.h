/*
 *  Panorama -  A simple system monitor for Linux, written using IMGui.
 *  Copyright (C) 2018 Ronen Lapushner
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

#ifndef PANORAMA_UTILS_H
#define PANORAMA_UTILS_H

#include <string>
#include <iterator>
#include <algorithm>
#include <fstream>
#include <exception>
#include <cctype>
#include <cstring>

#if defined(__linux__) || defined(LINUX)
#   include <unistd.h>
#   include <sys/types.h>
#   include <limits.h>
#endif // Includes

#include "imgui.h"

namespace panorama {
    namespace utils {
        void trimString(std::string &str);

        std::string extractValueFromFile(const std::string &sFilePath);

        bool isNumber(const char *cstr);

        bool isRunningInPrivilagedMode();

        std::string getCurrentProcessDir();
    }

    namespace guiutils {
        void drawBackgroundTextOnGraph(ImFont *fntTextFont, const ImVec2 &v2GraphSize,
                                       const std::string &sDrawnString, float fTextAlpha);
    }
}

#endif // PANORAMA_UTILS_H
