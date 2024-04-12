#pragma once

#include <vulkan/vulkan.hpp>

#define VERTEX_BUFFER_BIND_ID 0
#define ENABLE_VALIDATION  false

class Cubemap {
public:
    struct CubemapTexture {
        vk::Sampler sampler;
        vk::Image image;
        vk::ImageLayout imageLayour;
        vk::DeviceMemory deviceMemory;
        vk::ImageView view;
        uint32_t width, height;
        uint32_t mipLevels;
        uint32_t layerCount;
        vk::DescriptorImageInfo descriptor;
    };

    CubemapTexture cubeMap;

private:
    vk::DescriptorSet skybox;

	vk::PipelineLayout pipelineLayout;
	vk::DescriptorSetLayout descriptorSetLayout;

    vk::Device device;

    void createImage(uint32_t width, uint32_t height, uint32_t mipLevels, vk::Format format, vk::ImageUsageFlags usage,
        vk::MemoryPropertyFlags properties, vk::Image& image, vk::DeviceMemory& imageMemory);
    void transitionImageLayout(vk::Image image, vk::Format format, vk::ImageLayout oldLayout, vk::ImageLayout newLayout,
        uint32_t mipLevels);
    void copyBufferToImage(vk::Buffer buffer, vk::Image image, uint32_t width, uint32_t height);
    void generateMipmaps(vk::Image image, vk::Format imageFormat, int32_t texWidth, int32_t texHeight, uint32_t mipLevels);
};