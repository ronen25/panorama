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

#ifndef PANORAMA_SETTINGSDIALOG_H
#define PANORAMA_SETTINGSDIALOG_H

#include "imgui.h"
#include "config.h"

#include "MemoryUnits.h"
#include "Theming.h"

namespace panorama {

    class SettingsDialog {
    public:
        static void renderUI();
    };
}

#endif /* PANORAMA_SETTINGSDIALOG_H */