cmake_minimum_required(VERSION 3.22)
project(sdl3_cimgui)

# Set output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Set C standard (since you're using .c files)
# set(CMAKE_C_STANDARD 11)
# set(CMAKE_C_STANDARD 99)
# set(CMAKE_C_STANDARD_REQUIRED ON)

# Find Vulkan
# find_package(Vulkan REQUIRED)

# Include FetchContent for downloading dependencies
include(FetchContent)

# Fetch raylib
FetchContent_Declare(
    SDL3
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG release-3.2.22
    GIT_SHALLOW TRUE
    USES_TERMINAL_DOWNLOAD TRUE
)
FetchContent_MakeAvailable(sdl3)

# Find Vulkan
# find_package(Vulkan REQUIRED)

# Provides the Vulkan::Headers target, which sets up include directories for <vulkan/vulkan.h> and other headers.
FetchContent_Declare(
    vulkanheaders
    GIT_REPOSITORY https://github.com/KhronosGroup/Vulkan-Headers.git
    GIT_TAG vulkan-sdk-1.4.313.0
    GIT_SHALLOW TRUE
    USES_TERMINAL_DOWNLOAD TRUE
)
FetchContent_MakeAvailable(vulkanheaders)

# Fetch Vulkan-Loader
# Builds the Vulkan loader library, providing the vulkan target (equivalent to vulkan-1.lib on Windows).
# The vulkan target includes the runtime implementations of Vulkan API functions, resolving the linker errors (e.g., vkCreateInstance, vkCmdBeginRenderPass).
FetchContent_Declare(
    VulkanLoader
    GIT_REPOSITORY https://github.com/KhronosGroup/Vulkan-Loader.git
    GIT_TAG vulkan-sdk-1.4.313.0 # Match the Vulkan-Headers version
)
FetchContent_MakeAvailable(VulkanLoader)

# Fetch imgui
FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG master 
    EXCLUDE_FROM_ALL #this disable build?
    GIT_SHALLOW TRUE
    USES_TERMINAL_DOWNLOAD TRUE
)
FetchContent_MakeAvailable(imgui)
message(STATUS "imgui_SOURCE_DIR: >> ${imgui_SOURCE_DIR}")

#================================================
# Lua
#================================================
FetchContent_Declare(
    lua
    GIT_REPOSITORY https://github.com/lua/lua.git
    GIT_TAG v5.4.8
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(lua)

# Lua doesn't have a CMake build, so we manually compile it
# List Lua source files (core and libraries, excluding lua.c and luac.c)
set(LUA_SRC
    ${lua_SOURCE_DIR}/lapi.c
    ${lua_SOURCE_DIR}/lauxlib.c
    ${lua_SOURCE_DIR}/lbaselib.c
    ${lua_SOURCE_DIR}/lcode.c
    ${lua_SOURCE_DIR}/lcorolib.c
    ${lua_SOURCE_DIR}/lctype.c
    ${lua_SOURCE_DIR}/ldblib.c
    ${lua_SOURCE_DIR}/ldebug.c
    ${lua_SOURCE_DIR}/ldo.c
    ${lua_SOURCE_DIR}/ldump.c
    ${lua_SOURCE_DIR}/lfunc.c
    ${lua_SOURCE_DIR}/lgc.c
    ${lua_SOURCE_DIR}/linit.c
    ${lua_SOURCE_DIR}/liolib.c
    ${lua_SOURCE_DIR}/llex.c
    ${lua_SOURCE_DIR}/lmathlib.c
    ${lua_SOURCE_DIR}/lmem.c
    ${lua_SOURCE_DIR}/loadlib.c
    ${lua_SOURCE_DIR}/lobject.c
    ${lua_SOURCE_DIR}/lopcodes.c
    ${lua_SOURCE_DIR}/loslib.c
    ${lua_SOURCE_DIR}/lparser.c
    ${lua_SOURCE_DIR}/lstate.c
    ${lua_SOURCE_DIR}/lstring.c
    ${lua_SOURCE_DIR}/lstrlib.c
    ${lua_SOURCE_DIR}/ltable.c
    ${lua_SOURCE_DIR}/ltablib.c
    ${lua_SOURCE_DIR}/ltm.c
    ${lua_SOURCE_DIR}/lundump.c
    ${lua_SOURCE_DIR}/lvm.c
    ${lua_SOURCE_DIR}/lzio.c
    ${lua_SOURCE_DIR}/lutf8lib.c
)

# Create a static Lua library
add_library(lua STATIC ${LUA_SRC})
target_include_directories(lua PUBLIC ${lua_SOURCE_DIR})

#================================================
# IMAGE
#================================================
FetchContent_Declare(
    stb
    GIT_REPOSITORY https://github.com/nothings/stb.git
    GIT_TAG        master
    EXCLUDE_FROM_ALL #this disable build?
    GIT_SHALLOW TRUE
    USES_TERMINAL_DOWNLOAD TRUE
)
FetchContent_MakeAvailable(stb)

#================================================
# APP
#================================================

# Application NAME
set(APP_NAME sdl3_imgui)

# Source files (add more if needed)
set(SRC_FILES
    # Add other .c files here if necessary
    src/module_imgui_lua.cpp
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl3.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_vulkan.cpp
)

# Create executable
add_executable(${APP_NAME}
    src/main.cpp
    # src/imgui_image.cpp
    ${SRC_FILES}
)

# Lua
target_link_libraries(${APP_NAME} PRIVATE lua) # lua 5.4.7
# SDL3 
target_link_libraries(${APP_NAME} PUBLIC SDL3::SDL3) # SDL 3.2.22

target_link_libraries(${APP_NAME} PRIVATE # vulkan-headers
    Vulkan::Headers # Vulkan headers
    vulkan # Vulkan loader library (from Vulkan-Loader)
)

# Include directories
target_include_directories(${APP_NAME} PUBLIC
    ${CMAKE_SOURCE_DIR}/include             # root project
    ${imgui_SOURCE_DIR}                     # imgui
    ${imgui_SOURCE_DIR}/backends            # imgui/backends
    ${SDL3_SOURCE_DIR}/include              # SDL 3.2.22
    ${lua_SOURCE_DIR}                       # lua
    ${stb_SOURCE_DIR}                       # stb
)

# for c #define
target_compile_definitions(
	${APP_NAME}
	PUBLIC
    # IMGUI_DISABLE_OBSOLETE_FUNCTIONS=1
	# CIMGUI_DEFINE_ENUMS_AND_STRUCTS=1
)

# Platform-specific settings for Windows (MinGW/MSYS2)
if (WIN32)
    # Link necessary Windows libraries for raylib
    target_link_libraries(${APP_NAME} PRIVATE
        opengl32
        gdi32
        winmm
    )

    # Static linking for libgcc and libstdc++
    target_link_options(${APP_NAME} PRIVATE
        -static-libgcc
        -static-libstdc++  # Uncomment if C++ code is used
        -static           # Avoid full static linking to prevent issues with system libraries
    )
endif()

