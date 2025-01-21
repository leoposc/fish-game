from conan import ConanFile
from conan.tools.cmake import cmake_layout


class CompressorRecipe(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeToolchain", "CMakeDeps"

    def requirements(self):
        self.requires("sdl_image/2.8.2")
        self.requires("sdl_ttf/2.22.0")
        self.requires("sdl_mixer/2.8.0")
        self.requires("spdlog/1.14.1")
        self.requires("sdl/2.28.5", override=True)

    def build_requirements(self):
        self.tool_requires("cmake/3.22.6")

    def layout(self):
        cmake_layout(self)
