#
#  Panorama -  A simple system monitor for Linux, written using dear ImGui.
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

function(GeneratePanoramaVersion)
    # Find git information
    # Get the current working branch
    execute_process(
            COMMAND git rev-parse --abbrev-ref HEAD
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            OUTPUT_VARIABLE GIT_BRANCH
            OUTPUT_STRIP_TRAILING_WHITESPACE
    )

    # Get the latest abbreviated commit hash of the working branch
    execute_process(
            COMMAND git log -1 --format=%h
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            OUTPUT_VARIABLE GIT_COMMIT_HASH
            OUTPUT_STRIP_TRAILING_WHITESPACE
    )

    # Set version string
    if(NOT GIT_BRANCH MATCHES "master")
        # Set versions
        set(PANORAMA_VERSION "${GIT_BRANCH}-${GIT_COMMIT_HASH}")
    else()
        # Read version information from file
        file(READ ${CMAKE_SOURCE_DIR}/version PANORAMA_VERSION)
        string(REGEX REPLACE "\n$" "" PANORAMA_VERSION "${PANORAMA_VERSION}")
        set(PANORAMA_VERSION ${PANORAMA_VERSION} PARENT_SCOPE)
    endif()

    # Set major, minor and patch versions.
    # This'll be used for packaging.
    string(REPLACE "." ";" VERSION_LIST ${PANORAMA_VERSION})

    # Get list length
    list(LENGTH VERSION_LIST VERSION_LIST_LENGTH)

    # Get major and minor versions
    list(GET VERSION_LIST 0 PANORAMA_VERSION_MAJOR)
    list(GET VERSION_LIST 1 PANORAMA_VERSION_MINOR)

    # If patch version exists, get it as well.
    if (VERSION_LIST_LENGTH GREATER 2)
        string(SUBSTRING "${VERSION_LIST}" 4 1 PANORAMA_VERSION_PATCH)
    else()
        set(PANORAMA_VERSION_PATCH 0)
    endif()

    set(PANORAMA_VERSION ${PANORAMA_VERSION} PARENT_SCOPE)
endfunction()
