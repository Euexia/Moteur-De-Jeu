#include "lve_texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace lve {
	LveTexture::LveTexture(LveDevice& _device, const std::string& filepath) : lveDevice(_device) {
		int width;
		int height;
		int channels;
		int bytesPerPixel;

		// symbole externe non r�solu stbi_load
		stbi_uc* data = stbi_load(filepath.c_str(), &width, &height, &bytesPerPixel, 4);

		LveBuffer staginBuffer(lveDevice, 4,
			static_cast<uint32_t>(width, height),
			vk::BufferUsageFlagBits::eTransferSrc,
			vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent
		);

		staginBuffer.map();
		staginBuffer.writeToBuffer(data);

		imageFormat = vk::Format::eR8G8B8A8Srgb;

		vk::ImageCreateInfo imageInfo{};
		imageInfo.sType = vk::StructureType::eImageCreateInfo;
		imageInfo.imageType = vk::ImageType::e2D;
		imageInfo.format = imageFormat;
		imageInfo.mipLevels = 1;
		imageInfo.arrayLayers = 1;
		imageInfo.samples = vk::SampleCountFlagBits::e1;
		imageInfo.tiling = vk::ImageTiling::eOptimal;
		imageInfo.sharingMode = vk::SharingMode::eExclusive;
		imageInfo.initialLayout = vk::ImageLayout::eUndefined;
		imageInfo.setExtent({ static_cast<uint32_t>(width), static_cast<uint32_t>(height), 1 });
		imageInfo.usage = vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled;

		lveDevice.createImageWithInfo(imageInfo, vk::MemoryPropertyFlagBits::eDeviceLocal, image, imageMemory);

		transitionImageLayout(vk::ImageLayout::eTransferDstOptimal, vk::ImageLayout::eShaderReadOnlyOptimal);

		imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;

		vk::SamplerCreateInfo samplerInfo{};
		samplerInfo.sType = vk::StructureType::eSamplerCreateInfo;
		samplerInfo.magFilter = vk::Filter::eNearest;
		samplerInfo.minFilter = vk::Filter::eNearest;
		samplerInfo.mipmapMode = vk::SamplerMipmapMode::eLinear;
		samplerInfo.addressModeU = vk::SamplerAddressMode::eRepeat;
		samplerInfo.addressModeV = vk::SamplerAddressMode::eRepeat;
		samplerInfo.addressModeW = vk::SamplerAddressMode::eRepeat;
		samplerInfo.mipLodBias = 0.0f;
		samplerInfo.compareOp = vk::CompareOp::eNever;
		samplerInfo.minLod = 0.0f;
		samplerInfo.maxLod = 0.0f;
		samplerInfo.maxAnisotropy = 1.0f;
		samplerInfo.anisotropyEnable = vk::False;
		samplerInfo.borderColor = vk::BorderColor::eFloatOpaqueWhite;

		lveDevice.device().createSampler(&samplerInfo, nullptr, &sampler);

		vk::ImageViewCreateInfo imageViewInfo{};
		imageViewInfo.sType = vk::StructureType::eImageViewCreateInfo;
		imageViewInfo.viewType = vk::ImageViewType::e2D;
		imageViewInfo.format = imageFormat;
		imageViewInfo.components = (vk::ComponentSwizzle::eR, vk::ComponentSwizzle::eG, vk::ComponentSwizzle::eB, vk::ComponentSwizzle::eA );
		imageViewInfo.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
		imageViewInfo.subresourceRange.baseMipLevel = 0;
		imageViewInfo.subresourceRange.baseArrayLayer = 0;
		imageViewInfo.subresourceRange.layerCount = 1;
		imageViewInfo.subresourceRange.levelCount = 1;
		imageViewInfo.image = image;

		lveDevice.device().createImageView(&imageViewInfo, nullptr, &imageView);

		stbi_image_free(data); // symbole externe non r�solu stbi_image_free

	}

	LveTexture::~LveTexture() {
		lveDevice.device().destroyImage(image, nullptr);
		lveDevice.device().freeMemory(imageMemory, nullptr);
		lveDevice.device().destroyImageView(imageView, nullptr);
		lveDevice.device().destroySampler(sampler, nullptr);
	}

	void LveTexture::transitionImageLayout(vk::ImageLayout oldLayout, vk::ImageLayout newLayout) {
		vk::CommandBuffer commandBuffer = lveDevice.beginSingleTimeCommands();

		vk::ImageMemoryBarrier barrier{};
		barrier.sType = vk::StructureType::eImageMemoryBarrier;
		barrier.oldLayout = oldLayout;
		barrier.newLayout = newLayout;
		barrier.srcQueueFamilyIndex = vk::QueueFamilyIgnored;
		barrier.dstQueueFamilyIndex = vk::QueueFamilyIgnored;
		barrier.image = image;
		barrier.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
		barrier.subresourceRange.baseMipLevel = 0;
		barrier.subresourceRange.levelCount = 1;
		barrier.subresourceRange.baseArrayLayer = 0;
		barrier.subresourceRange.layerCount = 1;

		vk::PipelineStageFlags sourceStage;
		vk::PipelineStageFlags destinationStage;

		if (oldLayout == vk::ImageLayout::eUndefined && newLayout == vk::ImageLayout::eTransferDstOptimal) {
			barrier.srcAccessMask = vk::AccessFlagBits::eNone;
			barrier.dstAccessMask = vk::AccessFlagBits::eTransferWrite;

			sourceStage = vk::PipelineStageFlagBits::eTopOfPipe;
			destinationStage = vk::PipelineStageFlagBits::eTransfer;
		}

		else if(oldLayout == vk::ImageLayout::eTransferDstOptimal && newLayout == vk::ImageLayout::eShaderReadOnlyOptimal) {
			barrier.srcAccessMask = vk::AccessFlagBits::eTransferWrite;
			barrier.dstAccessMask = vk::AccessFlagBits::eShaderRead;

			sourceStage = vk::PipelineStageFlagBits::eTransfer;
			destinationStage = vk::PipelineStageFlagBits::eFragmentShader;
		}
		else {
			throw std::runtime_error("unsuppported layout transition!");
		}

		//vkCmdPipelineBarrier(commandBuffer, sourceStage, destinationStage, 0, 0, nullptr, 0, nullptr, 1, &barrier);
		//commandBuffer.CmdpipelineBarrier(sourceStage, destinationStage, 0, 0, nullptr, 0, nullptr, 1, &barrier);

		commandBuffer.pipelineBarrier(
			sourceStage,                      // Source pipeline stage
			destinationStage,                 // Destination pipeline stage
			vk::DependencyFlags(),            // Dependency flags
			nullptr,                          // Memory barriers
			nullptr,                          // Buffer memory barriers
			barrier                          // Image memory barriers
		);

		lveDevice.endSingleTimeCommands(commandBuffer);
	}



}