#include "VulkanRenderer.h"
#include <QVulkanDeviceFunctions>




static const Vertex vertices[] = {
    {{0.0f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}},
    {{0.5f, 0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}},
    {{-0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}},
    };

VulkanRenderer::VulkanRenderer(QVulkanWindow *window) : m_window(window) {
    // 初始化Vulkan资源
    initResources();
}


void VulkanRenderer::initResources()
{
    QVulkanDeviceFunctions *devFuncs = m_window->vulkanInstance()->deviceFunctions(m_window->device());

    // 创建顶点缓冲区
    VkBufferCreateInfo bufferInfo = {};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = sizeof(vertices);
    bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (devFuncs->vkCreateBuffer(m_window->device(), &bufferInfo, nullptr, &m_vertexBuffer) != VK_SUCCESS) {
        qFatal("Failed to create vertex buffer.");
    }

    // 获取内存需求
    VkMemoryRequirements memRequirements;
    devFuncs->vkGetBufferMemoryRequirements(m_window->device(), m_vertexBuffer, &memRequirements);

    // 分配内存
    VkMemoryAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = m_window->hostVisibleMemoryIndex();

    if (devFuncs->vkAllocateMemory(m_window->device(), &allocInfo, nullptr, &m_vertexBufferMemory) != VK_SUCCESS) {
        qFatal("Failed to allocate vertex buffer memory.");
    }

    // 绑定内存
    devFuncs->vkBindBufferMemory(m_window->device(), m_vertexBuffer, m_vertexBufferMemory, 0);

    // 填充顶点数据
    void *data;
    devFuncs->vkMapMemory(m_window->device(), m_vertexBufferMemory, 0, bufferInfo.size, 0, &data);
    memcpy(data, vertices, (size_t)bufferInfo.size);
    devFuncs->vkUnmapMemory(m_window->device(), m_vertexBufferMemory);

    // 创建图形管线
    VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;

    if (devFuncs->vkCreatePipelineLayout(m_window->device(), &pipelineLayoutInfo, nullptr, &m_pipelineLayout) != VK_SUCCESS) {
        qFatal("Failed to create pipeline layout.");
    }

    // 管线配置（着色器模块等）
    // 需要加载编译好的 SPIR-V 着色器文件（vertex 和 fragment shader）
    // 示例中略去详细加载代码
}

void VulkanRenderer::initSwapChainResources()
{

}

void VulkanRenderer::releaseSwapChainResources()
{

}

void VulkanRenderer::releaseResources()
{
    QVulkanDeviceFunctions *devFuncs = m_window->vulkanInstance()->deviceFunctions(m_window->device());

    if (m_pipeline != VK_NULL_HANDLE) {
        devFuncs->vkDestroyPipeline(m_window->device(), m_pipeline, nullptr);
        m_pipeline = VK_NULL_HANDLE;
    }

    if (m_pipelineLayout != VK_NULL_HANDLE) {
        devFuncs->vkDestroyPipelineLayout(m_window->device(), m_pipelineLayout, nullptr);
        m_pipelineLayout = VK_NULL_HANDLE;
    }

    if (m_vertexBuffer != VK_NULL_HANDLE) {
        devFuncs->vkDestroyBuffer(m_window->device(), m_vertexBuffer, nullptr);
        m_vertexBuffer = VK_NULL_HANDLE;
    }

    if (m_vertexBufferMemory != VK_NULL_HANDLE) {
        devFuncs->vkFreeMemory(m_window->device(), m_vertexBufferMemory, nullptr);
        m_vertexBufferMemory = VK_NULL_HANDLE;
    }
}

void VulkanRenderer::startNextFrame()
{
    QVulkanDeviceFunctions *devFuncs = m_window->vulkanInstance()->deviceFunctions(m_window->device());
    VkCommandBuffer cmdBuffer = m_window->currentCommandBuffer();

    // 开始录制命令缓冲区
    VkCommandBufferBeginInfo beginInfo = {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    if (devFuncs->vkBeginCommandBuffer(cmdBuffer, &beginInfo) != VK_SUCCESS) {
        qFatal("Failed to begin recording command buffer.");
    }

    // 设置视口
    VkViewport viewport = {};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float)m_window->swapChainImageSize().width();
    viewport.height = (float)m_window->swapChainImageSize().height();
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    devFuncs->vkCmdSetViewport(cmdBuffer, 0, 1, &viewport);

    // 设置剪裁矩形
    VkRect2D scissor = {};
    scissor.offset = {0, 0};
    uint32_t width = m_window->swapChainImageSize().width();
    uint32_t height = m_window->swapChainImageSize().height();
    scissor.extent = {width,height};

    devFuncs->vkCmdSetScissor(cmdBuffer, 0, 1, &scissor);

    // 绑定图形管线
    devFuncs->vkCmdBindPipeline(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline);

    // 绑定顶点缓冲区
    VkDeviceSize offsets[] = {0};
    devFuncs->vkCmdBindVertexBuffers(cmdBuffer, 0, 1, &m_vertexBuffer, offsets);

    // 绘制三角形
    devFuncs->vkCmdDraw(cmdBuffer, 3, 1, 0, 0);

    // 结束命令录制
    if (devFuncs->vkEndCommandBuffer(cmdBuffer) != VK_SUCCESS) {
        qFatal("Failed to record command buffer.");
    }

    // 通知 QVulkanWindow 帧准备完成
    m_window->frameReady();
    m_window->requestUpdate();
}

