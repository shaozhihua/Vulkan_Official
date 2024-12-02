#include "MainWindow.h"
#include "./ui_MainWindow.h"
#include <QVulkanInstance>
#include "VulkanWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QVulkanInstance *vulkanInstance = new QVulkanInstance();
    if (!vulkanInstance->create()) {
        qFatal("Failed to create Vulkan instance.");
    }

    // 创建自定义的 Vulkan 窗口
    VulkanWindow *vulkanWindow = new VulkanWindow(vulkanInstance);
    // 将 Vulkan 窗口嵌入到 QWidget 中
    ui->vulkanWidget = QWidget::createWindowContainer(vulkanWindow, this);






}

MainWindow::~MainWindow()
{
    delete ui;
}
