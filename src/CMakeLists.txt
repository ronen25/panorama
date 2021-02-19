#
#  Panorama -  A simple system monitor for Linux, written using dear ImGui.
#  Copyright (C) 2018-2021 Ronen Lapushner
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

# imgui directory
SET(IMGUI_DIR ${CMAKE_SOURCE_DIR}/imgui)

# Set IMGUI sources
set(IMGUI_SRCS ${IMGUI_DIR}/imgui.cpp
               ${IMGUI_DIR}/imgui_draw.cpp
               ${IMGUI_DIR}/imgui_widgets.cpp
               ${IMGUI_DIR}/imgui_tables.cpp
               ${IMGUI_DIR}/backends/imgui_impl_opengl2.cpp
               ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp)

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
         MainWindow.cpp
         Theming.cpp
         MeasurementUnit.cpp)

# Create the configuration file
configure_file("${CMAKE_SOURCE_DIR}/config.h.in"
               "${CMAKE_BINARY_DIR}/config.h")

# Add binary target
add_executable(panorama ${GL3W_SRCS}
                        ${IMGUI_SRCS}
                        ${SRCS})
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

target_include_directories(panorama PRIVATE 
        ${CMAKE_SOURCE_DIR}/imgui
        ${CMAKE_SOURCE_DIR}/imgui/backends
        ${CMAKE_BINARY_DIR}
        ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(panorama Threads::Threads ${OPENGL_LIBRARIES} dl glfw cap)

# Installation
install(TARGETS panorama DESTINATION bin)
install(FILES ${CMAKE_SOURCE_DIR}/fonts/DroidSans.ttf DESTINATION bin)
install(FILES ${CMAKE_SOURCE_DIR}/fonts/fa-solid-900.ttf DESTINATION bin)
install(FILES ${CMAKE_SOURCE_DIR}/images/panorama.svg DESTINATION bin)

# Copy the fonts post-build to the build directory
FILE(COPY ${CMAKE_SOURCE_DIR}/fonts/DroidSans.ttf DESTINATION ${CMAKE_BINARY_DIR})
FILE(COPY ${CMAKE_SOURCE_DIR}/fonts/fa-solid-900.ttf DESTINATION ${CMAKE_BINARY_DIR})
FILE(COPY ${CMAKE_SOURCE_DIR}/images/panorama.svg DESTINATION ${CMAKE_BINARY_DIR})
