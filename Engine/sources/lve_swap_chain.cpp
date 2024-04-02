#include "lve_swap_chain.h"

// std
#include <array>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <limits>
#include <set>
#include <stdexcept>

namespace lve {


    LveSwapChain::LveSwapChain(LveDevice& deviceRef, vk::Extent2D extent)
        : lveDevice{ deviceRef }, windowExtent{ extent } {
        Init();
    }

    LveSwapChain::LveSwapChain(LveDevice& deviceRef, vk::Extent2D extent, std::shared_ptr<LveSwapChain> previous) : lveDevice{ deviceRef }, windowExtent{ extent }, oldSwapChain{ previous } {
        Init();
        oldSwapChain = nullptr;
    }

    void LveSwapChain:: Init() {
        createSwapChain();
        createImageViews();
        createRenderPass();
        createDepthResources();
        createFramebuffers();
        createSyncObjects();
    }


    LveSwapChain::~LveSwapChain() {
        // Nettoie les image views de la cha�ne de swap
        for (auto imageView : swapChainImageViews) {
            lveDevice.device().destroyImageView(imageView, nullptr);
        }
        swapChainImageViews.clear();

        // D�truit la cha�ne de swap
        if (swapChain != nullptr) {
            lveDevice.device().destroySwapchainKHR(swapChain, nullptr);
            swapChain = nullptr;
        }

        // Nettoie les images de profondeur, leurs vues et la m�moire associ�e
        for (int i = 0; i < depthImages.size(); i++) {
            lveDevice.device().destroyImageView(depthImageViews[i], nullptr);
            lveDevice.device().destroyImage(depthImages[i], nullptr);
            lveDevice.device().freeMemory(depthImageMemorys[i], nullptr);
        }

        // Nettoie les framebuffers de la cha�ne de swap
        for (auto framebuffer : swapChainFramebuffers) {
            lveDevice.device().destroyFramebuffer(framebuffer, nullptr);
        }

        // D�truit le render pass
        lveDevice.device().destroyRenderPass(renderPass, nullptr);

        // Nettoie les objets de synchronisation
        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            lveDevice.device().destroySemaphore(renderFinishedSemaphores[i], nullptr);
            lveDevice.device().destroySemaphore(imageAvailableSemaphores[i], nullptr);
            lveDevice.device().destroyFence(inFlightFences[i], nullptr);
        }
    }

    vk::Result LveSwapChain::acquireNextImage(uint32_t* imageIndex) {
        // Attend que les barri�res de synchronisation soient satisfaites
        vk::Result result_ = lveDevice.device().waitForFences(1, &inFlightFences[currentFrame], VK_TRUE, std::numeric_limits<uint64_t>::max());
        if (result_ != vk::Result::eSuccess) {
            std::cout << "ALERTE" << std::endl;
        }

        // Acquiert l'index de l'image suivante dans la cha�ne de swap
        // en utilisant l'extension KHR_swapchain
        vk::Result result = lveDevice.device().acquireNextImageKHR(
            swapChain,                                          // La cha�ne de swap
            std::numeric_limits<uint64_t>::max(),              // Dur�e d'attente infinie
            imageAvailableSemaphores[currentFrame],            // S�maphore non signal� (attendre qu'il soit signal� avant)
            nullptr,                                           // Pas de gestion de l'objet de barri�re de blocage
            imageIndex);                                       // L'index de l'image acquise

        return result;
    }

    vk::Result LveSwapChain::submitCommandBuffers(const vk::CommandBuffer* buffers, uint32_t* imageIndex) {
        // Attend la fin des commandes en cours d'ex�cution pour l'image en cours d'acquisition
        if (imagesInFlight[*imageIndex] != nullptr) {
            vk::Result result = lveDevice.device().waitForFences(1, &imagesInFlight[*imageIndex], VK_TRUE, std::numeric_limits<uint64_t>::max());
            if (result != vk::Result::eSuccess) {
                // G�rer l'erreur ou afficher un message d'erreur
            }
        }
        imagesInFlight[*imageIndex] = inFlightFences[currentFrame];

        // Configuration de la soumission des commandes
        vk::SubmitInfo submitInfo;
        submitInfo.sType = vk::StructureType::eSubmitInfo;

        vk::Semaphore waitSemaphores[] = { imageAvailableSemaphores[currentFrame] };
        // D�claration d'un tableau de type VkPipelineStageFlags
        vk::PipelineStageFlags waitStages[] = { vk::PipelineStageFlagBits::eColorAttachmentOutput };  // Initialisation du tableau avec un seul �l�ment contenant le drapeau VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT

        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;

        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = buffers;

        vk::Semaphore signalSemaphores[] = { renderFinishedSemaphores[currentFrame] };
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;

        // Soumet les commandes � la file de commandes graphiques
        lveDevice.device().resetFences(1, &inFlightFences[currentFrame]);
        if (lveDevice.graphicsQueue().submit(1, &submitInfo, inFlightFences[currentFrame]) !=
            vk::Result::eSuccess) {
            throw std::runtime_error("failed to submit draw command buffer!");
        }

        // Configuration de la pr�sentation
        vk::PresentInfoKHR presentInfo;
        presentInfo.sType = vk::StructureType::ePresentInfoKHR;

        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = signalSemaphores;

        vk::SwapchainKHR swapChains[] = { swapChain };
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapChains;

        presentInfo.pImageIndices = imageIndex;

        // Pr�sente l'image
        vk::Result result = lveDevice.presentQueue().presentKHR(&presentInfo);

        // Passe au prochain frame
        currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;

        return result;
    }



    void LveSwapChain::createSwapChain() {
        // Obtient les d�tails de prise en charge de la cha�ne de swap
        SwapChainSupportDetails swapChainSupport = lveDevice.getSwapChainSupport();

        // Choix du format de surface de la cha�ne de swap
        vk::SurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);

        // Choix du mode de pr�sentation de la cha�ne de swap
        vk::PresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);

        // Choix de l'�tendue de la cha�ne de swap
        vk::Extent2D extent = chooseSwapExtent(swapChainSupport.capabilities);

        // D�termine le nombre d'images dans la cha�ne de swap
        uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
        if (swapChainSupport.capabilities.maxImageCount > 0 &&
            imageCount > swapChainSupport.capabilities.maxImageCount) {
            imageCount = swapChainSupport.capabilities.maxImageCount;
        }

        // Configuration de la cr�ation de la cha�ne de swap
        vk::SwapchainCreateInfoKHR createInfo = {};
        createInfo.sType = vk::StructureType::eSwapchainCreateInfoKHR;
        createInfo.setSurface(lveDevice.surface());

        createInfo.setMinImageCount(imageCount);
        createInfo.setImageFormat(surfaceFormat.format);
        createInfo.setImageColorSpace(surfaceFormat.colorSpace);
        createInfo.setImageExtent(extent);
        createInfo.setImageArrayLayers(1);
        createInfo.setImageUsage(vk::ImageUsageFlagBits::eColorAttachment);

        QueueFamilyIndices indices = lveDevice.findPhysicalQueueFamilies();
        uint32_t queueFamilyIndices[] = { indices.graphicsFamily, indices.presentFamily };

        if (indices.graphicsFamily != indices.presentFamily) {
            // Si les familles de files d'attente sont diff�rentes, partagez les images
            createInfo.setImageSharingMode(vk::SharingMode::eConcurrent);
            createInfo.setQueueFamilyIndexCount(2);
            createInfo.setPQueueFamilyIndices(queueFamilyIndices);
        }
        else {
            // Si les familles de files d'attente sont les m�mes, utilisez exclusivement
            createInfo.setImageSharingMode(vk::SharingMode::eExclusive);
            createInfo.queueFamilyIndexCount = 0;     
            createInfo.pQueueFamilyIndices = nullptr; 
        }

        createInfo.setPreTransform(swapChainSupport.capabilities.currentTransform);
        createInfo.setCompositeAlpha(vk::CompositeAlphaFlagBitsKHR::eOpaque);

        createInfo.setPresentMode(presentMode);
        createInfo.setClipped(VK_TRUE);

        createInfo.setOldSwapchain(oldSwapChain == nullptr ? nullptr : oldSwapChain->swapChain);

        // Cr�e la cha�ne de swap
        swapChain = lveDevice.device().createSwapchainKHR(createInfo);

        // Obtient les images de la cha�ne de swap
        // R�cup�rer le nombre d'images de swapchain
        swapChainImages = lveDevice.device().getSwapchainImagesKHR(swapChain);
        swapChainImages.resize(imageCount);

        if (lveDevice.device().getSwapchainImagesKHR(swapChain, &imageCount, swapChainImages.data()) != vk::Result::eSuccess) {
            throw std::runtime_error("failed to create swap chain!");
        }

        swapChainImageFormat = surfaceFormat.format;
        swapChainExtent = extent;
    }

    void LveSwapChain::createImageViews() {
        swapChainImageViews.resize(swapChainImages.size());
        for (size_t i = 0; i < swapChainImages.size(); i++) {
            vk::ImageViewCreateInfo viewInfo;
            viewInfo.sType = vk::StructureType::eImageViewCreateInfo;
            viewInfo.image = swapChainImages[i];
            viewInfo.viewType = vk::ImageViewType::e2D;
            viewInfo.format = swapChainImageFormat;
            viewInfo.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
            viewInfo.subresourceRange.baseMipLevel = 0;
            viewInfo.subresourceRange.levelCount = 1;
            viewInfo.subresourceRange.baseArrayLayer = 0;
            viewInfo.subresourceRange.layerCount = 1;

            if (lveDevice.device().createImageView(&viewInfo, nullptr, &swapChainImageViews[i]) != vk::Result::eSuccess)
            {
                throw std::runtime_error("failed to create texture image view!");
            }
           
        }
    }


    void LveSwapChain::createRenderPass() {
        vk::AttachmentDescription depthAttachment{};
        depthAttachment.format = findDepthFormat();
        depthAttachment.samples = vk::SampleCountFlagBits::e1;
        depthAttachment.loadOp = vk::AttachmentLoadOp::eClear;
        depthAttachment.storeOp = vk::AttachmentStoreOp::eDontCare;
        depthAttachment.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
        depthAttachment.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
        depthAttachment.initialLayout = vk::ImageLayout::eUndefined;
        depthAttachment.finalLayout = vk::ImageLayout::eDepthStencilAttachmentOptimal;

        vk::AttachmentReference depthAttachmentRef{};
        depthAttachmentRef.attachment = 1;
        depthAttachmentRef.layout = vk::ImageLayout::eDepthStencilAttachmentOptimal;

        vk::AttachmentDescription colorAttachment{};
        colorAttachment.format = getSwapChainImageFormat();
        colorAttachment.samples = vk::SampleCountFlagBits::e1;
        colorAttachment.loadOp = vk::AttachmentLoadOp::eClear;
        colorAttachment.storeOp = vk::AttachmentStoreOp::eStore;
        colorAttachment.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
        colorAttachment.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
        colorAttachment.initialLayout = vk::ImageLayout::eUndefined;
        colorAttachment.finalLayout = vk::ImageLayout::ePresentSrcKHR;

        vk::AttachmentReference colorAttachmentRef{};
        colorAttachmentRef.attachment = 0;
        colorAttachmentRef.layout = vk::ImageLayout::eColorAttachmentOptimal;

        vk::SubpassDescription subpass{};
        subpass.pipelineBindPoint = vk::PipelineBindPoint::eGraphics;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &colorAttachmentRef;
        subpass.pDepthStencilAttachment = &depthAttachmentRef;

        vk::SubpassDependency dependency{};
        dependency.dstSubpass = 0;
        dependency.dstAccessMask =
            vk::AccessFlagBits::eColorAttachmentWrite | vk::AccessFlagBits::eDepthStencilAttachmentWrite;
        dependency.dstStageMask =
            vk::PipelineStageFlagBits::eColorAttachmentOutput | vk::PipelineStageFlagBits::eEarlyFragmentTests;
        dependency.srcSubpass = vk::SubpassExternal;
        dependency.srcAccessMask = vk::AccessFlags{};
        dependency.srcStageMask =
            vk::PipelineStageFlagBits::eColorAttachmentOutput | vk::PipelineStageFlagBits::eEarlyFragmentTests;

        std::array<vk::AttachmentDescription, 2> attachments = { colorAttachment, depthAttachment };
        vk::RenderPassCreateInfo renderPassInfo{};
        renderPassInfo.sType = vk::StructureType::eRenderPassCreateInfo;
        renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
        renderPassInfo.pAttachments = attachments.data();
        renderPassInfo.subpassCount = 1;
        renderPassInfo.pSubpasses = &subpass;
        renderPassInfo.dependencyCount = 1;
        renderPassInfo.pDependencies = &dependency;

        if (lveDevice.device().createRenderPass(&renderPassInfo, nullptr, &renderPass) != vk::Result::eSuccess) {
            throw std::runtime_error("failed to create render pass!");
        }
    }


    void LveSwapChain::createFramebuffers() {
        swapChainFramebuffers.resize(imageCount());

        for (size_t i = 0; i < imageCount(); i++) {
            std::array<vk::ImageView, 2> attachments = { swapChainImageViews[i], depthImageViews[i] };

            vk::Extent2D swapChainExtent = getSwapChainExtent();
            vk::FramebufferCreateInfo framebufferInfo;
            framebufferInfo.sType = vk::StructureType::eFramebufferCreateInfo;
            framebufferInfo.setRenderPass(renderPass);
            framebufferInfo.setAttachmentCount(static_cast<uint32_t>(attachments.size()));
            framebufferInfo.setPAttachments(attachments.data());
            framebufferInfo.setWidth(swapChainExtent.width);
            framebufferInfo.setHeight(swapChainExtent.height);
            framebufferInfo.setLayers(1);

            if (lveDevice.device().createFramebuffer(&framebufferInfo, nullptr, &swapChainFramebuffers[i]) != vk::Result::eSuccess) {
                throw std::runtime_error("failed to create framebuffer!");
            }
        }
    }

    void LveSwapChain::createDepthResources() {
        vk::Format depthFormat = findDepthFormat();
        swapChainDepthFormat = depthFormat;
        vk::Extent2D swapChainExtent = getSwapChainExtent();

        depthImages.resize(imageCount());
        depthImageMemorys.resize(imageCount());
        depthImageViews.resize(imageCount());

        for (int i = 0; i < depthImages.size(); i++) {
            vk::ImageCreateInfo imageInfo;
            imageInfo.sType = vk::StructureType::eImageCreateInfo;
            imageInfo.setImageType(vk::ImageType::e2D);
            imageInfo.setExtent({ swapChainExtent.width, swapChainExtent.height, 1 });
            imageInfo.setMipLevels(1);
            imageInfo.setArrayLayers(1);
            imageInfo.setFormat(depthFormat);
            imageInfo.setTiling(vk::ImageTiling::eOptimal);
            imageInfo.setInitialLayout(vk::ImageLayout::eUndefined);
            imageInfo.setUsage(vk::ImageUsageFlagBits::eDepthStencilAttachment);
            imageInfo.setSamples(vk::SampleCountFlagBits::e1);
            imageInfo.setSharingMode(vk::SharingMode::eExclusive);

            lveDevice.createImageWithInfo(
                imageInfo,
                vk::MemoryPropertyFlagBits::eDeviceLocal,
                depthImages[i],
                depthImageMemorys[i]);

            vk::ImageViewCreateInfo viewInfo;
            viewInfo.sType = vk::StructureType::eImageViewCreateInfo;
            viewInfo.setImage(depthImages[i]);
            viewInfo.setViewType(vk::ImageViewType::e2D);
            viewInfo.setFormat(depthFormat);
            viewInfo.setSubresourceRange({ vk::ImageAspectFlagBits::eDepth, 0, 1, 0, 1 });

            if (lveDevice.device().createImageView(&viewInfo, nullptr, &depthImageViews[i]) != vk::Result::eSuccess) {
                throw std::runtime_error("failed to create texture image view!");
            }
        }
    }


    void LveSwapChain::createSyncObjects() {
        imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
        renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
        inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
        imagesInFlight.resize(imageCount(), vk::Fence{});

        vk::SemaphoreCreateInfo semaphoreInfo{};
        semaphoreInfo.sType = vk::StructureType::eSemaphoreCreateInfo;

        vk::FenceCreateInfo fenceInfo{};
        fenceInfo.sType = vk::StructureType::eFenceCreateInfo;
        fenceInfo.flags = vk::FenceCreateFlagBits::eSignaled;

        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            if (lveDevice.device().createSemaphore(&semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) !=
                vk::Result::eSuccess ||
                lveDevice.device().createSemaphore( &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) !=
                vk::Result::eSuccess ||
                lveDevice.device().createFence(&fenceInfo, nullptr, &inFlightFences[i]) != vk::Result::eSuccess) {
                throw std::runtime_error("failed to create synchronization objects for a frame!");
            }
        }
    }

    vk::SurfaceFormatKHR LveSwapChain::chooseSwapSurfaceFormat(
        const std::vector<vk::SurfaceFormatKHR>& availableFormats) {
        for (const auto& availableFormat : availableFormats) {
            if (availableFormat.format == vk::Format::eB8G8R8A8Srgb &&
                availableFormat.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear) {
                return availableFormat;
            }
        }

        return availableFormats[0];
    }


    vk::PresentModeKHR LveSwapChain::chooseSwapPresentMode(
        const std::vector<vk::PresentModeKHR>& availablePresentModes) {
       /* for (const auto& availablePresentMode : availablePresentModes) {
            if (availablePresentMode == vk::PresentModeKHR::eMailbox) {
                std::cout << "Present mode: Mailbox" << std::endl;
                return availablePresentMode;
            }
        }*/

        for (const auto& availablePresentMode : availablePresentModes) {
            if (availablePresentMode == vk::PresentModeKHR::eImmediate) {
                std::cout << "Present mode: Immediate" << std::endl;
                return availablePresentMode;
            }
        }

        std::cout << "Present mode: V-Sync" << std::endl;
        return vk::PresentModeKHR::eFifo;
    }

    vk::Extent2D LveSwapChain::chooseSwapExtent(const vk::SurfaceCapabilitiesKHR& capabilities) {
        if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
            return capabilities.currentExtent;
        }
        else {
            vk::Extent2D actualExtent = windowExtent;
            actualExtent.width = std::max(
                capabilities.minImageExtent.width,
                std::min(capabilities.maxImageExtent.width, actualExtent.width));
            actualExtent.height = std::max(
                capabilities.minImageExtent.height,
                std::min(capabilities.maxImageExtent.height, actualExtent.height));

            return actualExtent;
        }
    }

    vk::Format LveSwapChain::findDepthFormat() {
        return lveDevice.findSupportedFormat(
            { vk::Format::eD32Sfloat, vk::Format::eD32SfloatS8Uint, vk::Format::eD24UnormS8Uint },
            vk::ImageTiling::eOptimal,
            vk::FormatFeatureFlagBits::eDepthStencilAttachment);
    }

    void LveSwapChain::createTextureSampler() {
        vk::SamplerCreateInfo samplerInfo{};
        samplerInfo.sType = vk::StructureType::eSamplerCreateInfo;
        samplerInfo.magFilter = vk::Filter::eLinear;
        samplerInfo.minFilter = vk::Filter::eLinear;

        samplerInfo.addressModeU = vk::SamplerAddressMode::eRepeat;
        samplerInfo.addressModeV = vk::SamplerAddressMode::eRepeat;
        samplerInfo.addressModeW = vk::SamplerAddressMode::eRepeat;

        //samplerInfo.anisotropyEnable = vk::True;
        //samplerInfo.maxAnisotropy = lveDevice.properties.limits.maxSamplerAnisotropy;
        samplerInfo.anisotropyEnable = vk::False;
        samplerInfo.maxAnisotropy = 1.0f;

        samplerInfo.borderColor = vk::BorderColor::eIntOpaqueBlack;
        samplerInfo.unnormalizedCoordinates = vk::False;

        samplerInfo.compareEnable = vk::False;
        samplerInfo.compareOp = vk::CompareOp::eAlways;

        samplerInfo.mipmapMode = vk::SamplerMipmapMode::eLinear;
        samplerInfo.mipLodBias = 0.0f;
        samplerInfo.minLod = 0.0f;
        samplerInfo.maxLod = 0.0f;

    }

}  // namespace lve
