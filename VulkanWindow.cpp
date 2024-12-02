#include "VulkanWindow.h"
#include "VulkanRenderer.h"

VulkanWindow::VulkanWindow(QVulkanInstance *instance,QWindow *parent):
    QVulkanWindow(parent),
    _vulkanInstance(instance)
{

    this->setVulkanInstance(_vulkanInstance);
}

VulkanWindow::~VulkanWindow()
{

}

QVulkanWindowRenderer* VulkanWindow::createRenderer() {
    return new VulkanRenderer(this);
}
