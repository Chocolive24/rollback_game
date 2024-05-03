cmake_minimum_required(VERSION 3.22)
set(CMAKE_CXX_STANDARD 17)

project(rollback_game)

set(PHOTON_APP_ID "your_app_id_here" CACHE STRING "Set the photon app id")
add_definitions("-DPHOTON_APP_ID=\"${PHOTON_APP_ID}\"")

# Add a CMake option to enable or disable Tracy Profiler
option(USE_DEBUG "Use Debug Mode" OFF)

# Check if the build type is Debug or RelWithDebInfo
if(USE_DEBUG)
    add_compile_definitions(DEBUG)
endif()

if (EMSCRIPTEN)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Os -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -s ASYNCIFY -s ALLOW_MEMORY_GROWTH=1 --no-heap-copy")
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
endif ()

find_package(raylib REQUIRED)
find_package(ImGui CONFIG REQUIRED)
find_package(GTest CONFIG REQUIRED)

# Add a CMake option to enable or disable Tracy Profiler
option(USE_TRACY "Use Tracy Profiler" OFF)

if (USE_TRACY)
    # Enable Tracy profiling by setting the preprocessor directive
    add_compile_definitions(TRACY_ENABLE)
    # Include Tracy's headers for the entire project
    include_directories(externals/tracy_profiler/tracy)

    # Add the Tracy source code as part of the project
    add_library(tracyClient STATIC externals/tracy_profiler/TracyClient.cpp)
endif()

