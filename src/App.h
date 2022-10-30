#pragma once

#include "Log.h"
#include "FileSystem.h"
#include "Window.h"
#include "Vulkan.h"
#include "VulkanPhysicalDevice.h"
#include "VulkanDevice.h"
#include "VulkanCommandPool.h"
#include "VulkanCommandBuffer.h"
#include "VulkanVertexBuffer.h"
#include "VulkanIndexBuffer.h"
#include "VulkanUniformBuffer.h"
#include "VulkanSwapChain.h"
#include "VulkanRenderPass.h"
#include "VulkanGraphicsPipeline.h"
#include "VulkanFramebuffer.h"
#include "VulkanImage.h"
#include "Vertex.h"

#include <vulkan/vulkan.h>

#include <vector>

namespace Vulkandemo {

    struct CameraUniform {
        alignas(16) glm::mat4 model;
        alignas(16) glm::mat4 view;
        alignas(16) glm::mat4 projection;
    };

    class App {
    public:
        struct Config {
            std::string Name;
            Log::Level LogLevel;
            Window::Config Window;
            Vulkan::Config Vulkan;
        };

    private:
        Config config;
        FileSystem* fileSystem;
        Window* window;
        Vulkan* vulkan;
        VulkanPhysicalDevice* vulkanPhysicalDevice;
        VulkanDevice* vulkanDevice;
        VulkanCommandPool* vulkanCommandPool;
        std::vector<VulkanCommandBuffer> vulkanCommandBuffers;
        VulkanShader* vertexShader;
        VulkanShader* fragmentShader;
        VulkanVertexBuffer* vulkanVertexBuffer;
        VulkanIndexBuffer* vulkanIndexBuffer;
        VulkanSwapChain* vulkanSwapChain;
        VulkanRenderPass* vulkanRenderPass;
        VulkanGraphicsPipeline* vulkanGraphicsPipeline;
        std::vector<VulkanFramebuffer> framebuffers;
        std::vector<VkSemaphore> imageAvailableSemaphores;
        std::vector<VkSemaphore> renderFinishedSemaphores;
        std::vector<VkFence> inFlightFences;
        uint32_t currentFrame = 0;
        bool windowResized = false;

        std::vector<VulkanUniformBuffer> uniformBuffers;
        VkDescriptorSetLayout descriptorSetLayout = VK_NULL_HANDLE;
        VkDescriptorPool descriptorPool = VK_NULL_HANDLE;
        std::vector<VkDescriptorSet> descriptorSets;

        VulkanImage* vulkanTextureImage;
        VkImageView textureImageView = VK_NULL_HANDLE;
        VkSampler textureSampler = VK_NULL_HANDLE;

        VulkanImage* vulkanDepthImage;
        VkImageView depthImageView = VK_NULL_HANDLE;

        const std::string MODEL_PATH = "models/viking_room.obj";
        const std::string TEXTURE_PATH = "textures/viking_room.png";

        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;

        uint32_t mipLevels = 0;

        VulkanImage* vulkanColorImage;
        VkImageView colorImageView = VK_NULL_HANDLE;

    public:
        explicit App(Config config);

        ~App();

        void run();

    private:
        bool initialize();

        bool loadModel();

        bool initializeColorResources();

        bool initializeDepthResources();

        VkFormat findDepthFormat();

        bool hasStencilComponent(VkFormat format) const;

        VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels);

        bool initializeTextureImage();

        bool initializeTextureImageView();

        bool initializeTextureSampler();

        bool transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels) const;

        bool generateMipmaps(VkImage image, VkFormat imageFormat, int32_t texWidth, int32_t texHeight, uint32_t mipLevels) const;

        void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height) const;

        VkCommandBuffer beginSingleTimeCommands() const;

        void endSingleTimeCommands(VkCommandBuffer commandBuffer) const;

        bool initializeUniformBuffers();

        bool initializeDescriptorSetLayout();

        bool initializeDescriptorPool();

        bool initializeDescriptorSets();

        bool initializeRenderingObjects();

        bool initializeFramebuffers();

        bool initializeSyncObjects();

        void terminate();

        void terminateSyncObjects() const;

        void terminateFramebuffers();

        void terminateColorResources();

        void terminateDepthResources();

        void terminateRenderingObjects();

        void terminateUniformBuffers();

        bool recreateRenderingObjects();

        void drawFrame();

        void update();
    };

}
