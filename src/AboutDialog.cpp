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

#include "AboutDialog.h"

void panorama::AboutDialog::renderUI() {
    // Version
    ImGui::Text("Panorama v.%s for %s", PANORAMA_VERSION, PANORAMA_PLATFORM);

    // Determine compiler name`
    std::stringstream sstrCompilerVersion;
#if     defined(__GNUC__)
    sstrCompilerVersion << "GCC " << __GNUC__ << "." << __GNUC_MINOR__ << "." << __GNUC_PATCHLEVEL__;
#elif   defined(__llvm__) || defined(__clang__)
    sstrCompilerVersion << "LLVM/Clang " << __clang_major__ << "."
                                        << __clang_minor__ << "." << __clang_patchlevel__;
#endif

    ImGui::BulletText("Built on %s using %s.", PANORAMA_BUILD_DATE, sstrCompilerVersion.str().c_str());

    ImGui::Separator();

    ImGui::Text("Copyright (c) Ronen Lapushner 2018.");

    ImGui::Text("Redistributed under the GNU GPL v3+ license.");

    ImGui::Separator();

    // IMGUI Version
    ImGui::TextDisabled("ImGui version: %s", IMGUI_VERSION);

    // SDL Version
    {
        SDL_version sdlVersionInfo;

        SDL_VERSION(&sdlVersionInfo);

        ImGui::TextDisabled("SDL version: %d.%d.%d", sdlVersionInfo.major,
                            sdlVersionInfo.minor,
                            sdlVersionInfo.patch);
    }

    ImGui::Separator();

    // Close button
    if (ImGui::Button("Close"))
        ImGui::CloseCurrentPopup();
}
