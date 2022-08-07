#include "App.h"
#include "Log.h"

#include <vulkan/vulkan.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <chrono>

namespace Vulkandemo {

    const uint32_t MAX_FRAMES_IN_FLIGHT = 2;

    App::App(Config config)
            : config(std::move(config)),
              fileSystem(new FileSystem),
              window(new Window(config.Window)),
              vulkan(new Vulkan(config.Vulkan, window)),
              vulkanPhysicalDevice(new VulkanPhysicalDevice(vulkan)),
              vulkanDevice(new VulkanDevice(vulkan, vulkanPhysicalDevice)),
              vulkanCommandPool(new VulkanCommandPool(vulkanPhysicalDevice, vulkanDevice)),
              vertexShader(new VulkanShader(vulkanDevice)),
              fragmentShader(new VulkanShader(vulkanDevice)),
              vulkanVertexBuffer(new VulkanVertexBuffer(vulkanPhysicalDevice, vulkanDevice, vulkanCommandPool)),
              vulkanIndexBuffer(new VulkanIndexBuffer(vulkanPhysicalDevice, vulkanDevice, vulkanCommandPool)),
              vulkanSwapChain(new VulkanSwapChain(vulkanDevice, vulkanPhysicalDevice, vulkan, window)),
              vulkanRenderPass(new VulkanRenderPass(vulkanSwapChain, vulkanDevice)),
              vulkanGraphicsPipeline(new VulkanGraphicsPipeline(vulkanRenderPass, vulkanSwapChain, vulkanDevice)) {
    }

    App::~App() {
        delete vulkanGraphicsPipeline;
        delete vulkanRenderPass;
        delete vulkanSwapChain;
        delete vulkanIndexBuffer;
        delete vulkanVertexBuffer;
        delete fragmentShader;
        delete vertexShader;
        delete vulkanCommandPool;
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
        VD_LOG_INFO("Running...");
        while (!window->shouldClose()) {
            window->pollEvents();
            updateUniformBuffer();
            drawFrame();
        }
        vulkanDevice->waitUntilIdle();
        terminate();
    }

    bool App::initialize() {
        Log::initialize(config.Name, config.LogLevel);
        VD_LOG_INFO("Initializing...");

        if (!window->initialize()) {
            VD_LOG_ERROR("Could not initialize window");
            return false;
        }
        window->setOnResize([this](int width, int height) {
            this->windowResized = true;
        });
        window->setOnMinimize([this](bool minimized) {
            this->windowResized = true;
        });

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
        if (!vulkanCommandPool->initialize()) {
            VD_LOG_ERROR("Could not initialize Vulkan command pool");
            return false;
        }
        vulkanCommandBuffers = vulkanCommandPool->allocateCommandBuffers(MAX_FRAMES_IN_FLIGHT);
        if (vulkanCommandBuffers.empty()) {
            VD_LOG_ERROR("Could not initialize Vulkan command buffers");
            return false;
        }
        if (!vertexShader->initialize(fileSystem->readBytes("shaders/simple_shader.vert.spv"))) {
            VD_LOG_ERROR("Could not initialize vertex shader");
            return false;
        }
        if (!fragmentShader->initialize(fileSystem->readBytes("shaders/simple_shader.frag.spv"))) {
            VD_LOG_ERROR("Could not initialize fragment shader");
            return false;
        }
        if (!vulkanVertexBuffer->initialize(vertices)) {
            VD_LOG_ERROR("Could not initialize Vulkan vertex buffer");
            return false;
        }
        if (!vulkanIndexBuffer->initialize(indices)) {
            VD_LOG_ERROR("Could not initialize Vulkan index buffer");
            return false;
        }
        if (!initializeUniformBuffers()) {
            VD_LOG_ERROR("Could not initialize Vulkan uniform buffers");
            return false;
        }
        if (!initializeDescriptorSetLayout()) {
            VD_LOG_ERROR("Could not initialize Vulkan descroptor set layout");
            return false;
        }
        if (!initializeDescriptorPool()) {
            VD_LOG_ERROR("Could not initialize Vulkan descriptor pool");
            return false;
        }
        if (!initializeDescriptorSets()) {
            VD_LOG_ERROR("Could not initialize Vulkan descroptor sets");
            return false;
        }
        if (!initializeRenderingObjects()) {
            VD_LOG_ERROR("Could not initialize Vulkan rendering objects (swap chain & dependents)");
            return false;
        }
        if (!initializeSyncObjects()) {
            VD_LOG_ERROR("Could not create Vulkan sync objects (semaphores & fences)");
            return false;
        }
        return true;
    }

