#include "App.h"
#include "Log.h"

#include <vulkan/vulkan.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

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
              vulkanTextureImage(new VulkanImage(vulkanPhysicalDevice, vulkanDevice)),
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
        delete vulkanTextureImage;
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
            update();
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
        if (!initializeTextureImage()) {
            VD_LOG_ERROR("Could not initialize texture image");
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

    bool App::initializeTextureImage() {

        /*
         * Load image texels
         */

        int width;
        int height;
        int channels;
        int desiredChannels = STBI_rgb_alpha;
        stbi_uc* pixels = stbi_load("textures/texture.jpeg", &width, &height, &channels, desiredChannels);

        if (!pixels) {
            VD_LOG_ERROR("Could not load texture image");
            return false;
        }

        /*
         * Copy image texels to staging stagingBuffer
         */

        VkDeviceSize imageSize = width * height * desiredChannels;
        VulkanBuffer stagingBuffer(vulkanPhysicalDevice, vulkanDevice);

        VulkanBuffer::Config stagingBufferConfig{};
        stagingBufferConfig.Size = imageSize;
        stagingBufferConfig.Usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
        stagingBufferConfig.MemoryProperties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

        if (!stagingBuffer.initialize(stagingBufferConfig)) {
            VD_LOG_ERROR("Could not initialize texture image stagingBuffer");
            return false;
        }

        stagingBuffer.setData(pixels);
        stbi_image_free(pixels);

        /*
         * Copy image texelse from staging stagingBuffer to image stagingBuffer
         */

        VulkanImage::Config textureImageConfig{};
        textureImageConfig.Width = width;
        textureImageConfig.Height = height;
        textureImageConfig.Usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
        textureImageConfig.Format = VK_FORMAT_R8G8B8A8_SRGB;
        textureImageConfig.Tiling = VK_IMAGE_TILING_OPTIMAL;
        textureImageConfig.MemoryProperties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
        textureImageConfig.Layout = VK_IMAGE_LAYOUT_UNDEFINED;

        if (!vulkanTextureImage->initialize(textureImageConfig)) {
            VD_LOG_ERROR("Could not initialize texture image");
            return false;
        }

        VkImage textureImage = vulkanTextureImage->getVkImage();
        transitionImageLayout(textureImage, textureImageConfig.Format, textureImageConfig.Layout, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
        copyBufferToImage(stagingBuffer.getVkBuffer(), textureImage, (uint32_t) width, (uint32_t) height);
        transitionImageLayout(textureImage, textureImageConfig.Format, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

        stagingBuffer.terminate();

        VD_LOG_INFO("Initialized texture image");
        return true;
    }

    void App::transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout) const {
        VkCommandBuffer commandBuffer = beginSingleTimeCommands();

        VkImageMemoryBarrier imageMemoryBarrier{};
        imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        imageMemoryBarrier.oldLayout = oldLayout;
        imageMemoryBarrier.newLayout = newLayout;
        imageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        imageMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        imageMemoryBarrier.image = image;
        imageMemoryBarrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        imageMemoryBarrier.subresourceRange.baseMipLevel = 0;
        imageMemoryBarrier.subresourceRange.levelCount = 1;
        imageMemoryBarrier.subresourceRange.baseArrayLayer = 0;
        imageMemoryBarrier.subresourceRange.layerCount = 1;
        imageMemoryBarrier.srcAccessMask = 0;
        imageMemoryBarrier.dstAccessMask = 0;

        VkPipelineStageFlags sourceStage;
        VkPipelineStageFlags destinationStage;

        if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
            imageMemoryBarrier.srcAccessMask = 0;
            imageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
            destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        } else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
            imageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            imageMemoryBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
            sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
            destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
        } else {
            throw std::invalid_argument("Unsupported layout transition!");
        }

        constexpr VkDependencyFlags dependencyFlags = 0;
        constexpr uint32_t memoryBarrierCount = 0;
        constexpr VkMemoryBarrier* memoryBarriers = VK_NULL_HANDLE;
        constexpr uint32_t bufferMemoryBarrierCount = 0;
        constexpr VkBufferMemoryBarrier* bufferMemoryBarriers = VK_NULL_HANDLE;
        constexpr uint32_t imageMemoryBarrierCount = 1;
        vkCmdPipelineBarrier(
                commandBuffer,
                sourceStage,
                destinationStage,
                dependencyFlags,
                memoryBarrierCount,
                memoryBarriers,
                bufferMemoryBarrierCount,
                bufferMemoryBarriers,
                imageMemoryBarrierCount,
                &imageMemoryBarrier
        );

        endSingleTimeCommands(commandBuffer);
    }

    void App::copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height) const {
        VkCommandBuffer commandBuffer = beginSingleTimeCommands();

        VkBufferImageCopy region{};
        region.bufferOffset = 0;
        region.bufferRowLength = 0;
        region.bufferImageHeight = 0;

        region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        region.imageSubresource.mipLevel = 0;
        region.imageSubresource.baseArrayLayer = 0;
        region.imageSubresource.layerCount = 1;

        region.imageOffset = {0, 0, 0};

        constexpr uint32_t depth = 1;
        region.imageExtent = { width, height, depth };

        constexpr uint32_t regionCount = 1;
        constexpr VkImageLayout imageLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        vkCmdCopyBufferToImage(
                commandBuffer,
                buffer,
                image,
                imageLayout,
                regionCount,
                &region
        );
        
        endSingleTimeCommands(commandBuffer);
    }

    /*
     * TODO
     *
     * All of the helper functions that submit commands so far have been set up to execute synchronously by waiting for the queue to become idle.
     *
     * For practical applications it is recommended to combine these operations in a single command buffer and execute them asynchronously for higher throughput,
     * especially the transitions and copy in the createTextureImage function.
     *
     * Try to experiment with this by creating a setupCommandBuffer that the helper functions record commands into,
     * and add a flushSetupCommands to execute the commands that have been recorded so far.
     *
     * It's best to do this after the texture mapping works to check if the texture resources are still set up correctly.
     */
    VkCommandBuffer App::beginSingleTimeCommands() const {
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = vulkanCommandPool->getVkCommandPool();
        allocInfo.commandBufferCount = 1;

        VkCommandBuffer commandBuffer;
        vkAllocateCommandBuffers(vulkanDevice->getDevice(), &allocInfo, &commandBuffer);

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        vkBeginCommandBuffer(commandBuffer, &beginInfo);
        return commandBuffer;
    }

    void App::endSingleTimeCommands(VkCommandBuffer commandBuffer) const {
        vkEndCommandBuffer(commandBuffer);

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer;

        VkFence fence = VK_NULL_HANDLE;
        constexpr uint32_t submitCount = 1;
        vkQueueSubmit(vulkanDevice->getGraphicsQueue(), submitCount, &submitInfo, fence);
        vkQueueWaitIdle(vulkanDevice->getGraphicsQueue());

        vkFreeCommandBuffers(vulkanDevice->getDevice(), vulkanCommandPool->getVkCommandPool(), submitInfo.commandBufferCount, &commandBuffer);
    }

    bool App::initializeUniformBuffers() {
        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            VulkanUniformBuffer uniformBuffer(vulkanPhysicalDevice, vulkanDevice);
            if (!uniformBuffer.initialize(sizeof(CameraUniform))) {
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
            bufferInfo.range = sizeof(CameraUniform);

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
            constexpr VkCopyDescriptorSet* descriptorCopies = nullptr;
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

        VkAllocationCallbacks* allocationCallbacks = VK_NULL_HANDLE;
        vkDestroyDescriptorPool(vulkanDevice->getDevice(), descriptorPool, allocationCallbacks);
        vkDestroyDescriptorSetLayout(vulkanDevice->getDevice(), descriptorSetLayout, allocationCallbacks);

        terminateUniformBuffers();

        vulkanIndexBuffer->terminate();
        vulkanVertexBuffer->terminate();
        fragmentShader->terminate();
        vertexShader->terminate();
        vulkanTextureImage->terminate();
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
         * Frame acquisition
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
         * Command recording
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
        vkCmdBindVertexBuffers(vulkanCommandBuffer.getVkCommandBuffer(), firstBinding, bindingCount, vertexBuffers, vertexBufferOffsets);

        constexpr VkDeviceSize indexBufferOffset = 0;
        constexpr VkIndexType indexType = VK_INDEX_TYPE_UINT16;
        vkCmdBindIndexBuffer(vulkanCommandBuffer.getVkCommandBuffer(), vulkanIndexBuffer->getVulkanBuffer().getVkBuffer(), indexBufferOffset, indexType);

        VkDescriptorSet descriptorSet = descriptorSets[currentFrame];
        VkPipelineBindPoint pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        VkPipelineLayout pipelineLayout = vulkanGraphicsPipeline->getPipelineLayout();
        constexpr uint32_t firstSet = 0;
        constexpr uint32_t descriptorSetCount = 1;
        constexpr uint32_t dynamicOffsetCount = 0;
        constexpr uint32_t* dynamicOffsets = nullptr;
        vkCmdBindDescriptorSets(vulkanCommandBuffer.getVkCommandBuffer(), pipelineBindPoint, pipelineLayout, firstSet, descriptorSetCount, &descriptorSet, dynamicOffsetCount, dynamicOffsets);

        constexpr uint32_t instanceCount = 1;
        constexpr uint32_t firstVertex = 0;
        constexpr int32_t vertexOffset = 0;
        constexpr uint32_t firstInstance = 0;
        vkCmdDrawIndexed(vulkanCommandBuffer.getVkCommandBuffer(), (uint32_t) indices.size(), instanceCount, firstVertex, vertexOffset, firstInstance);

        vulkanRenderPass->end(vulkanCommandBuffer);

        if (!vulkanCommandBuffer.end()) {
            VD_LOG_CRITICAL("Could not end frame");
            throw std::runtime_error("Could not end frame");
        }

        /*
         * Command submission
         */

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        VkCommandBuffer vkCommandBuffer = vulkanCommandBuffer.getVkCommandBuffer();
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
         * Frame presentation
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

    void App::update() {
        static auto startTime = std::chrono::high_resolution_clock::now();

        auto currentTime = std::chrono::high_resolution_clock::now();
        float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

        CameraUniform cameraUniform{};

        const auto& modelTransform = glm::mat4(1.0f);
        float rotationAngle = time * glm::radians(90.0f);
        const auto& rotationAxis = glm::vec3(0.0f, 0.0f, 1.0f);
        cameraUniform.model = glm::rotate(modelTransform, rotationAngle, rotationAxis);

        const auto& eyeTransform = glm::vec3(2.0f, 2.0f, 2.0f);
        const auto& centerTransform = glm::vec3(0.0f, 0.0f, 0.0f);
        const auto& upAxis = glm::vec3(0.0f, 0.0f, 1.0f);
        cameraUniform.view = glm::lookAt(eyeTransform, centerTransform, upAxis);

        float fieldOfView = glm::radians(45.0f);
        float aspectRatio = (float) vulkanSwapChain->getExtent().width / (float) vulkanSwapChain->getExtent().height;
        float nearViewPlane = 0.1f;
        float farViewPlane = 10.0f;
        cameraUniform.projection = glm::perspective(fieldOfView, aspectRatio, nearViewPlane, farViewPlane);

        /*
         * GLM was originally designed for OpenGL, where the Y coordinate of the clip coordinates is inverted.
         * The easiest way to compensate for that is to flip the sign on the scaling factor of the Y rotationAxis in the projection matrix.
         * If we don't do this, then the image will be rendered upside down.
         *
         * This change causes the vertices to be drawn in counter-clockwise order instead of clockwise order.
         * This causes backface culling to kick in and prevents any geometry from being drawn.
         * To fix this the graphics pipeline's rasterization state should have a counter clockwise front-facing triangle orientation to be used for culling.
         *
         * VkPipelineRasterizationStateCreateInfo rasterizationState{};
         * rasterizationState.cullMode = VK_CULL_MODE_BACK_BIT;
         * rasterizationState.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
         */
        cameraUniform.projection[1][1] *= -1;

        const VulkanUniformBuffer& uniformBuffer = uniformBuffers[currentFrame];
        uniformBuffer.setData((void*) &cameraUniform);
    }

}