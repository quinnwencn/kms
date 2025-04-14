from conan import ConanFile
from conan.tools.cmake import CMake, CMakeToolchain, CMakeDeps, cmake_layout


class Kms(ConanFile):
    name = "kms"
    version = "0.1.0"
    description = "KMS is a simple C++ project for managing key-value stores."
    settings = "os", "compiler", "build_type", "arch"
    generator = "CMakeDeps"

    default_options = {
        "boost/*:shared": True,
        "openssl/*:shared": True,
        "gtest/*:shared": True
    }

    def requirements(self):
        self.requires("boost/1.81.0")
        self.requires("openssl/3.0.8")
        self.requires("gtest/1.13.0")

    def build_requirements(self):
        self.build_requires("cmake/3.26.4")

    def layout(self):
        cmake_layout(self)

    def generate(self):
        tc = CMakeToolchain(self)
        tc.variables["CMAKE_EXPORT_COMPILE_COMMANDS"] = True
        tc.generate()

        deps = CMakeDeps(self)
        deps.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()