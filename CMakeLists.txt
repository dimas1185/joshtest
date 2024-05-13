cmake_minimum_required(VERSION 3.0.0)
project(joshtest VERSION 0.1.0 LANGUAGES C CXX)

set (CMAKE_CXX_STANDARD 20)

# =============================================libraries===
add_subdirectory(imports/cpp-httplib)

set(JSON_BuildTests OFF CACHE INTERNAL "")
add_subdirectory(imports/json)
# =========================================================

file(GLOB SOURCES *.cpp)

add_executable(joshtest ${SOURCES})

target_link_libraries(joshtest PUBLIC httplib::httplib nlohmann_json::nlohmann_json)

include_directories(imports/CmdParser)

# =============================================tests=======
include(CTest)
enable_testing()
add_subdirectory(imports/doctest)
add_subdirectory(tests)
# =========================================================