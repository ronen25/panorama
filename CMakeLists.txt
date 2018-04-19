#
#  Panorama -  A simple system monitor for Linux, written using IMGui.
#  Copyright (C) 2018 Ronen Lapushner
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

cmake_minimum_required(VERSION 3.0)
project(panorama)

# Set module include path
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

# Includes
include(FindOpenGL)
include(FindThreads)
include(GeneratePanoramaVersion)

# Set version and build date
GeneratePanoramaVersion(PANORAMA_VERSION)

# Set project properties
string(TIMESTAMP PANORAMA_BUILD_DATE "%Y-%m-%d %H:%M:%S" UTC)
set(PANORAMA_PLATFORM ${CMAKE_SYSTEM_NAME})

# Set standard
set(CMAKE_CXX_STANDARD 14)

# Find dependencies
find_package(OpenGL  REQUIRED)
find_package(SDL2    REQUIRED)
find_package(Threads REQUIRED)

# Generate GL3W headers using the provided script
message("Downloading gl3w sources into Panorama source tree...")
execute_process(COMMAND "python" "${CMAKE_SOURCE_DIR}/gl3w/gl3w_gen.py" "--ext" "--root=.."
                WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/gl3w")

# Uninstall target
configure_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
        IMMEDIATE @ONLY)

add_custom_target(uninstall
                  COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)

# Add subfolders
add_subdirectory(src)
