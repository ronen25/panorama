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

#include "Window.h"

using std::string;

panorama::Window::Window(SDL_Window *pSdlWindow, std::string sTitle, int w, int h) :
        m_pRawWindow{pSdlWindow}, m_sTitle{sTitle}, m_iWidth{w}, m_iHeight{h},
        m_bMaximized{false},
        m_eWindowFlags{ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoCollapse } {
    // If the title is empty, set it to something to avoid an assertion.
    if (m_sTitle.empty())
        m_sTitle = " ";
}

panorama::Window::~Window() { }

void panorama::Window::setWidth(int w) {
    m_iWidth = w;
}

void panorama::Window::setHeight(int h) {
    m_iHeight = h;
}

void panorama::Window::setSize(int w, int h) {
    setWidth(w);
    setHeight(h);
}

void panorama::Window::setTitle(std::string sTitle) {
    m_sTitle = std::move(sTitle);
}

void panorama::Window::setMaximized(bool bMaximized) {
    m_bMaximized = bMaximized;
}

void panorama::Window::setWindowFlags(ImGuiWindowFlags eWinFlags) {
    m_eWindowFlags = eWinFlags;
}

int panorama::Window::width() const {
    return m_iWidth;
}

int panorama::Window::height() const {
    return m_iHeight;
}

std::string panorama::Window::title() const {
    return m_sTitle;
}

bool panorama::Window::maximized() {
    return m_bMaximized;
}

void panorama::Window::render() {
    bool bOpen = false;

    // Setup window before it's drawn
    if (m_bMaximized) {
        int iWidth, iHeight;

        // Get main SDL window's size
        SDL_GetWindowSize(m_pRawWindow, &iWidth, &iHeight);
        m_iWidth = iWidth;
        m_iHeight = iHeight;

        ImGui::SetNextWindowSize(ImVec2(m_iWidth, m_iHeight), ImGuiCond_Always);
        ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);

        m_eWindowFlags |= ImGuiWindowFlags_NoResize;
    }
    else {
        m_eWindowFlags &= ~ImGuiWindowFlags_NoResize;
    }

    // Begin drawing the window
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::Begin(m_sTitle.c_str(), &bOpen, m_eWindowFlags);

    // Overridable UI render function
    renderUI();

    ImGui::End();
    ImGui::PopStyleVar();
}
