#include "VulkanWindow.h"

QVulkanWindowRenderer* VulkanWindow::createRenderer() {
    return new VulkanRenderer(this);
}
