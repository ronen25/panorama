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

#ifndef PANORAMA_WINDOW_H
#define PANORAMA_WINDOW_H

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl2.h"

#include <SDL.h>

#include <string>
#include <imgui_internal.h>

namespace panorama {
    class Window {
    public:
        // Cnstr.
        Window(SDL_Window *pSdlWindow, std::string sTitle, int w, int h);

        // Dstr.
        virtual ~Window();

        // Setters
        void setWidth(int w);
        void setHeight(int h);
        void setSize(int w, int h);

        void setTitle(std::string sTitle);

        void setMaximized(bool bMaximized);

        void setWindowFlags(ImGuiWindowFlags eWinFlags);
        // Getters
        int width()  const;
        int height() const;

        bool maximized();

        std::string title() const;

        // Render methods
        void render();

    private:
        // Overridable UI render thread
        virtual void renderUI() = 0;

        // Properties
        SDL_Window *m_pRawWindow;
        int m_iWidth, m_iHeight;
        std::string m_sTitle;
        bool m_bMaximized;
        ImGuiWindowFlags m_eWindowFlags;
    };

}

#endif // PANORAMA_WINDOW_H
