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

#include "AboutDialog.h"

void panorama::AboutDialog::renderUI() {
    // Version
    ImGui::PushFont(panorama::getFont(PANORAMA_FONT_EXTRALARGE));
    ImGui::Text("PANORAMA");
    ImGui::PopFont();

    ImGui::Text("Version %s for %s", PANORAMA_VERSION, PANORAMA_PLATFORM);

    // Determine compiler name
    std::stringstream sstrCompilerVersion;
#if     defined(__GNUC__)
    sstrCompilerVersion << "GCC " << __GNUC__ << "." << __GNUC_MINOR__ << "." << __GNUC_PATCHLEVEL__;
#elif   defined(__llvm__) || defined(__clang__)
    sstrCompilerVersion << "LLVM/Clang " << __clang_major__ << "."
                                        << __clang_minor__ << "." << __clang_patchlevel__;
#endif

    ImGui::Separator();

    ImGui::Text("Copyright (c) Ronen Lapushner 2018-2019.");
    ImGui::Text("Redistributed under the GNU GPL v3+ license.");

    ImGui::Separator();

    ImGui::Columns(2, "##clmnVersions", false);
    ImGui::Separator();

    // Compiler
    ImGui::TextDisabled("Compiler");
    ImGui::NextColumn();
    ImGui::TextDisabled(sstrCompilerVersion.str().c_str());
    ImGui::NextColumn();

    // Build Date
    ImGui::TextDisabled("Build Date");
    ImGui::NextColumn();
    ImGui::TextDisabled(PANORAMA_BUILD_DATE);
    ImGui::NextColumn();

    // IMGUI version
    ImGui::TextDisabled("IMGui version");
    ImGui::NextColumn();
    ImGui::TextDisabled(IMGUI_VERSION);
    ImGui::NextColumn();

    // SDL version
    ImGui::TextDisabled("SDL Version");
    ImGui::NextColumn();
    {
        SDL_version sdlVersionInfo;

        SDL_VERSION(&sdlVersionInfo);

        ImGui::TextDisabled("%d.%d.%d", sdlVersionInfo.major,
                            sdlVersionInfo.minor,
                            sdlVersionInfo.patch);
        ImGui::NextColumn();
    }

    // FontAwesome version
    // TODO: Find a way to extract this at compile time/runtime - currently hardcoded!
    ImGui::TextDisabled("FontAwesome version");
    ImGui::NextColumn();
    ImGui::TextDisabled("5.0.13");
    ImGui::NextColumn();
}
