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

#include "Globals.h"

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl2.h"
#include <SDL.h>
#include <SDL_opengl.h>

#include <iostream>
#include <string>

#include <unistd.h>
#include <wordexp.h>

#include "MainWindow.h"
#include "IconsFontAwesome5.h"

#define MAIN_WINDOW_WIDTH 1280
#define MAIN_WINDOW_HEIGHT 720

// Global variables
SDL_Window *g_sdlWindow;
SDL_GLContext g_glContext;
float g_fFontScaling = 1.0f;

void loadFonts(ImGuiIO &io) {
    // Load default font
    io.Fonts->AddFontDefault();

    // Check if the regular font fonts exists.
    std::string sFontPath = panorama::utils::getCurrentProcessDir() + "/DroidSans.ttf";
    if (access(sFontPath.c_str(), F_OK) == 0) {
        io.Fonts->AddFontFromFileTTF(sFontPath.c_str(), PANORAMA_FONT_SIZE_REGULAR * g_fFontScaling);

        // Load fontawesome into the regular font
        {
            std::string sFontAwesomePath = panorama::utils::getCurrentProcessDir() + "/fa-solid-900.ttf";
            if (access(sFontAwesomePath.c_str(), F_OK) == 0) {
                static const ImWchar icons_ranges[] = {ICON_MIN_FA, ICON_MAX_FA, 0};
                ImFontConfig icons_config;
                icons_config.MergeMode = true;
                icons_config.PixelSnapH = true;

                io.Fonts->AddFontFromFileTTF(sFontAwesomePath.c_str(),
                                             (PANORAMA_FONT_SIZE_REGULAR - 2) * g_fFontScaling,
                                             &icons_config, icons_ranges);
            }
            else
                std::cerr << "Error: Could not find font " << sFontAwesomePath <<
                          ", no icons will be displayed!" << std::endl;
        }

        io.Fonts->AddFontFromFileTTF(sFontPath.c_str(), PANORAMA_FONT_SIZE_TITLE * g_fFontScaling);
        io.Fonts->AddFontFromFileTTF(sFontPath.c_str(), PANORAMA_FONT_SIZE_EXTRALARGE * g_fFontScaling);
    }
    else {
        std::cerr << "Error: Could not find font " << sFontPath << ", using fallback!" << std::endl;

        // Fallback to default font
        for (int i = 0; i < 3; i++)
            io.Fonts->AddFontDefault();
    }
}

int initApplication() {
    // Setup SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
        std::cerr << "SDL initialization error: " << SDL_GetError()  << std::endl;
        return -1;
    }

    // Setup window
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_DisplayMode current;
    SDL_GetCurrentDisplayMode(0, &current);

    // If we have the PANORAMA_SCALING environment variable, we're gonna scale all fonts by it.
    // A bit of a double check here, but we need this to verify the scaling, since the utility
    // function does not check for errors.
    const char * cstrScalingModifier;
    if ((cstrScalingModifier = std::getenv("PANORAMA_SCALING")) != nullptr) {
        try {
            g_fFontScaling = panorama::guiutils::getScalingFactor();
        }
        catch (const std::invalid_argument &ex) {
            std::cerr << "Error getting scale from PANORAMA_SCALING, using 1 as default." << std::endl;
        }
    }

    // Create SDL window
    std::string sTitle = "Panorama v." PANORAMA_VERSION;
    if (panorama::utils::isRunningInPrivilagedMode())
        sTitle += " (Privileged)";

    g_sdlWindow = SDL_CreateWindow(sTitle.c_str(),
                                   SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                   MAIN_WINDOW_WIDTH * g_fFontScaling, MAIN_WINDOW_HEIGHT * g_fFontScaling,
                                   SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);

    g_glContext = SDL_GL_CreateContext(g_sdlWindow);
    SDL_GL_SetSwapInterval(1); // Enable vsync

    // Setup ImGui binding
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    // Initialize ImGui
    ImGui_ImplSDL2_InitForOpenGL(g_sdlWindow, g_glContext);
    ImGui_ImplOpenGL2_Init();

    // Enable keyboard navigation
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    // Load fonts
    loadFonts(io);

    // Setup style
    ImGui::StyleColorsLight();

    return 0;
}

void destroyApplication() {
    // Cleanup
    ImGui_ImplOpenGL2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(g_glContext);
    SDL_DestroyWindow(g_sdlWindow);
    SDL_Quit();
}

int main(int argc, char **argv) {
    // Init application
    int iInitResult = initApplication();
    if (iInitResult != 0)
        return iInitResult;

    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // Init main window
    panorama::MainWindow wndMain(g_sdlWindow, "", MAIN_WINDOW_WIDTH, MAIN_WINDOW_HEIGHT);
    wndMain.setWindowFlags(ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse);
    wndMain.setMaximized(true);

    // Main loop
    bool done = false;
    while (!done) {
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL2_ProcessEvent(&event);

            if (event.type == SDL_QUIT)
                done = true;
        }

        ImGui_ImplOpenGL2_NewFrame();
        ImGui_ImplSDL2_NewFrame(g_sdlWindow);
        ImGui::NewFrame();

        // Render main window
        wndMain.render();

        // Rendering
        ImGui::Render();
        glViewport(0, 0, (int) ImGui::GetIO().DisplaySize.x, (int) ImGui::GetIO().DisplaySize.y);
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);


        ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

        SDL_GL_SwapWindow(g_sdlWindow);
    }

    destroyApplication();

    return 0;
}
