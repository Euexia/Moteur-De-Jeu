#pragma once
#ifndef HUD_MANAGER_H
#define HUD_MANAGER_H

#include <vulkan/vulkan.hpp>
#include <vector>
#include <string>

struct HUDComponent {
    float x, y; // Position de l'élément HUD
    float width, height; // Largeur et hauteur de l'élément HUD
    vk::Image texture; // Image Vulkan de l'élément HUD
    vk::ImageView imageView; // Vue de l'image Vulkan
    vk::Sampler sampler; // Échantillonneur Vulkan
};

class HUDManager {
public:
    HUDManager(vk::Device device, vk::RenderPass renderPass, vk::Queue graphicsQueue, vk::CommandPool commandPool);
    ~HUDManager();

    void addComponent(float x, float y, float width, float height, const std::string& texturePath);
    void removeComponent(int index);
    void cleanupComponents();
    void render(vk::CommandBuffer commandBuffer);

private:
    vk::Device m_device;
    vk::RenderPass m_renderPass;
    vk::Queue m_graphicsQueue;
    vk::CommandPool m_commandPool;
    std::vector<HUDComponent> m_components;

    // Méthodes pour créer et gérer les ressources Vulkan
    vk::Image createTextureImage(const std::string& texturePath);
    vk::ImageView createTextureImageView(vk::Image image);
    vk::Sampler createTextureSampler();
    void transitionImageLayout(vk::Image image, vk::Format format, vk::ImageLayout oldLayout, vk::ImageLayout newLayout);

    void createBuffer(vk::DeviceSize size, vk::BufferUsageFlags usage, vk::MemoryPropertyFlags properties, vk::Buffer& buffer, vk::DeviceMemory& bufferMemory);
    void createImage(vk::DeviceSize size, vk::Format format, vk::ImageTiling tiling, vk::ImageUsageFlags usage, vk::MemoryPropertyFlags properties, vk::Image& image, vk::DeviceMemory& imageMemory);

    void copyBufferToImage(vk::Buffer buffer, vk::Image image, uint32_t width, uint32_t height);

    vk::ImageView createImageView(vk::Image image, vk::Format format, vk::ImageAspectFlags aspectFlags);
};

#endif // HUD_MANAGER_H
