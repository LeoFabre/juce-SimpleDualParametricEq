# WORK IN PROGRESS - SimpleDualParametricEq - CMake, JUCE 6.1.6, ElkPi
This is a JUCE-based Parametric EQ audio plugin that I'm making for my dub sound system control tower.

The plugin is intended to be used in a live sound system setup, but it can also be used in a studio environment.

Please feel free to contribute by opening a PR or an issue.

# Features
- 2 parametric bell filters
- adjustable frequency, gain, and Q
- kill switches for each filter


# Building the plugin

## Prerequisites
- CMake 3.15 or newer
- OPTIONAL - A VST3 host to test the plugin (like Ableton, FL Studio, Pro Tools, Carla, Ardour, Reaper, etc.)

Modify the `set(VST3_COPY_DIR "C:/Program Files/VST")` line in the CMakeLists.txt file to point to your VST3 plugins folder.  
If you don't want to copy the plugin to your VST3 plugins folder, you can remove this line and set `COPY_PLUGIN_AFTER_BUILD` to FALSE.  
The installation step will fail if the folder is protected by the OS. You can run your terminal or IDE as an administrator to work around this.  
## Building steps (using CLion)
1. Clone the repository
2. Open the repository folder in CLion
3. Load the CMake project if not done automatically
4. Select the SimpleDualParametricEq_VST3 target
5. Build the project
6. The plugin will be in the `cmake-build-debug/SimpleDualParametricEq_artefacts/Debug/VST3` folder
7. Copy the `.vst3` file to your VST3 plugins folder
8. Open your VST3 host and check if the plugin is available
9. Enjoy!

## Building steps (Windows)
1. Clone the repository
2. Open a terminal and navigate to the repository folder
3. Create a build directory: `mkdir build && cd build`
4. Run CMake: `cmake .. -G "Visual Studio 16 2019" -A x64`
5. Open the generated solution file in Visual Studio
6. Build the SimpleDualParametricEq_VST3 project in Release mode
7. The plugin will be in the `build\SimpleDualParametricEq_artefacts\Release\VST3` folder
8. Copy the `.vst3` file to your VST3 plugins folder
9. Open your VST3 host and check if the plugin is available
10. Enjoy!

## Building steps (Linux, macOS)
1. Clone the repository
2. Open a terminal and navigate to the repository folder
3. Create a build directory: `mkdir build && cd build`
4. Run CMake: `cmake ..`
5. Build the project: `make -j $(nproc)` or `cmake --build . --target SimpleDualParametricEq_VST3 --config Release`
6. The plugin will be in the `build/SimpleDualParametricEq_artefacts/Debug/VST3` folder
7. Copy the `.vst3` file to your VST3 plugins folder
8. Open your VST3 host and check if the plugin is available
9. Enjoy!


# CrossBuilding for ElkPi

This will only work with JUCE 6, and the juceaide CMakeLists.txt needs to get rid of all the cross-compile unset(xxx)
because sourcing the SDK will set all the needed stuff and the unset(xxx) calls will override the SDK thus making the
build fail

[from the original ELK docs](https://elk-audio.github.io/elk-docs/html/documents/building_plugins_for_elk.html#plugins-using-juce-version-6)

I personally build on a WSL2 Ubuntu 22.04.4, but it should work on any linux machine.

## install SDK on a linux machine (first time only)

Get the Elk cross-build SDK from there and run it to install (ideally keep the default destinations) :
  https://github.com/elk-audio/elkpi-sdk/releases

## Build

With the Elk crossbuild SDK installed, cd to the repo folder, then run these commands in the following order :

- ```mkdir x-build && cd x-build```

- ```unset LD_LIBRARY_PATH && source /opt/elk/1.0.0/environment-setup-cortexa72-elk-linux```

- ```export CXXFLAGS="-O3 -pipe -ffast-math -feliminate-unused-debug-types -funroll-loops"```

- ```cmake ../ -DCMAKE_BUILD_TYPE=Release```

- ```AR=aarch64-elk-linux-ar make -j `nproc` CONFIG=Release CFLAGS="-Wno-psabi" TARGET_ARCH="-mcpu=cortex-a72 -mtune=cortex-a72"```

The resulting x-build/SimpleDualParametricEq_artefacts/Debug/VST3/SimpleDualParametricEq.vst3 is ready to use on your Elk-Pi device.
