add_executable(unit_test main.cpp)
target_link_libraries(unit_test PUBLIC doctest nlohmann_json::nlohmann_json)
target_include_directories(unit_test PUBLIC ${CMAKE_SOURCE_DIR})

add_test( NAME unit_test COMMAND $<TARGET_FILE:unit_test> )