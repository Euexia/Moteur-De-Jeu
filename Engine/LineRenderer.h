#include "lve_device.h"
#include "Systems/simple_render_system.h"

class LineRenderer {
public:
    LineRenderer(lve::LveDevice& device, vk::RenderPass renderPass, vk::DescriptorSetLayout globalSetLayout)
        : lveDevice(device), renderPass(renderPass) {
        CreatePipelineLayout(globalSetLayout);
        CreatePipeline();
    }

    ~LineRenderer() {
        lveDevice.device().destroyPipelineLayout(pipelineLayout, nullptr);
    }

    void Render(vk::CommandBuffer commandBuffer) {
        commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, pipeline);

        // Dessinez vos lignes ici
    }

private:
    lve::LveDevice& lveDevice;
    vk::RenderPass renderPass;
    vk::PipelineLayout pipelineLayout;
    vk::Pipeline pipeline;

    void CreatePipelineLayout(vk::DescriptorSetLayout globalSetLayout) {
        vk::PushConstantRange pushConstantRange{};
        pushConstantRange.stageFlags = vk::ShaderStageFlagBits::eVertex | vk::ShaderStageFlagBits::eFragment;
        pushConstantRange.offset = 0;
        pushConstantRange.size = sizeof(lve::SimplePushConstantData);

        std::vector<vk::DescriptorSetLayout> descriptorSetLayouts{ globalSetLayout };

        vk::PipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(descriptorSetLayouts.size());
        pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();
        pipelineLayoutInfo.pushConstantRangeCount = 1;
        pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
        if (lveDevice.device().createPipelineLayout(&pipelineLayoutInfo, nullptr, &pipelineLayout) !=
            vk::Result::eSuccess) {
            throw std::runtime_error("failed to create pipeline layout!");
        }
    }

    void CreatePipeline() {
        // Créez le pipeline Vulkan pour dessiner les lignes
        // Cela inclut la création des shaders, la configuration des étapes du pipeline, etc.
    }
};
