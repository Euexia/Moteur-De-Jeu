#include "Systems/simple_render_system.h"

//libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm.hpp>

// std
#include <cassert>
#include <stdexcept>

namespace lve
{
	SimpleRenderSystem::SimpleRenderSystem(LveDevice&              _device, vk::RenderPass _renderPass,
	                                       vk::DescriptorSetLayout _globalSetLayout) : lveDevice(_device)
	{
		CreatePipelineLayout(_globalSetLayout);
		CreatePipeline(_renderPass);
	}

	SimpleRenderSystem::~SimpleRenderSystem() { lveDevice.Device().destroyPipelineLayout(pipelineLayout, nullptr); }

	void SimpleRenderSystem::CreatePipelineLayout(vk::DescriptorSetLayout _globalSetLayout)
	{
		vk::PushConstantRange pushConstantRange{};
		pushConstantRange.stageFlags = vk::ShaderStageFlagBits::eVertex | vk::ShaderStageFlagBits::eFragment;
		pushConstantRange.offset     = 0;
		pushConstantRange.size       = sizeof(SimplePushConstantData);

		std::vector<vk::DescriptorSetLayout> descriptorSetLayouts{_globalSetLayout};

		vk::PipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType                  = vk::StructureType::ePipelineLayoutCreateInfo;
		pipelineLayoutInfo.setLayoutCount         = static_cast<uint32_t>(descriptorSetLayouts.size());
		pipelineLayoutInfo.pSetLayouts            = descriptorSetLayouts.data();
		pipelineLayoutInfo.pushConstantRangeCount = 1;
		pipelineLayoutInfo.pPushConstantRanges    = &pushConstantRange;
		if (lveDevice.Device().createPipelineLayout(&pipelineLayoutInfo, nullptr, &pipelineLayout) !=
		    vk::Result::eSuccess)
			throw std::runtime_error("failed to create pipeline layout!");
	}


	void SimpleRenderSystem::CreatePipeline(vk::RenderPass _renderPass)
	{
		assert(pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");

		PipelineConfigInfo pipelineConfig{};
		LvePipeline::DefaultPipelineConfigInfo(pipelineConfig);
		pipelineConfig.renderPass     = _renderPass;
		pipelineConfig.pipelineLayout = pipelineLayout;
		lvePipeline                   = std::make_unique<LvePipeline>(
			lveDevice,
			"Shaders/simple_shader.vert.spv",
			"Shaders/simple_shader.frag.spv",
			pipelineConfig);
	}

	void SimpleRenderSystem::RenderGameObjects(FrameInfo& _frameInfo)
	{
		// Liaison du pipeline
		lvePipeline->Bind(_frameInfo.commandBuffer);

        //// Liaison de l'ensemble de descripteurs global
        //_frameInfo.commandBuffer.bindDescriptorSets(
        //    vk::PipelineBindPoint::eGraphics,
        //    pipelineLayout,
        //    0,
        //    _frameInfo.globalDescriptorSet,
        //    nullptr);

        for (auto& kv : _frameInfo.gameObjects) {
            auto& obj = kv.second;
            if (obj.model == nullptr || obj.texture == nullptr) continue;

            ///////////
            // Create the image info for the texture
            vk::DescriptorImageInfo imageInfo{};
            imageInfo.sampler = obj.texture->getSampler();
            imageInfo.imageView = obj.texture->getImageView();
            imageInfo.imageLayout = obj.texture->getImageLayout();

            _frameInfo.commandBuffer.bindDescriptorSets(
                vk::PipelineBindPoint::eGraphics,
                pipelineLayout,
                0,
                _frameInfo.globalDescriptorSet,
                nullptr);

            // Update the descriptor set with the image info for the current texture
            vk::WriteDescriptorSet descriptorWrite{
                _frameInfo.globalDescriptorSet, // Use the global descriptor set for the frame
                1, // Assuming your texture descriptor set has binding 1
                0, // Assuming your texture descriptor set has array element 0
                1, // Number of descriptors to update
                vk::DescriptorType::eCombinedImageSampler, // Type of descriptor
                &imageInfo, // Pointer to array of image infos
                nullptr, // Optional buffer info
                nullptr // Optional texel buffer view info
            };
            // Update the descriptor set
            lveDevice.device().updateDescriptorSets(1, &descriptorWrite, 0, nullptr);
            ////////////


            SimplePushConstantData push{};
            push.modelMatrix = obj.transform.mat4();
            push.normalMatrix = obj.transform.normalMatrix();

			// Mise � jour des push constants
			_frameInfo.commandBuffer.pushConstants<SimplePushConstantData>(
				pipelineLayout,
				vk::ShaderStageFlagBits::eVertex | vk::ShaderStageFlagBits::eFragment,
				0,
				push);

			// Liaison du mod�le et dessin
			obj.model->Bind(_frameInfo.commandBuffer);
			obj.model->Draw(_frameInfo.commandBuffer);
		}
	}
} // namespace lve
