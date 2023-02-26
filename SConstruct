#!/usr/bin/env python
import os
import sys

env = SConscript("godot-cpp/SConstruct")

# For the reference:
# - CCFLAGS are compilation flags shared between C and C++
# - CFLAGS are for C-specific compilation flags
# - CXXFLAGS are for C++-specific compilation flags
# - CPPFLAGS are for pre-processor flags
# - CPPDEFINES are for pre-processor defines
# - LINKFLAGS are for linking flags

# tweak this if you want to use different folders, or more folders, to store your source code in.
env.Append(CPPPATH=["src/", "C:/Program Files/Microsoft SDKs/Kinect/v2.0_1409/inc", "src/include", "godot-cpp/include/godot_cpp"])
env.Append(LINKFLAGS=["C:/Program Files/Microsoft SDKs/Kinect/v2.0_1409/Lib/x64/Kinect20.lib"])
sources = Glob("src/*.cpp")

library = env.SharedLibrary(
    "project/bin/libgkinect{}{}".format(env["suffix"], env["SHLIBSUFFIX"]),
    source=sources,
)

Default(library)