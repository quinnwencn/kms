from conan import ConanFile
from conan.tools.cmake import CMake, CMakeToolchain, CMakeDeps, cmake_layout


class Kms(ConanFile):
    name = "kms"
    version = "0.1.0"
    description = "KMS is a simple C++ project for managing key-value stores."
    settings = "os", "compiler", "build_type", "arch"

    default_options = {
        "openssl/*:shared": True,
        "boost/*:shared": True,
        "gtest/*:shared": True,
        "tomlplusplus/*:shared": True,
        "fmt/*:shared": True
    }

    def requirements(self):
        self.requires("openssl/3.0.8")
        self.requires("boost/1.81.0", transitive_headers=True, transitive_libs=True)
        self.requires("gtest/1.13.0")
        self.requires("tomlplusplus/3.4.0")
        self.requires("fmt/12.1.0")
        self.requires("cli11/2.6.0")

    def build_requirements(self):
        self.build_requires("cmake/3.26.4")

    def layout(self):
        cmake_layout(self)

    def generate(self):
        tc = CMakeToolchain(self)
        tc.variables["CMAKE_EXPORT_COMPILE_COMMANDS"] = True
        openssl = self.dependencies.get("openssl")

        # Inject OpenSSL's include path into global CXX flags,
        # to fix boost can not find openssl/conf.h error
        if openssl:
            openssl_include = openssl.cpp_info.includedirs[0]
            tc.cache_variables["CMAKE_CXX_FLAGS"] = f"-I{openssl_include}"
        tc.generate()

        deps = CMakeDeps(self)
        deps.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
