#include "Managers/HUDManager.h"
#include <iostream>
#include <stdexcept>
#include <fstream>
#include <algorithm>
#include <stb_image.h>


// Fonction utilitaire pour charger une image depuis un fichier sur le disque
std::vector<char> readFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::ate | std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + filename);
    }

    size_t fileSize = static_cast<size_t>(file.tellg());
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);

    file.close();

    return buffer;
}

// Méthode pour créer une image Vulkan à partir d'un fichier image sur le disque
vk::Image HUDManager::createTextureImage(const std::string& texturePath) {
    int texWidth, texHeight, texChannels;
    stbi_uc* pixels = stbi_load(texturePath.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
    if (!pixels) {
        throw std::runtime_error("Failed to load texture image: " + texturePath);
    }

    vk::DeviceSize imageSize = texWidth * texHeight * 4;

    // Création du tampon d'étape (staging buffer)
    vk::Buffer stagingBuffer;
    vk::DeviceMemory stagingBufferMemory;
    createBuffer(imageSize, vk::BufferUsageFlagBits::eTransferSrc, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent, stagingBuffer, stagingBufferMemory);

    // Copie des données des pixels dans le tampon d'étape
    void* data;
    m_device.mapMemory(stagingBufferMemory, 0, imageSize, {}, &data);
    memcpy(data, pixels, static_cast<size_t>(imageSize));
    m_device.unmapMemory(stagingBufferMemory);

    // Libération de la mémoire des pixels chargés
    stbi_image_free(pixels);

    // Création de l'image de texture
    vk::Image textureImage;
    vk::DeviceMemory textureImageMemory;
    createImage(imageSize, vk::Format::eR8G8B8A8Unorm, vk::ImageTiling::eOptimal, vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled, vk::MemoryPropertyFlagBits::eDeviceLocal, textureImage, textureImageMemory);

    // Transition de l'image de texture à un format optimal pour les transferts
    transitionImageLayout(textureImage, vk::Format::eR8G8B8A8Unorm, vk::ImageLayout::eUndefined, vk::ImageLayout::eTransferDstOptimal);

    // Copie des données du tampon d'étape dans l'image de texture
    copyBufferToImage(stagingBuffer, textureImage, texWidth, texHeight);

    // Transition de l'image de texture à un format optimal pour être utilisée comme texture
    transitionImageLayout(textureImage, vk::Format::eR8G8B8A8Unorm, vk::ImageLayout::eTransferDstOptimal, vk::ImageLayout::eShaderReadOnlyOptimal);

    // Nettoyage des ressources du tampon d'étape
    m_device.destroyBuffer(stagingBuffer);
    m_device.freeMemory(stagingBufferMemory);

    return textureImage;
}

// Méthode pour créer une vue d'image Vulkan à partir d'une image Vulkan
vk::ImageView HUDManager::createTextureImageView(vk::Image image) {
    return createImageView(image, vk::Format::eR8G8B8A8Unorm, vk::ImageAspectFlagBits::eColor);
}

// Méthode pour créer un échantillonneur Vulkan pour les textures
vk::Sampler HUDManager::createTextureSampler() {
    vk::SamplerCreateInfo samplerInfo{};
    samplerInfo.sType = vk::StructureType::eSamplerCreateInfo;
    samplerInfo.magFilter = vk::Filter::eLinear;
    samplerInfo.minFilter = vk::Filter::eLinear;
    samplerInfo.addressModeU = vk::SamplerAddressMode::eRepeat;
    samplerInfo.addressModeV = vk::SamplerAddressMode::eRepeat;
    samplerInfo.addressModeW = vk::SamplerAddressMode::eRepeat;
    samplerInfo.anisotropyEnable = vk::False;
    samplerInfo.maxAnisotropy = 16;
    samplerInfo.borderColor = vk::BorderColor::eIntOpaqueBlack;
    samplerInfo.unnormalizedCoordinates = vk::False;
    samplerInfo.compareEnable = vk::False;
    samplerInfo.compareOp = vk::CompareOp::eAlways;
    samplerInfo.mipmapMode = vk::SamplerMipmapMode::eLinear;
    samplerInfo.mipLodBias = 0.0f;
    samplerInfo.minLod = 0.0f;
    samplerInfo.maxLod = 0.0f;

    vk::Sampler sampler;
    if (m_device.createSampler(&samplerInfo, nullptr, &sampler) != vk::Result::eSuccess) {
        throw std::runtime_error("Failed to create texture sampler");
    }

    return sampler;
}

// Méthode pour ajouter un composant HUD
void HUDManager::addComponent(float x, float y, float width, float height, const std::string& texturePath) {
    HUDComponent component{};
    component.x = x;
    component.y = y;
    component.width = width;
    component.height = height;
    //component.texture = createTextureImage(texturePath);
    component.imageView = createTextureImageView(component.texture);
    component.sampler = createTextureSampler();

    m_components.push_back(component);
}

// Méthode pour nettoyer les composants HUD
void HUDManager::cleanupComponents() {
    for (auto& component : m_components) {
        m_device.destroySampler(component.sampler, nullptr);
        m_device.destroyImageView(component.imageView, nullptr);
        m_device.destroyImage(component.texture, nullptr);
    }
    m_components.clear();
}

// Méthode pour dessiner les composants HUD
void HUDManager::render(vk::CommandBuffer commandBuffer) {
    // Code pour dessiner les composants HUD avec Vulkan
    // Utiliser les commandes Vulkan pour dessiner les textures des composants sur l'écran
}

void HUDManager::transitionImageLayout(vk::Image image, vk::Format format, vk::ImageLayout oldLayout, vk::ImageLayout newLayout) {

}

void HUDManager::createBuffer(vk::DeviceSize size, vk::BufferUsageFlags usage, vk::MemoryPropertyFlags properties, vk::Buffer& buffer, vk::DeviceMemory& bufferMemory) {
    // Implémentation de la création du tampon Vulkan
}
void HUDManager::createImage(vk::DeviceSize size, vk::Format format, vk::ImageTiling tiling, vk::ImageUsageFlags usage, vk::MemoryPropertyFlags properties, vk::Image& image, vk::DeviceMemory& imageMemory) {
    // Implémentation de la création de l'image Vulkan
}

void HUDManager::copyBufferToImage(vk::Buffer buffer, vk::Image image, uint32_t width, uint32_t height) {
    // Implémentation de la copie du tampon vers l'image Vulkan
}

vk::ImageView HUDManager::createImageView(vk::Image image, vk::Format format, vk::ImageAspectFlags aspectFlags) {
    // Implémentation de la création de la vue d'image Vulkan
    return vk::ImageView();
}