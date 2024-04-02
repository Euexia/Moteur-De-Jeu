#include "lve_image.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace lve {

	LveImage::LveImage(LveDevice& _lveDevice) : lveDevice(_lveDevice) {
		createTextureImage();
	}

	LveImage::~LveImage() {}

	void LveImage::createTextureImage() {
		int texWidth, texHeight, texChannels;
		stbi_uc* pixels = stbi_load("textures/textures.jpg", &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
		VkDeviceSize imageSize = texWidth * texHeight * 4;

		if (!pixels) {
			throw std::runtime_error("failed to load texture image");
		}

		vk::Buffer stagingBuffer;
		vk::DeviceMemory stagingBufferMemory;

		lveDevice.createBuffer(imageSize, vk::BufferUsageFlagBits::eTransferSrc, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent, 
			stagingBuffer, stagingBufferMemory);

		void* data;
		lveDevice.device().mapMemory(stagingBufferMemory, 0, imageSize, vk::MemoryMapFlags(), &data);
		memcpy(data, pixels, static_cast<size_t>(imageSize));
		lveDevice.device().unmapMemory(stagingBufferMemory);
		stbi_image_free(pixels);
			
		createImage(texWidth, texHeight, vk::Format::eR8G8B8A8Srgb, vk::ImageTiling::eOptimal, vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled
			, vk::MemoryPropertyFlagBits::eDeviceLocal, textureImage, textureImageMemory);

		transitionImageLayout(textureImage, vk::Format::eR8G8B8A8Srgb, vk::ImageLayout::eUndefined, vk::ImageLayout::eTransferDstOptimal);
		lveDevice.copyBufferToImage(stagingBuffer, textureImage, static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight),1);
		transitionImageLayout(textureImage, vk::Format::eR8G8B8A8Srgb, vk::ImageLayout::eTransferDstOptimal, vk::ImageLayout::eShaderReadOnlyOptimal);
	
		lveDevice.device().destroyBuffer(stagingBuffer);
		lveDevice.device().freeMemory(stagingBufferMemory, nullptr);
	}

	void LveImage::createImage(uint32_t width, uint32_t height, vk::Format format, vk::ImageTiling tiling, vk::ImageUsageFlags usage, 
		vk::MemoryPropertyFlags properties, vk::Image& image, vk::DeviceMemory& imageMemory) {

		vk::ImageCreateInfo imageInfo{};
		imageInfo.sType = vk::StructureType::eImageCreateInfo;
		imageInfo.imageType = vk::ImageType::e2D;
		imageInfo.extent.width = width;
		imageInfo.extent.height = height;
		imageInfo.extent.depth = 1;
		imageInfo.mipLevels = 1;
		imageInfo.arrayLayers = 1;
		imageInfo.format = vk::Format::eR8G8B8A8Srgb;
		//VK_IMAGE_TILING_LINEAR: Texels are laid out in row-major order like our pixels array
		//VK_IMAGE_TILING_OPTIMAL: Texels are laid out in an implementation defined order for optimal access
		imageInfo.tiling = vk::ImageTiling::eOptimal;
		imageInfo.initialLayout = vk::ImageLayout::eUndefined;
		imageInfo.usage = vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled;
		imageInfo.sharingMode = vk::SharingMode::eExclusive;
		imageInfo.samples = vk::SampleCountFlagBits::e1;
		//imageInfo.flags = 0; // Optional

		if (lveDevice.device().createImage(&imageInfo, nullptr, &image) != vk::Result::eSuccess) {
			throw std::runtime_error("failed to create image!");
		}

		vk::MemoryRequirements memRequirements;
		lveDevice.device().getImageMemoryRequirements(image, &memRequirements);

		vk::MemoryAllocateInfo allocInfo{};
		allocInfo.sType = vk::StructureType::eMemoryAllocateInfo;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = lveDevice.findMemoryType(memRequirements.memoryTypeBits, properties);

		if (lveDevice.device().allocateMemory(&allocInfo, nullptr, &imageMemory) != vk::Result::eSuccess) {
			throw std::runtime_error("failed to allocate image memory!");
		}

		lveDevice.device().bindImageMemory(image, imageMemory, 0);

	}
	void LveImage::transitionImageLayout(vk::Image image, vk::Format, vk::ImageLayout oldLayout, vk::ImageLayout newLayout) {
		vk::CommandBuffer commandBuffer = lveDevice.beginSingleTimeCommands();
		
		lveDevice.endSingleTimeCommands(commandBuffer);
	}

}