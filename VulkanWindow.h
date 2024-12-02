#ifndef VULKANWINDOW_H
#define VULKANWINDOW_H

#include <QVulkanWindow>
#include "VulkanRenderer.h"

class VulkanWindow : public QVulkanWindow
{
public:
    virtual QVulkanWindowRenderer *createRenderer() override;
};

#endif // VULKANWINDOW_H
