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

#ifndef PANORAMA_SIDEBAR_H
#define PANORAMA_SIDEBAR_H

#include <SDL.h>

#include <cstdlib>

#include "imgui.h"

#include "AboutDialog.h"
#include "Globals.h"
#include "FontDefinitions.h"
#include "Theming.h"
#include "Utils.h"

#define ITEMS_AT_BOTTOM 2

namespace panorama {

    // Enums
    enum PaneType {
        PANETYPE_CPU,
        PANETYPE_PROCESSES,
        PANETYPE_MEMORY
    };

    class Sidebar {
    public:
        // Cnstr.
        Sidebar(float fWidth);

        // Dstr.
        ~Sidebar();

        // Methods
        void renderUI();

        // Getters
        inline float width() const { return m_fWidth; }
        inline PaneType currentlyVisiblePane() const { return m_eCurrentlyVisiblePane; }

    private:
        // Properties
        float m_fWidth;
        PaneType m_eCurrentlyVisiblePane;
    };

}

#endif //PANORAMA_SIDEBAR_H
