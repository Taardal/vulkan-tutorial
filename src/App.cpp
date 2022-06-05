#include "App.h"
#include "Log.h"

#include <vulkan/vulkan.h>

namespace Vulkandemo {

    const int MAX_FRAMES_IN_FLIGHT = 2;

    App::App(Config config)
            : config(std::move(config)),
              fileSystem(new FileSystem),
              window(new Window(config.Window)),
              vulkan(new Vulkan(config.Vulkan, window)),
              vulkanPhysicalDevice(new VulkanPhysicalDevice(vulkan)),
              vulkanDevice(new VulkanDevice(vulkan, vulkanPhysicalDevice)),
              vulkanSwapChain(new VulkanSwapChain(vulkanDevice, vulkanPhysicalDevice, vulkan, window)),
              vertexShader(new VulkanShader(vulkanDevice)),
              fragmentShader(new VulkanShader(vulkanDevice)),
              vulkanRenderPass(new VulkanRenderPass(vulkanSwapChain, vulkanDevice)),
              vulkanGraphicsPipeline(new VulkanGraphicsPipeline(vulkanRenderPass, vulkanSwapChain, vulkanDevice)),
              vulkanCommandPool(new VulkanCommandPool(vulkanPhysicalDevice, vulkanDevice)) {
    }

    App::~App() {
        delete vulkanCommandPool;
        delete vulkanGraphicsPipeline;
        delete vulkanRenderPass;
        delete fragmentShader;
        delete vertexShader;
        delete vulkanSwapChain;
        delete vulkanDevice;
        delete vulkanPhysicalDevice;
        delete vulkan;
        delete window;
        delete fileSystem;
    }

    void App::run() {
        if (!initialize()) {
            VD_LOG_CRITICAL("Could not initialize app");
            return;
        }
        VD_LOG_DEBUG("Running...");
        while (!window->shouldClose()) {
            window->pollEvents();
            drawFrame();
        }
        vulkanDevice->waitUntilIdle();
        terminate();
    }

    bool App::initialize() {
        Log::initialize(config.Name, config.LogLevel);
        VD_LOG_DEBUG("Initializing...");
        if (!window->initialize()) {
            VD_LOG_ERROR("Could not initialize window");
            return false;
        }
        if (!vulkan->initialize()) {
            VD_LOG_ERROR("Could not initialize Vulkan");
            return false;
        }
        if (!vulkanPhysicalDevice->initialize()) {
            VD_LOG_ERROR("Could not initialize Vulkan physical device");
            return false;
        }
        if (!vulkanDevice->initialize()) {
            VD_LOG_ERROR("Could not initialize Vulkan device");
            return false;
        }
        if (!vulkanSwapChain->initialize()) {
            VD_LOG_ERROR("Could not initialize Vulkan swap chain");
            return false;
        }

        std::vector<char> vertexShaderBytes = fileSystem->readBytes("shaders/simple_shader.vert.spv");
        VD_LOG_INFO("Vertex shader byte size: [{0}]", vertexShaderBytes.size());
        if (!vertexShader->initialize(vertexShaderBytes)) {
            VD_LOG_ERROR("Could not initialize vertex shader");
            return false;
        }
        std::vector<char> fragmentShaderBytes = fileSystem->readBytes("shaders/simple_shader.frag.spv");
        VD_LOG_INFO("Fragment shader byte size: [{0}]", fragmentShaderBytes.size());
        if (!fragmentShader->initialize(fragmentShaderBytes)) {
            VD_LOG_ERROR("Could not initialize fragment shader");
            return false;
        }

        if (!vulkanRenderPass->initialize()) {
            VD_LOG_ERROR("Could not initialize Vulkan render pass");
            return false;
        }
        if (!vulkanGraphicsPipeline->initialize(*vertexShader, *fragmentShader)) {
            VD_LOG_ERROR("Could not initialize Vulkan graphics pipeline");
            return false;
        }

        const std::vector<VkImageView>& swapChainImageViews = vulkanSwapChain->getImageViews();
        for (auto swapChainImageView : swapChainImageViews) {
            VulkanFramebuffer framebuffer(vulkanDevice, vulkanSwapChain, vulkanRenderPass);
            if (!framebuffer.initialize(swapChainImageView)) {
                VD_LOG_ERROR("Could not initialize framebuffers");
                return false;
            }
            framebuffers.push_back(framebuffer);
        }
        VD_LOG_INFO("Created [{}] Vulkan framebuffers", framebuffers.size());

        if (!vulkanCommandPool->initialize()) {
            VD_LOG_ERROR("Could not initialize Vulkan command pool");
            return false;
        }
        if (!createCommandBuffers()) {
            VD_LOG_ERROR("Could not create Vulkan command buffers");
            return false;
        }
        if (!createSyncObjects()) {
            VD_LOG_ERROR("Could not create Vulkan sync objects (semaphores & fences)");
            return false;
        }

        return true;
    }

    void App::terminate() {
        VD_LOG_DEBUG("Terminating...");

        VkAllocationCallbacks* allocationCallbacks = VK_NULL_HANDLE;
        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            vkDestroySemaphore(vulkanDevice->getDevice(), renderFinishedSemaphores[i], allocationCallbacks);
            vkDestroySemaphore(vulkanDevice->getDevice(), imageAvailableSemaphores[i], allocationCallbacks);
            vkDestroyFence(vulkanDevice->getDevice(), inFlightFences[i], allocationCallbacks);
        }
        VD_LOG_INFO("Destroyed Vulkan sync objects (semaphores & fences)");

        vulkanCommandPool->terminate();

        int framebufferCount = (int) framebuffers.size();
        for (VulkanFramebuffer framebuffer : framebuffers) {
            framebuffer.terminate();
        }
        VD_LOG_INFO("Destroyed [{}] Vulkan framebuffers", framebufferCount);

