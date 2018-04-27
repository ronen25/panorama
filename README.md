
<p align="center">
  <img src="https://github.com/ronen25/panorama/blob/master/images/panorama_logo_with_text.png">
</p>

A simple system monitor for Linux.

**Current version: 1.0.0**

-------------

<p align="center">
  <img src="https://github.com/ronen25/panorama/blob/master/images/screenshot.png">
</p>

-------------
[![Snap Status](https://build.snapcraft.io/badge/ronen25/panorama.svg)](https://build.snapcraft.io/user/ronen25/panorama)
-------------

What is it?
----------------
Panorama is a very simple Linux system monitor written in C++ using the IMGui UI toolkit. It's meant to show that writing beautiful and functional tools using IMGui is quite easy.

Supported Platforms
-----------------------
Currently the tool was tested on KDE Neon 5.12.3, Ubuntu 16.04 LTS, and Fedora WS 27 with the AMD64 and x86 variants.

Any video driver that has support for OpenGL 2.1 will suffice. Tested with AMD, Intel, NVIDIA and Virtualbox drivers.

Building From Source
--------------------
**Note:** Your compiler must be fully compatible with the C++14 standard!

1) Install the required dependencies:

|Platform|Command|
|--------|-------|
| Debian/Ubuntu | ``` $ sudo apt-get install libsdl2-dev libsdl2-image-dev libmesa-dev libcap-dev cmake build-essential python```|
| Fedora | ``` $ sudo dnf install SDL2-devel SDL2_image-devel mesa-libGL-devel libcap-devel cmake gcc-c++ python2```|

2) Clone the project:

```
git clone https://github.com/ronen25/panorama
```

3) Go to the `panorama` directory and create a `build` directory:
```
cd panorama
mkdir build && cd build
```

4) Initiate a build using CMake:
```
cmake -DCMAKE_BUILD_TYPE=Release ..
```

5) _(Optional)_ Install the program (elevated permissions might be required):
```
make install
```

FAQ
------
1 *I have a HiDPI screen and Panorama's font looks small :-(*

Set the `PANORAMA_SCALING` environment variable to 2.0 or above.
This will also scale other UI elements.

2 *When Panorama opens, all I get is a black window...*

Panorama uses ImGui which uses OpenGL 2.1 (through SDL2) to render the window.
Make sure you have the latest video drivers!

----------------------

TODO
------
At first I intended Panorama to be a cross-platform system monitor. Because of time constrains I could not complete
the cross-platform support, so I only coded the Linux support.

Also, need to replace the concurrent scheduling bit with a single worker
thread with a work queue of some sort, which'll work better than the whole ```future``` stuff done here.

License
--------
This software is licensed under the [GNU GPLv3 License](https://www.gnu.org/licenses/gpl-3.0.en.html).
