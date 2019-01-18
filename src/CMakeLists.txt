#
#  Panorama -  A simple system monitor for Linux, written using IMGui.
#  Copyright (C) 2018-2019 Ronen Lapushner
#
#  This program is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program.  If not, see <https://www.gnu.org/licenses/>.
#

# Set gl3w sources
set(GL3W_SRCS  ${CMAKE_SOURCE_DIR}/src/gl3w.c)

# Set IMGUI sources
set(IMGUI_SRCS ${CMAKE_SOURCE_DIR}/imgui/imgui.cpp
               ${CMAKE_SOURCE_DIR}/imgui/imgui_draw.cpp
               ${CMAKE_SOURCE_DIR}/imgui/imgui_widgets.cpp
               ${CMAKE_SOURCE_DIR}/imgui/imgui_impl_opengl2.cpp
               ${CMAKE_SOURCE_DIR}/imgui/imgui_impl_sdl.cpp)

# Set panorama sources
set(SRCS panorama.cpp
         CPUInformation.cpp
         CPUUsage.cpp
         ProcessList.cpp
         Utils.cpp
         PlotColorsArray.cpp
         Sidebar.cpp
         CPUPane.cpp
         AboutDialog.cpp
         ProcessListPane.cpp
         ProcessUtils.cpp
         MemoryInfo.cpp
         MemoryInfoPane.cpp
         Window.cpp
         MainWindow.cpp
         Theming.cpp)

# Create the configuration file
configure_file("${CMAKE_SOURCE_DIR}/config.h.in"
               "${CMAKE_BINARY_DIR}/config.h")

# Add binary target
add_executable(panorama ${GL3W_SRCS}
                        ${IMGUI_SRCS}
                        ${SRCS})
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

target_include_directories(panorama PRIVATE ${CMAKE_SOURCE_DIR}/imgui
        ${CMAKE_SOURCE_DIR}/gl3w
        ${CMAKE_SOURCE_DIR}/gui
        ${CMAKE_BINARY_DIR}
        ${CMAKE_SOURCE_DIR}/include
        ${SDL2_INCLUDE_DIRS})

target_link_libraries(panorama Threads::Threads ${OPENGL_LIBRARIES} dl SDL2 cap)

# Installation
install(TARGETS panorama DESTINATION bin)
install(FILES "${CMAKE_SOURCE_DIR}/fonts/DroidSans.ttf" DESTINATION bin)
install(FILES "${CMAKE_SOURCE_DIR}/fonts/fa-solid-900.ttf" DESTINATION bin)
install(FILES ${CMAKE_SOURCE_DIR}/images/panorama.svg DESTINATION bin)
