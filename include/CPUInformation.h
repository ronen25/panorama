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

#ifndef PANORAMA_CPUINFORMATION_H
#define PANORAMA_CPUINFORMATION_H

#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <utility>
#include <iterator>
#include <sstream>

#include "Utils.h"

/* Supported vendor ID definitions */
#define PANORAMA_CPU_VENDOR_ID_AMD             "AuthenticAMD"
#define PANORAMA_CPU_VENDOR_ID_AMD_K5          "AMDisbetter!"
#define PANORAMA_CPU_VENDOR_ID_CENTAUR         "CentaurHauls"
#define PANORAMA_CPU_VENDOR_ID_CYRIX           "CyrixInstead"
#define PANORAMA_CPU_VENDOR_ID_INTEL           "GenuineIntel"
#define PANORAMA_CPU_VENDOR_ID_TRANSMETA1      "TransmetaCPU"
#define PANORAMA_CPU_VENDOR_ID_TRANSMETA2      "GenuineTMx86"
#define PANORAMA_CPU_VENDOR_ID_NATIONAL        "Geode by NSC"
#define PANORAMA_CPU_VENDOR_ID_NEXGEN          "NexGenDriven"
#define PANORAMA_CPU_VENDOR_ID_RISE            "RiseRiseRise"
#define PANORAMA_CPU_VENDOR_ID_SIS             "SiS SiS SiS"
#define PANORAMA_CPU_VENDOR_ID_UMC             "UMC UMC UMC"
#define PANORAMA_CPU_VENDOR_ID_VIA             "VIA VIA VIA"
#define PANORAMA_CPU_VENDOR_ID_VORTEX          "Vortex86 SoC"
#define PANORAMA_CPU_VENDOR_ID_BHYVE           "bhyve bhyve"
#define PANORAMA_CPU_VENDOR_ID_KVM             "KVMKVMKVM"
#define PANORAMA_CPU_VENDOR_ID_HYPERV          "Microsoft Hv"
#define PANORAMA_CPU_VENDOR_ID_VMWARE          "VMwareVMware"
#define PANORAMA_CPU_VENDOR_ID_XEN             "XenVMMXenVMM"

/* /proc/cpuinfo header names */
#define PANORAMA_HEADER_VENDOR_ID          "vendor_id"
#define PANORAMA_HEADER_MODEL_NUMBER       "model"
#define PANORAMA_HEADER_MODEL_NAME         "model name"
#define PANORAMA_HEADER_CPU_MHZ            "cpu MHz"
#define PANORAMA_HEADER_SIBLINGS           "siblings"
#define PANORAMA_HEADER_CACHE_SIZE         "cache size"
#define PANORAMA_HEADER_CPU_CORES          "cpu cores"
#define PANORAMA_HEADER_FLAGS              "flags"

/* Error codes */
#define PANORAMA_CPUINFORMATION_ERROR_NO_ERROR            (0)
#define PANORAMA_CPUINFORMATION_ERROR_NOMEM               (1 << 1)
#define PANORAMA_CPUINFORMATION_ERROR_NO_CPUINFO_FILE     (1 << 2)


namespace panorama {

    class CPUInformation {
    public:
        // Cnstr.
        CPUInformation();

        // Dstr.
        ~CPUInformation();

        // Getters
        inline const std::string &vendorName() { return m_sVendorName; }
        inline const std::string &modelName() { return m_sModelName; }
        inline int modelNumber() const { return m_nModelNumber; }
        inline int cores() const { return m_nCores; }
        inline int threads() const { return m_nThreads; }
        inline int speedMHZ() const { return m_nCPUSpeedMHZ; }
        inline int cacheSize() const { return m_nCPUSpeedMHZ; }
        inline const std::vector<std::string> features() { return m_vFeatures; }

        // Helpers
        void sanitizeModelName();

    private:
        // Per-platform implementation of the data getter function
#       if defined(__linux__) || defined(LINUX)
        void getInformation_Linux();

#       define getInformation getInformation_Linux
#       endif

        // Properties
        std::string m_sVendorName;
        std::string m_sModelName;
        int m_nModelNumber;
        int m_nCores, m_nThreads;
        int m_nCPUSpeedMHZ;
        int m_nCacheSize;
        std::vector<std::string> m_vFeatures;
    };

}

#endif // PANORAMA_CPUINFORMATION_H