    bool App::initializeUniformBuffers() {
        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            VulkanUniformBuffer uniformBuffer(vulkanPhysicalDevice, vulkanDevice);
            if (!uniformBuffer.initialize()) {
                VD_LOG_ERROR("Could not initialize uniform buffer for frame [{}]", i);
                return false;
            }
            uniformBuffers.push_back(uniformBuffer);
        }
        VD_LOG_INFO("Initialized [{}] Vulkan uniform buffers", uniformBuffers.size());
        return true;
    }

    bool App::initializeDescriptorSetLayout() {
        VkDescriptorSetLayoutBinding uboLayoutBinding{};
        uboLayoutBinding.binding = 0;
        uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        uboLayoutBinding.descriptorCount = 1;
        uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

        VkDescriptorSetLayoutCreateInfo layoutInfo{};
        layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        layoutInfo.bindingCount = 1;
        layoutInfo.pBindings = &uboLayoutBinding;

        constexpr VkAllocationCallbacks* allocationCallbacks = VK_NULL_HANDLE;
        if (vkCreateDescriptorSetLayout(vulkanDevice->getDevice(), &layoutInfo, allocationCallbacks, &descriptorSetLayout) != VK_SUCCESS) {
            VD_LOG_ERROR("Could not create uniform buffer descrptor set layout");
            return false;
        }
        return true;
    }

    bool App::initializeDescriptorPool() {
        VkDescriptorPoolSize poolSize{};
        poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        poolSize.descriptorCount = MAX_FRAMES_IN_FLIGHT;

        VkDescriptorPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolInfo.poolSizeCount = 1;
        poolInfo.pPoolSizes = &poolSize;
        poolInfo.maxSets = MAX_FRAMES_IN_FLIGHT;

        constexpr VkAllocationCallbacks* allocationCallbacks = VK_NULL_HANDLE;
        if (vkCreateDescriptorPool(vulkanDevice->getDevice(), &poolInfo, allocationCallbacks, &descriptorPool) != VK_SUCCESS) {
            VD_LOG_ERROR("Could not initialize descriptor pool");
            return false;
        }

        VD_LOG_INFO("Initialized descriptor pool");
        return true;
    }

    bool App::initializeDescriptorSets() {
        std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, descriptorSetLayout);

        VkDescriptorSetAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool = descriptorPool;
        allocInfo.descriptorSetCount = MAX_FRAMES_IN_FLIGHT;
        allocInfo.pSetLayouts = layouts.data();

        descriptorSets.resize(MAX_FRAMES_IN_FLIGHT);
        if (vkAllocateDescriptorSets(vulkanDevice->getDevice(), &allocInfo, descriptorSets.data()) != VK_SUCCESS) {
            VD_LOG_ERROR("Could not allocate [{}] descriptor sets", allocInfo.descriptorSetCount);
            return false;
        }

        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            const VulkanUniformBuffer& uniformBuffer = uniformBuffers[i];

            VkDescriptorBufferInfo bufferInfo{};
            bufferInfo.buffer = uniformBuffer.getBuffer().getVkBuffer();
            bufferInfo.offset = 0;
            bufferInfo.range = sizeof(UniformBufferObject);

            VkWriteDescriptorSet descriptorWrite{};
            descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptorWrite.dstSet = descriptorSets[i];
            descriptorWrite.dstBinding = 0;
            descriptorWrite.dstArrayElement = 0;
            descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            descriptorWrite.descriptorCount = 1;
            descriptorWrite.pBufferInfo = &bufferInfo;

            constexpr uint32_t descriptorWriteCount = 1;
            constexpr uint32_t descriptorCopyCount = 0;
            VkCopyDescriptorSet* descriptorCopies = nullptr;
            vkUpdateDescriptorSets(vulkanDevice->getDevice(), descriptorWriteCount, &descriptorWrite, descriptorCopyCount, descriptorCopies);
        }
        
        VD_LOG_INFO("Initialized descriptor sets");
        return true;
    }

    bool App::initializeRenderingObjects() {
        if (!vulkanSwapChain->initialize()) {
            VD_LOG_ERROR("Could not initialize Vulkan swap chain");
            return false;
        }
        if (!vulkanRenderPass->initialize()) {
            VD_LOG_ERROR("Could not initialize Vulkan render pass");
            return false;
        }
        if (!vulkanGraphicsPipeline->initialize(*vertexShader, *fragmentShader, descriptorSetLayout)) {
            VD_LOG_ERROR("Could not initialize Vulkan graphics pipeline");
            return false;
        }
        if (!initializeFramebuffers()) {
            VD_LOG_ERROR("Could not initialize Vulkan framebuffers");
            return false;
        }
        return true;
    }

    bool App::initializeFramebuffers() {
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
        return true;
    }

    bool App::initializeSyncObjects() {
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
        VD_LOG_INFO("Created Vulkan sync objects (semaphores & fences)");
        return true;
    }

    void App::terminate() {
        VD_LOG_INFO("Terminating...");
        terminateSyncObjects();
        terminateRenderingObjects();

        terminateUniformBuffers();

        VkAllocationCallbacks* allocationCallbacks = VK_NULL_HANDLE;
        vkDestroyDescriptorPool(vulkanDevice->getDevice(), descriptorPool, allocationCallbacks);
        vkDestroyDescriptorSetLayout(vulkanDevice->getDevice(), descriptorSetLayout, allocationCallbacks);

        vulkanIndexBuffer->terminate();
        vulkanVertexBuffer->terminate();
        fragmentShader->terminate();
        vertexShader->terminate();
        vulkanCommandPool->terminate();
        vulkanDevice->terminate();
        vulkan->terminate();
        window->terminate();
    }

    void App::terminateUniformBuffers() {
        for (VulkanUniformBuffer& uniformBuffer : uniformBuffers) {
            uniformBuffer.terminate();
        }
        VD_LOG_INFO("Terminated Vulkan uniform buffers");
    }

    void App::terminateSyncObjects() const {
        VkAllocationCallbacks* allocationCallbacks = VK_NULL_HANDLE;
        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            vkDestroySemaphore(vulkanDevice->getDevice(), renderFinishedSemaphores[i], allocationCallbacks);
            vkDestroySemaphore(vulkanDevice->getDevice(), imageAvailableSemaphores[i], allocationCallbacks);
            vkDestroyFence(vulkanDevice->getDevice(), inFlightFences[i], allocationCallbacks);
        }
        VD_LOG_INFO("Destroyed Vulkan sync objects (semaphores & fences)");
    }

    void App::terminateRenderingObjects() {
        terminateFramebuffers();
        vulkanGraphicsPipeline->terminate();
        vulkanRenderPass->terminate();
        vulkanSwapChain->terminate();
    }

    void App::terminateFramebuffers() {
        for (VulkanFramebuffer framebuffer : framebuffers) {
            framebuffer.terminate();
        }
        framebuffers.clear();
        VD_LOG_INFO("Destroyed Vulkan framebuffers");
    }

    bool App::recreateRenderingObjects() {
        window->waitUntilNotMinimized();
        vulkanDevice->waitUntilIdle();
        terminateRenderingObjects();
        vulkanPhysicalDevice->updateSwapChainInfo();
        return initializeRenderingObjects();
    }

    void App::drawFrame() {

        /*
         * Preparation
         */

        // Wait until the previous frame has finished
        constexpr uint32_t fenceCount = 1;
        constexpr VkBool32 waitForAllFences = VK_TRUE;
        constexpr uint64_t waitForFenceTimeout = UINT64_MAX;
        VkFence inFlightFence = inFlightFences[currentFrame];
        vkWaitForFences(vulkanDevice->getDevice(), fenceCount, &inFlightFence, waitForAllFences, waitForFenceTimeout);

        // Acquire an image from the swap chain
        uint32_t swapChainImageIndex;
        VkFence acquireNextImageFence = VK_NULL_HANDLE;
        constexpr uint64_t acquireNextImageTimeout = UINT64_MAX;
        VkSemaphore imageAvailableSemaphore = imageAvailableSemaphores[currentFrame];
        VkResult acquireNextImageResult = vkAcquireNextImageKHR(
                vulkanDevice->getDevice(),
                vulkanSwapChain->getSwapChain(),
                acquireNextImageTimeout,
                imageAvailableSemaphore,
                acquireNextImageFence,
                &swapChainImageIndex
        );
        // VK_ERROR_OUT_OF_DATE_KHR: The swap chain has become incompatible with the surface and can no longer be used for rendering. Usually happens after a window resize.
        if (acquireNextImageResult == VK_ERROR_OUT_OF_DATE_KHR) {
            recreateRenderingObjects();
            return;
        }
        // VK_SUBOPTIMAL_KHR: The swap chain can still be used to successfully present to the surface, but the surface properties are no longer matched exactly.
        if (acquireNextImageResult != VK_SUCCESS && acquireNextImageResult != VK_SUBOPTIMAL_KHR) {
            VD_LOG_CRITICAL("Could not acquire swap chain image");
            throw std::runtime_error("Could not acquire swap chain image");
        }

        // After waiting, we need to manually reset the fence to the unsignaled state
        vkResetFences(vulkanDevice->getDevice(), fenceCount, &inFlightFence);

        /*
         * Recording
         */

        const VulkanCommandBuffer& vulkanCommandBuffer = vulkanCommandBuffers[currentFrame];
        vulkanCommandBuffer.reset();
        vulkanCommandBuffer.begin();

        vulkanRenderPass->begin(vulkanCommandBuffer, framebuffers.at(swapChainImageIndex));
        vulkanGraphicsPipeline->bind(vulkanCommandBuffer);

        VkBuffer vertexBuffers[] = {vulkanVertexBuffer->getVulkanBuffer().getVkBuffer()};
        VkDeviceSize vertexBufferOffsets[] = {0};
        constexpr uint32_t firstBinding = 0;
        constexpr uint32_t bindingCount = 1;
        vkCmdBindVertexBuffers(vulkanCommandBuffer.getCommandBuffer(), firstBinding, bindingCount, vertexBuffers, vertexBufferOffsets);

        constexpr VkDeviceSize indexBufferOffset = 0;
        constexpr VkIndexType indexType = VK_INDEX_TYPE_UINT16;
        vkCmdBindIndexBuffer(vulkanCommandBuffer.getCommandBuffer(), vulkanIndexBuffer->getVulkanBuffer().getVkBuffer(), indexBufferOffset, indexType);

        VkDescriptorSet descriptorSet = descriptorSets[currentFrame];
        VkPipelineBindPoint pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        VkPipelineLayout pipelineLayout = vulkanGraphicsPipeline->getPipelineLayout();
        constexpr uint32_t firstSet = 0;
        constexpr uint32_t descriptorSetCount = 1;
        constexpr uint32_t dynamicOffsetCount = 0;
        constexpr uint32_t* dynamicOffsets = nullptr;
        vkCmdBindDescriptorSets(vulkanCommandBuffer.getCommandBuffer(), pipelineBindPoint, pipelineLayout, firstSet, descriptorSetCount, &descriptorSet, dynamicOffsetCount, dynamicOffsets);
        
        constexpr uint32_t instanceCount = 1;
        constexpr uint32_t firstVertex = 0;
        constexpr int32_t vertexOffset = 0;
        constexpr uint32_t firstInstance = 0;
        vkCmdDrawIndexed(vulkanCommandBuffer.getCommandBuffer(), (uint32_t) indices.size(), instanceCount, firstVertex, vertexOffset, firstInstance);

        vulkanRenderPass->end(vulkanCommandBuffer);

        if (!vulkanCommandBuffer.end()) {
            VD_LOG_CRITICAL("Could not end frame");
            throw std::runtime_error("Could not end frame");
        }

        /*
         * Submission
         */

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        VkCommandBuffer vkCommandBuffer = vulkanCommandBuffer.getCommandBuffer();
        submitInfo.pCommandBuffers = &vkCommandBuffer;
        submitInfo.commandBufferCount = 1;

        // Wait with writing colors to the image until it's available
        VkSemaphore waitSemaphores[] = {imageAvailableSemaphore};
        VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;
        submitInfo.waitSemaphoreCount = 1;

        // Which semaphores to signal once the command buffer(s) have finished execution
        VkSemaphore renderFinishedSemaphore = renderFinishedSemaphores[currentFrame];
        VkSemaphore signalSemaphores[] = {renderFinishedSemaphore};
        submitInfo.pSignalSemaphores = signalSemaphores;
        submitInfo.signalSemaphoreCount = 1;

        // Submit recorded graphics commands
        constexpr uint32_t submitCount = 1;
        if (vkQueueSubmit(vulkanDevice->getGraphicsQueue(), submitCount, &submitInfo, inFlightFence) != VK_SUCCESS) {
            VD_LOG_CRITICAL("Could not submit to graphics queue");
            throw std::runtime_error("Could not submit to graphics queue");
        }

        /*
         * Presentation
         */

        VkPresentInfoKHR presentInfo{};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

        // Which semaphores to wait on before presentation can happen
        presentInfo.pWaitSemaphores = signalSemaphores;
        presentInfo.waitSemaphoreCount = 1;

        // Which swap chain to present image to
        VkSwapchainKHR swapChains[] = {vulkanSwapChain->getSwapChain()};
        presentInfo.pSwapchains = swapChains;
        presentInfo.pImageIndices = &swapChainImageIndex;
        presentInfo.swapchainCount = 1;

        // Present image to swap chain
        VkResult presentResult = vkQueuePresentKHR(vulkanDevice->getPresentQueue(), &presentInfo);
        if (presentResult == VK_ERROR_OUT_OF_DATE_KHR || presentResult == VK_SUBOPTIMAL_KHR || windowResized) {
            windowResized = false;
            recreateRenderingObjects();
        } else if (presentResult != VK_SUCCESS) {
            VD_LOG_CRITICAL("Could not present image to swap chain");
            throw std::runtime_error("Could not present image to swap chain");
        }

        currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
    }

    void App::updateUniformBuffer() {
        static auto startTime = std::chrono::high_resolution_clock::now();

        auto currentTime = std::chrono::high_resolution_clock::now();
        float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

        UniformBufferObject ubo{};

        const auto& modelTransform = glm::mat4(1.0f);
        float rotationAngle = time * glm::radians(90.0f);
        const auto& rotationAxis = glm::vec3(0.0f, 0.0f, 1.0f);
        ubo.model = glm::rotate(modelTransform, rotationAngle, rotationAxis);

        const auto& eyeTransform = glm::vec3(2.0f, 2.0f, 2.0f);
        const auto& centerTransform = glm::vec3(0.0f, 0.0f, 0.0f);
        const auto& upAxis = glm::vec3(0.0f, 0.0f, 1.0f);
        ubo.view = glm::lookAt(eyeTransform, centerTransform, upAxis);

        float fieldOfView = glm::radians(45.0f);
        float aspectRatio = (float) vulkanSwapChain->getExtent().width / (float) vulkanSwapChain->getExtent().height;
        float nearViewPlane = 0.1f;
        float farViewPlane = 10.0f;
        ubo.proj = glm::perspective(fieldOfView, aspectRatio, nearViewPlane, farViewPlane);

        /*
         * GLM was originally designed for OpenGL, where the Y coordinate of the clip coordinates is inverted.
         * The easiest way to compensate for that is to flip the sign on the scaling factor of the Y rotationAxis in the projection matrix.
         * If we don't do this, then the image will be rendered upside down.
         *
         * This change causes the vertices to be drawn in counter-clockwise order instead of clockwise order.
         * This causes backface culling to kick in and prevents any geometry from being drawn.
         * To fix this the graphics pipeline's rasterization state should have a front-facing triangle orientation to be used for culling.
         *
         * VkPipelineRasterizationStateCreateInfo rasterizationState{};
         * rasterizationState.cullMode = VK_CULL_MODE_BACK_BIT;
         * rasterizationState.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
         */
        ubo.proj[1][1] *= -1;

        const VulkanUniformBuffer& uniformBuffer = uniformBuffers[currentFrame];
        uniformBuffer.setData(ubo);
    }

}