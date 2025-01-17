#ifndef VULKANRENDERER_H
#define VULKANRENDERER_H

#include <QVulkanInstance>
#include <QVulkanWindow>
#include <QVector3D>
#include <vulkan/vulkan.h>

struct Vertex {
    QVector3D position;
    QVector3D color;
};


// 自定义 Vulkan 渲染器
class VulkanRenderer : public QVulkanWindowRenderer {
public:
    VulkanRenderer(QVulkanWindow *window);

    /// 初始化Vulkan资源
    virtual void initResources() override;

    /// 初始化交换链相关资源
    void initSwapChainResources() override;

    /// 释放交换链资源
    void releaseSwapChainResources() override;

    /// 释放Vulkan资源
    virtual void releaseResources() override;

    /// 下一帧的渲染
    virtual void startNextFrame() override;

private:
    QVulkanWindow       *m_window;
    VkInstance          m_instance = VK_NULL_HANDLE;
    VkDevice            m_device = VK_NULL_HANDLE;
    VkQueue             m_graphicsQueue = VK_NULL_HANDLE;
    VkCommandPool       m_commandPool = VK_NULL_HANDLE;
    VkPipeline          m_pipeline = VK_NULL_HANDLE;
    VkPipelineLayout    m_pipelineLayout = VK_NULL_HANDLE;
    VkBuffer            m_vertexBuffer = VK_NULL_HANDLE;
    VkDeviceMemory      m_vertexBufferMemory = VK_NULL_HANDLE;

};

#endif // VULKANRENDERER_H
