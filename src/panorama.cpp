/*
 *  Panorama -  A simple system monitor for Linux, written using dear ImGui.
 *  Copyright (C) 2018-2021 Ronen Lapushner
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
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl2.h"

#include <iostream>
#include <string>

#include <unistd.h>
#include <wordexp.h>

#include <GLFW/glfw3.h>

#include "MainWindow.h"
#include "IconsFontAwesome5.h"

// Global variables
GLFWwindow *g_glfwWindow;
float g_fFontScaling = 1.0f;

using std::cerr;
using std::endl;

constexpr int MAIN_WINDOW_WIDTH = 1280;
constexpr int MAIN_WINDOW_HEIGHT = 720;

static void glfw_error_callback(int error, const char *description) {
    cerr << description << " (" << error << ")" << endl;
}

static bool initGLFW() {
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return false;

    return true;
}

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
                cerr << "Error: Could not find font " << sFontAwesomePath <<
                        ", no icons will be displayed!" << endl;
        }

        io.Fonts->AddFontFromFileTTF(sFontPath.c_str(), PANORAMA_FONT_SIZE_TITLE * g_fFontScaling);
        io.Fonts->AddFontFromFileTTF(sFontPath.c_str(), PANORAMA_FONT_SIZE_EXTRALARGE * g_fFontScaling);
    }
    else {
        cerr << "Error: Could not find font " << sFontPath << ", using fallback!" << endl;

        // Fallback to default font
        for (int i = 0; i < 3; i++)
            io.Fonts->AddFontDefault();
    }
}

int initApplication() {
    // Setup GLFW
    if (!initGLFW()) {
        return EXIT_FAILURE;
    }

    // If we have the PANORAMA_SCALING environment variable, we're gonna scale all fonts by it.
    // A bit of a double check here, but we need this to verify the scaling, since the utility
    // function does not check for errors.
    const char *cstrScalingModifier;
    if ((cstrScalingModifier = std::getenv("PANORAMA_SCALING")) != nullptr) {
        try {
            g_fFontScaling = panorama::guiutils::getScalingFactor();
        }
        catch (const std::invalid_argument &ex) {
            cerr << "Error getting scale from PANORAMA_SCALING, using 1 as default." << endl;
        }
    }

    std::string sTitle = "Panorama v." PANORAMA_VERSION;
    if (panorama::utils::isRunningInPrivilagedMode())
        sTitle += " (Privileged)";

    // Create window
    g_glfwWindow = glfwCreateWindow(MAIN_WINDOW_WIDTH, MAIN_WINDOW_HEIGHT, sTitle.c_str(), nullptr, nullptr);
    if (!g_glfwWindow) {
        cerr << "Error creating GLFW window." << endl;
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(g_glfwWindow);
    glfwSwapInterval(1);

    // Setup ImGui binding
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();


    // Initialize ImGui
    ImGui_ImplGlfw_InitForOpenGL(g_glfwWindow, true);
    ImGui_ImplOpenGL2_Init();

    // Load fonts
    loadFonts(io);

    return 0;
}

void destroyApplication() {
    // Cleanup
    ImGui_ImplOpenGL2_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(g_glfwWindow);
    glfwTerminate();
}

int main(int argc, char **argv) {
    const ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // Init application
    int iInitResult = initApplication();
    if (iInitResult != 0)
        return iInitResult;

    // Init main window
    panorama::MainWindow wndMain(g_glfwWindow, "", MAIN_WINDOW_WIDTH, MAIN_WINDOW_HEIGHT);


    // Setup style
    ImGui::StyleColorsLight();

    // Main loop
    while (!glfwWindowShouldClose(g_glfwWindow)) {
        glfwPollEvents();

        ImGui_ImplOpenGL2_NewFrame();
        ImGui_ImplGlfw_NewFrame();

        ImGui::NewFrame();

        // Render main window
        wndMain.renderUI();

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(g_glfwWindow, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

        glfwMakeContextCurrent(g_glfwWindow);
        glfwSwapBuffers(g_glfwWindow);
    }

    destroyApplication();

    return 0;
}
