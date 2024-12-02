#include "VulkanRenderer.h"

VulkanRenderer::VulkanRenderer(QVulkanWindow *window) : m_window(window) {
    // 初始化Vulkan资源
    initResources();
}


void VulkanRenderer::initResources()
{
    // 1. 初始化 Vulkan 实例
    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Qt Vulkan Triangle Example";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_2;

    VkInstanceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    // 需要启用扩展
    uint32_t extensionCount = 0;
    const char** extensions = glfwGetRequiredInstanceExtensions(&extensionCount);
    createInfo.enabledExtensionCount = extensionCount;
    createInfo.ppEnabledExtensionNames = extensions;

    // 创建 Vulkan 实例
    if (vkCreateInstance(&createInfo, nullptr, &m_instance) != VK_SUCCESS) {
        qFatal("failed to create Vulkan instance!");
    }

    // 2. 创建 Vulkan 设备和队列
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(m_instance, &deviceCount, nullptr);
    if (deviceCount == 0) {
        qFatal("failed to find a GPU with Vulkan support!");
    }
    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(m_instance, &deviceCount, devices.data());

    physicalDevice = devices[0];  // 选择第一个设备
    VkDeviceCreateInfo deviceCreateInfo = {};
    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.queueCreateInfoCount = 1;

    VkDeviceQueueCreateInfo queueCreateInfo = {};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueCount = 1;
    queueCreateInfo.pQueuePriorities = (float[])1.0f;

    deviceCreateInfo.pQueueCreateInfos = &queueCreateInfo;

    if (vkCreateDevice(physicalDevice, &deviceCreateInfo, nullptr, &m_device) != VK_SUCCESS) {
        qFatal("failed to create Vulkan device!");
    }

    vkGetDeviceQueue(m_device, 0, 0, &m_graphicsQueue);
}

void VulkanRenderer::releaseResources()
{
    vkDestroyDevice(m_device, nullptr);
    vkDestroyInstance(m_instance, nullptr);
}

void VulkanRenderer::startNextFrame()
{
    // 2. 每帧开始时调用，用来开始下一帧的渲染。
    // 在此处，通常需要处理交换链、渲染目标等。

    // 创建命令缓冲区并记录绘制命令
    VkCommandBuffer commandBuffer;
    VkCommandBufferAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = m_commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = 1;

    vkAllocateCommandBuffers(m_device, &allocInfo, &commandBuffer);

    VkCommandBufferBeginInfo beginInfo = {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    vkBeginCommandBuffer(commandBuffer, &beginInfo);

    // 设置清除颜色值
    VkClearColorValue clearColor = {};
    clearColor.float32[0] = 0.0f;
    clearColor.float32[1] = 0.0f;
    clearColor.float32[2] = 1.0f;
    clearColor.float32[3] = 1.0f;

    VkClearAttachment clearAttachment = {};
    clearAttachment.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    clearAttachment.clearValue.color = clearColor;

    VkClearRect clearRect = {};
    clearRect.rect.extent.width = m_window->width();
    clearRect.rect.extent.height = m_window->height();
    clearRect.layerCount = 1;

    vkCmdClearAttachments(commandBuffer, 1, &clearAttachment, 1, &clearRect);

    // 绘制命令（此处应包括顶点缓冲区和着色器）

    vkEndCommandBuffer(commandBuffer);

    // 提交命令缓冲区
    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    vkQueueSubmit(m_graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(m_graphicsQueue);
}

