// module_imgui_lua.hpp
#ifndef MODULE_IMGUI_LUA_HPP
#define MODULE_IMGUI_LUA_HPP

#ifdef __cplusplus
extern "C" {
#endif
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
#ifdef __cplusplus
}
#endif

// Add these includes for STL types in externs
#include <map>
#include <string>

// Vulkan and Texture includes
#include <vulkan/vulkan.h>  // For VkDevice, VkCommandPool, etc.
#include "textures.h"  // For TextureData (full definition)

extern VkDevice g_Device;
extern VkPhysicalDevice g_PhysicalDevice;
extern VkCommandPool g_CommandPool;
extern VkQueue g_Queue;
extern VkAllocationCallbacks* g_Allocator;
extern std::map<std::string, TextureData> g_TextureMap;  // For bindings

// Expose functions for Lua bindings
extern TextureData CreateTexture(VkDevice device, VkPhysicalDevice physicalDevice, VkCommandPool commandPool, VkQueue queue, const char* imagePath);
extern void DestroyTexture(VkDevice device, TextureData& texture);

extern lua_State* L;  // Declare for external access

bool InitLua(const char* script_file = nullptr);
void RunLuaDraw();
void CleanupLua();

#endif