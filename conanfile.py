from conan import ConanFile
from conan.tools.cmake import CMake, CMakeToolchain, cmake_layout

class QuasarApexConan(ConanFile):
    name = "quasar-apex"
    version = "10.0.0"
    license = "Apache-2.0"
    url = "https://github.com/OWNER/REPO"
    description = "High-performance portable C++ text search toolkit"
    settings = "os", "compiler", "build_type", "arch"
    options = { "shared": [True, False] }
    default_options = { "shared": False }
    exports_sources = ("CMakeLists.txt", "include/*", "src/*", "apps/*", "cmake/*",
                       "man/*", "completions/*", "bench/*")

    def layout(self):
        cmake_layout(self)

    def generate(self):
        tc = CMakeToolchain(self)
        tc.generate()

    def build(self):
        cm = CMake(self)
        cm.configure()
        cm.build()

    def package(self):
        cm = CMake(self)
        cm.install()

    def package_info(self):
        self.cpp_info.libs = ["quasar"]