if (NOT EMSCRIPTEN)
    # Create the photon library.
    file(GLOB_RECURSE PHOTON_SRC_FILES externals/photon/LoadBalancing-cpp/inc/*.h externals/photon/LoadBalancing-cpp/src/*.cpp)
    add_library(photon ${PHOTON_SRC_FILES})
    set_target_properties(photon PROPERTIES LINKER_LANGUAGE CXX)
    target_include_directories(photon PUBLIC externals/photon)
    message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")

    if(NOT USE_DEBUG)
        target_link_libraries(photon PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}/externals/photon/Common-cpp/lib/Common-cpp_vc17_release_windows_md_x64.lib
            ${CMAKE_CURRENT_SOURCE_DIR}/externals/photon/Photon-cpp/lib/Photon-cpp_vc17_release_windows_md_x64.lib
        )
    else()
        target_link_libraries(photon PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}/externals/photon/Common-cpp/lib/Common-cpp_vc17_debug_windows_md_x64.lib
            ${CMAKE_CURRENT_SOURCE_DIR}/externals/photon/Photon-cpp/lib/Photon-cpp_vc17_debug_windows_md_x64.lib
        )
    endif()
   
endif()

set(data_dir "${CMAKE_SOURCE_DIR}/data")

file(GLOB_RECURSE DATA_FILES
            "data/*.json"
            "data/*.png"
            "data/*.jpg"
            "data/*.wav"
            )
foreach(DATA ${DATA_FILES})
    get_filename_component(FILE_NAME ${DATA} NAME)
    get_filename_component(PATH_NAME ${DATA} DIRECTORY)
    get_filename_component(EXTENSION ${DATA} EXT)

    file(RELATIVE_PATH PATH_NAME "${CMAKE_CURRENT_SOURCE_DIR}" ${PATH_NAME})
    set(DATA_OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/${PATH_NAME}/${FILE_NAME}")

    add_custom_command(
                OUTPUT ${DATA_OUTPUT}
                COMMAND ${CMAKE_COMMAND} -E copy
                ${DATA}
                ${DATA_OUTPUT}
                DEPENDS ${DATA})

    list(APPEND Data_OUTPUT_FILES  ${DATA_OUTPUT})
endforeach(DATA)

add_custom_target(data_target DEPENDS ${Data_OUTPUT_FILES})

# Create the rlImGui library.
file(GLOB_RECURSE RLIMGUI_FILES libs/rlImGui/include/*.h libs/rlImGui/src/*.cpp)
add_library(rl_imgui ${RLIMGUI_FILES})
set_target_properties(rl_imgui PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(rl_imgui PUBLIC libs/rlImGui/include/)
target_link_libraries(rl_imgui PRIVATE raylib)
add_compile_definitions(NO_FONT_AWESOME)

# Create the Math library
file(GLOB_RECURSE MATH_SRC_FILES libs/math/include/*.h libs/math/src/*.cpp)
add_library(math ${MATH_SRC_FILES})
set_target_properties(math PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(math PUBLIC libs/math/include/)

# Create the Common library with Math as a dependency
file(GLOB_RECURSE COMMON_SRC_FILES common/include/*.h common/src/*.cpp)
add_library(common ${COMMON_SRC_FILES})
set_target_properties(common PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(common PUBLIC common/include/)
target_link_libraries(common PRIVATE math)

# Create the physics library with math and common as dependencies.
file(GLOB_RECURSE PHYSICS_SRC_FILES physics_engine/include/*.h physics_engine/src/*.cpp)
add_library(physics ${PHYSICS_SRC_FILES})
set_target_properties(physics PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(physics PUBLIC physics_engine/include/)
target_include_directories(math PUBLIC libs/math/include/)
target_link_libraries(physics PRIVATE math common)
if (USE_TRACY)
    target_compile_definitions(physics PUBLIC TRACY_ENABLE)
    # Link the TracyClient library
    target_link_libraries(physics PRIVATE tracyClient)
endif()

# Create the core library.
file(GLOB_RECURSE CORE_SRC_FILES core/include/*.h core/src/*.cpp)
add_library(core ${CORE_SRC_FILES})
set_target_properties(core PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(core PUBLIC core/include/)
target_link_libraries(core PUBLIC raylib imgui::imgui rl_imgui math common)

## Create the Common library with Math as a dependency
#file(GLOB_RECURSE NETWORK_SRC_FILES network/include/*.h network/src/*.cpp)
#add_library(network ${NETWORK_SRC_FILES})
#set_target_properties(network PROPERTIES LINKER_LANGUAGE CXX)
#target_include_directories(network PUBLIC network/include/)
#target_link_libraries(network PUBLIC photon)

# Create the game library.
file(GLOB_RECURSE GAME_SRC_FILES game/include/*.h game/src/*.cpp)
add_library(game ${GAME_SRC_FILES})
set_target_properties(game PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(game PUBLIC game/include/)
target_link_libraries(game PUBLIC math common physics core)
if (NOT EMSCRIPTEN)
    target_link_libraries(game PUBLIC photon)
endif()
add_dependencies(game data_target)

add_executable(client_app main/client_entry_point.cpp)
target_link_libraries(client_app PRIVATE game)
if (EMSCRIPTEN)
        # The local resources path needs to be mapped to /data virtual path
        string(APPEND data_dir "@data")
        set_target_properties(client_app PROPERTIES LINK_FLAGS "--preload-file ${data_dir}")
endif ()

if (NOT EMSCRIPTEN)
    add_executable(photon_sample main/photon_sample.cpp)
    target_link_libraries(photon_sample PRIVATE game)

    add_executable(simulation_app main/simulation_app_entry_point.cpp)
    target_link_libraries(simulation_app PRIVATE game)

    add_executable(split_screen_app main/split_screen_app_entry_point.cpp)
    target_link_libraries(split_screen_app PRIVATE game)
endif()

# Copy all of the resource files to the destination
#file(COPY ${data_files} DESTINATION "data/")

# Tests.
#if (NOT EMSCRIPTEN)
#    file(GLOB_RECURSE GAME_TEST_FILES game/tests/*.cpp)
#    foreach(test_file ${GAME_TEST_FILES} )
#        get_filename_component(test_name ${test_file} NAME_WE)
#
#        add_executable(${test_name} ${test_file})
#
#        target_link_libraries(${test_name} PRIVATE common core game)
#        target_link_libraries(${test_name} PRIVATE GTest::gtest GTest::gtest_main)
#    endforeach()
#endif()