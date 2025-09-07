// include/textures.h
#ifndef TEXTURES_H
#define TEXTURES_H

#include <vulkan/vulkan.h>  // For VkImage, VkDeviceMemory, etc.

struct TextureData {
    VkImage image = VK_NULL_HANDLE;
    VkDeviceMemory memory = VK_NULL_HANDLE;
    VkImageView imageView = VK_NULL_HANDLE;
    VkSampler sampler = VK_NULL_HANDLE;
    VkDescriptorSet descriptorSet = VK_NULL_HANDLE;
};

#endif