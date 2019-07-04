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

#include "Utils.h"

using std::string;

void panorama::utils::trimString(std::string &str) {
    str.erase(str.begin(), std::find_if(str.begin(), str.end(), [](int ch) {
        return !std::isspace(ch);
    }));

    str.erase(std::find_if(str.rbegin(), str.rend(), [](int ch) {
        return !std::isspace(ch);
    }).base(), str.end());
}

std::string panorama::utils::extractValueFromFile(const std::string &sFilePath) {
    std::ifstream fFile{sFilePath};
    std::string sContents;

    if (fFile.is_open())
        std::getline(fFile, sContents);

    fFile.close();

    return sContents;
}

bool panorama::utils::isNumber(const char *cstr) {
    const size_t szLen = std::strlen(cstr);
    for (size_t i = 0; i < szLen; i++)
        if (!std::isdigit(cstr[i]))
            return false;

    return true;
}

bool panorama::utils::isRunningInPrivilagedMode() {
#if defined(__linux__) || defined(LINUX)
    return getuid() == 0;
#endif // Per-platform implementation
}

string panorama::utils::getCurrentProcessDir() {
    char path[PATH_MAX];
    string retPath;
    memset(path, 0, sizeof(path));

    if (readlink("/proc/self/exe", path, PATH_MAX) != -1) {
        retPath = path;
        retPath.erase(retPath.find_last_of('/'));
    }
    else
        return "";

    return retPath;
}

void panorama::guiutils::drawBackgroundTextOnGraph(ImFont *fntTextFont,
                                                   const ImVec2 &v2GraphSize,
                                                   const std::string &sDrawnString,
                                                   float fTextAlpha) {
    ImGui::PushFont(fntTextFont);

    const ImVec2 v2TextTotalSize = ImGui::CalcTextSize(sDrawnString.c_str());
    const ImVec2 v2GraphRect = ImGui::GetItemRectMin();
    const float fTextX = v2GraphRect.x + (v2GraphSize.x - v2TextTotalSize.x) / 2;
    const float fTextY = v2GraphRect.y + (v2GraphSize.y - v2TextTotalSize.y) / 2;

    ImDrawList *drawList = ImGui::GetWindowDrawList();
    drawList->AddText(fntTextFont, fntTextFont->FontSize,
                      ImVec2(fTextX, fTextY), ImGui::GetColorU32(ImGuiCol_PlotLines, 0.4f),
                      sDrawnString.c_str());

    ImGui::PopFont();
}

float panorama::guiutils::getScalingFactor() {
    const char *cstrScaling = std::getenv("PANORAMA_SCALING");
    if (cstrScaling != nullptr)
        return std::stof(cstrScaling);

    return 1.0f; // Default scaling
}
