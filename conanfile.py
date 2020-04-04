import os
from conans import ConanFile, CMake

class dds_loaderConan(ConanFile):
    name = "dds_loader"
    version = "0.0.1"
    _version_range = "[~=%s]" % ".".join(version.split(".")[0:2])
    settings = "os", "compiler", "build_type", "arch"
    scm = {"type": "git", "url": "auto", "revision": "auto" }
    generators = "cmake"
    no_copy_source = True
    
    def build(self):
        cmake = CMake(self)
        cmake.definitions["CMAKE_POSITION_INDEPENDENT_CODE"] = "ON"
        cmake.configure()
        cmake.build()
        cmake.install()