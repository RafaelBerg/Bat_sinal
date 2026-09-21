# The following five lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.22)

# OneDrive path "Área de Trabalho" is UTF-8; kconfgen otherwise opens
# build/config.env as Windows cp1252 and crashes (byte 0x81).
set(ENV{PYTHONUTF8} "1")
set(ENV{PYTHONIOENCODING} "utf-8")
# ccache also chokes on that path (illegal byte sequence).
set(ENV{IDF_CCACHE_ENABLE} "0")

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
# "Trim" the build. Include the minimal set of components, main, and anything it depends on.
idf_build_set_property(MINIMAL_BUILD ON)
project(bat_sinal)