        vulkanGraphicsPipeline->terminate();
        vulkanRenderPass->terminate();
        fragmentShader->terminate();
        vertexShader->terminate();
        vulkanSwapChain->terminate();
        vulkanDevice->terminate();
        vulkan->terminate();
        window->terminate();
    }

    bool App::createCommandBuffers() {
        std::vector<VkCommandBuffer> vkCommandBuffers;
        vkCommandBuffers.resize(MAX_FRAMES_IN_FLIGHT);

        VkCommandBufferAllocateInfo allocateInfo{};
        allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocateInfo.commandBufferCount = vkCommandBuffers.size();
        allocateInfo.commandPool = vulkanCommandPool->getCommandPool();

        if (vkAllocateCommandBuffers(vulkanDevice->getDevice(), &allocateInfo, vkCommandBuffers.data()) != VK_SUCCESS) {
            VD_LOG_ERROR("Could not allocate [{}] Vulkan command buffers", vkCommandBuffers.size());
            return false;
        }
        for (VkCommandBuffer vkCommandBuffer : vkCommandBuffers) {
            vulkanCommandBuffers.push_back(new VulkanCommandBuffer(vkCommandBuffer));
        }
        VD_LOG_INFO("Allocated [{}] Vulkan command buffers", vulkanCommandBuffers.size());
        return true;
    }

    bool App::createSyncObjects() {
        imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
        renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
        inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

        VkSemaphoreCreateInfo semaphoreInfo{};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkFenceCreateInfo fenceInfo{};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        VkAllocationCallbacks* allocationCallbacks = VK_NULL_HANDLE;
        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            if (vkCreateSemaphore(vulkanDevice->getDevice(), &semaphoreInfo, allocationCallbacks, &imageAvailableSemaphores[i]) != VK_SUCCESS) {
                VD_LOG_ERROR("Could not create 'image available' semaphore for frame [{}]", i);
                return false;
            }
            if (vkCreateSemaphore(vulkanDevice->getDevice(), &semaphoreInfo, allocationCallbacks, &renderFinishedSemaphores[i]) != VK_SUCCESS) {
                VD_LOG_ERROR("Could not create 'render finished' semaphore for frame [{}]", i);
                return false;
            }
            if (vkCreateFence(vulkanDevice->getDevice(), &fenceInfo, allocationCallbacks, &inFlightFences[i]) != VK_SUCCESS) {
                VD_LOG_ERROR("Could not create 'in flight' fence for frame [{}]", i);
                return false;
            }
        }
        return true;
    }

    void App::drawFrame() {
        constexpr uint32_t fenceCount = 1;
        constexpr VkBool32 waitForAllFences = VK_TRUE;
        constexpr uint64_t waitForFenceTimeout = UINT64_MAX;
        VkFence inFlightFence = inFlightFences[currentFrame];
        vkWaitForFences(vulkanDevice->getDevice(), fenceCount, &inFlightFence, waitForAllFences, waitForFenceTimeout);
        vkResetFences(vulkanDevice->getDevice(), fenceCount, &inFlightFence);

        uint32_t swapChainImageIndex;
        VkFence acquireNextImageFence = VK_NULL_HANDLE;
        constexpr uint64_t acquireNextImageTimeout = UINT64_MAX;
        VkSemaphore imageAvailableSemaphore = imageAvailableSemaphores[currentFrame];
        vkAcquireNextImageKHR(vulkanDevice->getDevice(), vulkanSwapChain->getSwapChain(), acquireNextImageTimeout, imageAvailableSemaphore, acquireNextImageFence, &swapChainImageIndex);

        VulkanCommandBuffer* vulkanCommandBuffer = vulkanCommandBuffers[currentFrame];
        vulkanCommandBuffer->reset();
        vulkanCommandBuffer->begin();
        vulkanRenderPass->begin(vulkanCommandBuffer, framebuffers.at(swapChainImageIndex));
        vulkanGraphicsPipeline->bind(vulkanCommandBuffer);

        constexpr uint32_t vertexCount = 3;
        constexpr uint32_t instanceCount = 1;
        constexpr uint32_t firstVertex = 0;
        constexpr uint32_t firstInstance = 0;
        vkCmdDraw(vulkanCommandBuffer->getCommandBuffer(), vertexCount, instanceCount, firstVertex, firstInstance);

        vulkanRenderPass->end(vulkanCommandBuffer);
        if (!vulkanCommandBuffer->end()) {
            VD_LOG_CRITICAL("Could not end frame");
            throw std::runtime_error("Could not end frame");
        }

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        VkSemaphore waitSemaphores[] = {imageAvailableSemaphore};
        VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;

        VkCommandBuffer commandBuffer = vulkanCommandBuffer->getCommandBuffer();
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer;

        VkSemaphore renderFinishedSemaphore = renderFinishedSemaphores[currentFrame];
        VkSemaphore signalSemaphores[] = {renderFinishedSemaphore};
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;

        constexpr uint32_t submitCount = 1;
        if (vkQueueSubmit(vulkanDevice->getGraphicsQueue(), submitCount, &submitInfo, inFlightFence) != VK_SUCCESS) {
            VD_LOG_CRITICAL("Could not submit to graphics queue");
            throw std::runtime_error("Could not submit to graphics queue");
        }

        VkPresentInfoKHR presentInfo{};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = signalSemaphores;

        VkSwapchainKHR swapChains[] = {vulkanSwapChain->getSwapChain()};
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapChains;
        presentInfo.pImageIndices = &swapChainImageIndex;

        vkQueuePresentKHR(vulkanDevice->getPresentQueue(), &presentInfo);

        currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
    }

}