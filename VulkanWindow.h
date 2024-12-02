#ifndef VULKANWINDOW_H
#define VULKANWINDOW_H

#include <QVulkanWindow>
#include <QVulkanInstance>


class VulkanWindow : public QVulkanWindow
{
public:

    explicit VulkanWindow(QVulkanInstance *instance,QWindow *parent = nullptr);
    ~VulkanWindow();
    virtual QVulkanWindowRenderer *createRenderer() override;


private:
    QVulkanInstance *_vulkanInstance = nullptr;
};

#endif // VULKANWINDOW_H
