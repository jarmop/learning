#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <limits>
#include <optional>
#include <stdexcept>
#include <vector>

// ---------- Embedded SPIR-V (very small passthrough shaders) ----------
// compiled from simple GLSL:
// vertex: positions from hardcoded array
// fragment: outputs color

const std::vector<uint32_t> VERT_SHADER = {
#include "vert_spv.inc"
};

const std::vector<uint32_t> FRAG_SHADER = {
#include "frag_spv.inc"
};

// ---------- Config ----------
const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;
const int MAX_FRAMES_IN_FLIGHT = 2;

// ---------- Helpers ----------
struct QueueFamilyIndices {
  std::optional<uint32_t> graphicsFamily;
  std::optional<uint32_t> presentFamily;

  bool complete() { return graphicsFamily.has_value() && presentFamily.has_value(); }
};

struct SwapChainSupport {
  VkSurfaceCapabilitiesKHR capabilities;
  std::vector<VkSurfaceFormatKHR> formats;
  std::vector<VkPresentModeKHR> modes;
};

// ---------- App ----------
class App {
 public:
  void run() {
    initWindow();
    initVulkan();
    loop();
    cleanup();
  }

 private:
  GLFWwindow* window;

  VkInstance instance;
  VkSurfaceKHR surface;

  VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
  VkDevice device;

  VkQueue graphicsQueue;
  VkQueue presentQueue;

  VkSwapchainKHR swapchain;
  std::vector<VkImage> images;
  std::vector<VkImageView> imageViews;
  VkFormat format;
  VkExtent2D extent;

  VkRenderPass renderPass;
  VkPipelineLayout pipelineLayout;
  VkPipeline pipeline;

  std::vector<VkFramebuffer> framebuffers;

  VkCommandPool commandPool;
  std::vector<VkCommandBuffer> cmdBuffers;

  std::vector<VkSemaphore> imageAvailable;
  std::vector<VkSemaphore> renderFinished;
  std::vector<VkFence> inFlight;

  size_t currentFrame = 0;

  // ---------- Init ----------
  void initWindow() {
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    window = glfwCreateWindow(WIDTH, HEIGHT, "Triangle", nullptr, nullptr);
  }

  void initVulkan() {
    createInstance();
    createSurface();
    pickDevice();
    createLogicalDevice();
    createSwapchain();
    createImageViews();
    createRenderPass();
    createPipeline();
    createFramebuffers();
    createCommandPool();
    createCommandBuffers();
    createSync();
  }

  // ---------- Core ----------
  void createInstance() {
    VkApplicationInfo app{VK_STRUCTURE_TYPE_APPLICATION_INFO};
    app.pApplicationName = "Triangle";
    app.apiVersion = VK_API_VERSION_1_3;

    uint32_t count;
    const char** ext = glfwGetRequiredInstanceExtensions(&count);

    VkInstanceCreateInfo info{VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO};
    info.pApplicationInfo = &app;
    info.enabledExtensionCount = count;
    info.ppEnabledExtensionNames = ext;

    if (vkCreateInstance(&info, nullptr, &instance) != VK_SUCCESS)
      throw std::runtime_error("instance");
  }

  void createSurface() { glfwCreateWindowSurface(instance, window, nullptr, &surface); }

  QueueFamilyIndices findQueues(VkPhysicalDevice dev) {
    QueueFamilyIndices idx;

    uint32_t count;
    vkGetPhysicalDeviceQueueFamilyProperties(dev, &count, nullptr);

    std::vector<VkQueueFamilyProperties> props(count);
    vkGetPhysicalDeviceQueueFamilyProperties(dev, &count, props.data());

    for (uint32_t i = 0; i < count; i++) {
      if (props[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) idx.graphicsFamily = i;

      VkBool32 present = false;
      vkGetPhysicalDeviceSurfaceSupportKHR(dev, i, surface, &present);
      if (present) idx.presentFamily = i;

      if (idx.complete()) break;
    }

    return idx;
  }

  void pickDevice() {
    uint32_t count;
    vkEnumeratePhysicalDevices(instance, &count, nullptr);

    std::vector<VkPhysicalDevice> devs(count);
    vkEnumeratePhysicalDevices(instance, &count, devs.data());

    for (auto d : devs) {
      if (findQueues(d).complete()) {
        physicalDevice = d;
        break;
      }
    }

    if (physicalDevice == VK_NULL_HANDLE) throw std::runtime_error("no device");
  }

  void createLogicalDevice() {
    QueueFamilyIndices idx = findQueues(physicalDevice);

    float prio = 1.0f;
    std::vector<VkDeviceQueueCreateInfo> queues;

    std::vector<uint32_t> unique = {idx.graphicsFamily.value(), idx.presentFamily.value()};

    for (uint32_t fam : unique) {
      VkDeviceQueueCreateInfo q{VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO};
      q.queueFamilyIndex = fam;
      q.queueCount = 1;
      q.pQueuePriorities = &prio;
      queues.push_back(q);
    }

    VkDeviceCreateInfo info{VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO};
    info.queueCreateInfoCount = queues.size();
    info.pQueueCreateInfos = queues.data();

    const char* exts[] = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
    info.enabledExtensionCount = 1;
    info.ppEnabledExtensionNames = exts;

    if (vkCreateDevice(physicalDevice, &info, nullptr, &device) != VK_SUCCESS)
      throw std::runtime_error("device");

    vkGetDeviceQueue(device, idx.graphicsFamily.value(), 0, &graphicsQueue);
    vkGetDeviceQueue(device, idx.presentFamily.value(), 0, &presentQueue);
  }

  SwapChainSupport querySwap(VkPhysicalDevice dev) {
    SwapChainSupport s;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(dev, surface, &s.capabilities);

    uint32_t count;
    vkGetPhysicalDeviceSurfaceFormatsKHR(dev, surface, &count, nullptr);
    s.formats.resize(count);
    vkGetPhysicalDeviceSurfaceFormatsKHR(dev, surface, &count, s.formats.data());

    vkGetPhysicalDevicePresentModesKHR(dev, surface, &count, nullptr);
    s.modes.resize(count);
    vkGetPhysicalDevicePresentModesKHR(dev, surface, &count, s.modes.data());

    return s;
  }

  void createSwapchain() {
    auto s = querySwap(physicalDevice);

    VkSurfaceFormatKHR fmt = s.formats[0];
    VkPresentModeKHR mode = VK_PRESENT_MODE_FIFO_KHR;

    extent = s.capabilities.currentExtent;
    format = fmt.format;

    uint32_t imageCount = s.capabilities.minImageCount + 1;

    VkSwapchainCreateInfoKHR info{VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR};
    info.surface = surface;
    info.minImageCount = imageCount;
    info.imageFormat = format;
    info.imageColorSpace = fmt.colorSpace;
    info.imageExtent = extent;
    info.imageArrayLayers = 1;
    info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    QueueFamilyIndices idx = findQueues(physicalDevice);
    uint32_t q[] = {idx.graphicsFamily.value(), idx.presentFamily.value()};

    if (idx.graphicsFamily != idx.presentFamily) {
      info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
      info.queueFamilyIndexCount = 2;
      info.pQueueFamilyIndices = q;
    } else {
      info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    }

    info.preTransform = s.capabilities.currentTransform;
    info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    info.presentMode = mode;
    info.clipped = VK_TRUE;

    vkCreateSwapchainKHR(device, &info, nullptr, &swapchain);

    vkGetSwapchainImagesKHR(device, swapchain, &imageCount, nullptr);
    images.resize(imageCount);
    vkGetSwapchainImagesKHR(device, swapchain, &imageCount, images.data());
  }

  void createImageViews() {
    imageViews.resize(images.size());

    for (size_t i = 0; i < images.size(); i++) {
      VkImageViewCreateInfo info{VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO};
      info.image = images[i];
      info.viewType = VK_IMAGE_VIEW_TYPE_2D;
      info.format = format;
      info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
      info.subresourceRange.levelCount = 1;
      info.subresourceRange.layerCount = 1;

      vkCreateImageView(device, &info, nullptr, &imageViews[i]);
    }
  }

  void createRenderPass() {
    VkAttachmentDescription color{};
    color.format = format;
    color.samples = VK_SAMPLE_COUNT_1_BIT;
    color.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    color.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    color.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference ref{};
    ref.attachment = 0;
    ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription sub{};
    sub.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    sub.colorAttachmentCount = 1;
    sub.pColorAttachments = &ref;

    VkRenderPassCreateInfo info{VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO};
    info.attachmentCount = 1;
    info.pAttachments = &color;
    info.subpassCount = 1;
    info.pSubpasses = &sub;

    vkCreateRenderPass(device, &info, nullptr, &renderPass);
  }

  VkShaderModule makeModule(const std::vector<uint32_t>& code) {
    VkShaderModuleCreateInfo info{VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO};
    info.codeSize = code.size() * sizeof(uint32_t);
    info.pCode = code.data();

    VkShaderModule mod;
    vkCreateShaderModule(device, &info, nullptr, &mod);
    return mod;
  }

  void createPipeline() {
    VkShaderModule vert = makeModule(VERT_SHADER);
    VkShaderModule frag = makeModule(FRAG_SHADER);

    VkPipelineShaderStageCreateInfo vs{VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO};
    vs.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vs.module = vert;
    vs.pName = "main";

    VkPipelineShaderStageCreateInfo fs{VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO};
    fs.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fs.module = frag;
    fs.pName = "main";

    VkPipelineShaderStageCreateInfo stages[] = {vs, fs};

    VkPipelineVertexInputStateCreateInfo vi{
        VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO};

    VkPipelineInputAssemblyStateCreateInfo ia{
        VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO};
    ia.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

    VkViewport vp{};
    vp.width = (float)extent.width;
    vp.height = (float)extent.height;
    vp.maxDepth = 1.0f;

    VkRect2D sc{};
    sc.extent = extent;

    VkPipelineViewportStateCreateInfo vpState{
        VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO};
    vpState.viewportCount = 1;
    vpState.pViewports = &vp;
    vpState.scissorCount = 1;
    vpState.pScissors = &sc;

    VkPipelineRasterizationStateCreateInfo rs{
        VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO};
    rs.polygonMode = VK_POLYGON_MODE_FILL;
    rs.lineWidth = 1.0f;
    rs.cullMode = VK_CULL_MODE_BACK_BIT;
    rs.frontFace = VK_FRONT_FACE_CLOCKWISE;

    VkPipelineMultisampleStateCreateInfo ms{
        VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO};
    ms.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

    VkPipelineColorBlendAttachmentState cb{};
    cb.colorWriteMask = 0xF;

    VkPipelineColorBlendStateCreateInfo cbState{
        VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO};
    cbState.attachmentCount = 1;
    cbState.pAttachments = &cb;

    VkPipelineLayoutCreateInfo layout{VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO};
    vkCreatePipelineLayout(device, &layout, nullptr, &pipelineLayout);

    VkGraphicsPipelineCreateInfo info{VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO};
    info.stageCount = 2;
    info.pStages = stages;
    info.pVertexInputState = &vi;
    info.pInputAssemblyState = &ia;
    info.pViewportState = &vpState;
    info.pRasterizationState = &rs;
    info.pMultisampleState = &ms;
    info.pColorBlendState = &cbState;
    info.layout = pipelineLayout;
    info.renderPass = renderPass;

    vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &info, nullptr, &pipeline);

    vkDestroyShaderModule(device, vert, nullptr);
    vkDestroyShaderModule(device, frag, nullptr);
  }

  void createFramebuffers() {
    framebuffers.resize(imageViews.size());

    for (size_t i = 0; i < imageViews.size(); i++) {
      VkImageView att[] = {imageViews[i]};

      VkFramebufferCreateInfo info{VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO};
      info.renderPass = renderPass;
      info.attachmentCount = 1;
      info.pAttachments = att;
      info.width = extent.width;
      info.height = extent.height;
      info.layers = 1;

      vkCreateFramebuffer(device, &info, nullptr, &framebuffers[i]);
    }
  }

  void createCommandPool() {
    QueueFamilyIndices idx = findQueues(physicalDevice);

    VkCommandPoolCreateInfo info{VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO};
    info.queueFamilyIndex = idx.graphicsFamily.value();

    vkCreateCommandPool(device, &info, nullptr, &commandPool);
  }

  void createCommandBuffers() {
    cmdBuffers.resize(framebuffers.size());

    VkCommandBufferAllocateInfo alloc{VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO};
    alloc.commandPool = commandPool;
    alloc.commandBufferCount = cmdBuffers.size();

    vkAllocateCommandBuffers(device, &alloc, cmdBuffers.data());

    for (size_t i = 0; i < cmdBuffers.size(); i++) {
      VkCommandBufferBeginInfo begin{VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO};
      vkBeginCommandBuffer(cmdBuffers[i], &begin);

      VkClearValue clear{{0.f, 0.f, 0.f, 1.f}};

      VkRenderPassBeginInfo rp{VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO};
      rp.renderPass = renderPass;
      rp.framebuffer = framebuffers[i];
      rp.renderArea.extent = extent;
      rp.clearValueCount = 1;
      rp.pClearValues = &clear;

      vkCmdBeginRenderPass(cmdBuffers[i], &rp, VK_SUBPASS_CONTENTS_INLINE);
      vkCmdBindPipeline(cmdBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
      vkCmdDraw(cmdBuffers[i], 3, 1, 0, 0);
      vkCmdEndRenderPass(cmdBuffers[i]);

      vkEndCommandBuffer(cmdBuffers[i]);
    }
  }

  void createSync() {
    imageAvailable.resize(MAX_FRAMES_IN_FLIGHT);
    renderFinished.resize(MAX_FRAMES_IN_FLIGHT);
    inFlight.resize(MAX_FRAMES_IN_FLIGHT);

    VkSemaphoreCreateInfo sem{VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO};
    VkFenceCreateInfo fence{VK_STRUCTURE_TYPE_FENCE_CREATE_INFO};
    fence.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
      vkCreateSemaphore(device, &sem, nullptr, &imageAvailable[i]);
      vkCreateSemaphore(device, &sem, nullptr, &renderFinished[i]);
      vkCreateFence(device, &fence, nullptr, &inFlight[i]);
    }
  }

  // ---------- Loop ----------
  void loop() {
    while (!glfwWindowShouldClose(window)) {
      glfwPollEvents();
      draw();
    }
    vkDeviceWaitIdle(device);
  }

  void draw() {
    vkWaitForFences(device, 1, &inFlight[currentFrame], VK_TRUE, UINT64_MAX);

    uint32_t imageIndex;
    vkAcquireNextImageKHR(device, swapchain, UINT64_MAX, imageAvailable[currentFrame],
                          VK_NULL_HANDLE, &imageIndex);

    vkResetFences(device, 1, &inFlight[currentFrame]);

    VkSemaphore waitSem[] = {imageAvailable[currentFrame]};
    VkPipelineStageFlags stages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};

    VkSubmitInfo submit{VK_STRUCTURE_TYPE_SUBMIT_INFO};
    submit.waitSemaphoreCount = 1;
    submit.pWaitSemaphores = waitSem;
    submit.pWaitDstStageMask = stages;
    submit.commandBufferCount = 1;
    submit.pCommandBuffers = &cmdBuffers[imageIndex];

    VkSemaphore signalSem[] = {renderFinished[currentFrame]};
    submit.signalSemaphoreCount = 1;
    submit.pSignalSemaphores = signalSem;

    vkQueueSubmit(graphicsQueue, 1, &submit, inFlight[currentFrame]);

    VkPresentInfoKHR present{VK_STRUCTURE_TYPE_PRESENT_INFO_KHR};
    present.waitSemaphoreCount = 1;
    present.pWaitSemaphores = signalSem;
    present.swapchainCount = 1;
    present.pSwapchains = &swapchain;
    present.pImageIndices = &imageIndex;

    vkQueuePresentKHR(presentQueue, &present);

    currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
  }

  // ---------- Cleanup ----------
  void cleanup() {
    for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
      vkDestroySemaphore(device, imageAvailable[i], nullptr);
      vkDestroySemaphore(device, renderFinished[i], nullptr);
      vkDestroyFence(device, inFlight[i], nullptr);
    }

    vkDestroyCommandPool(device, commandPool, nullptr);

    for (auto fb : framebuffers) vkDestroyFramebuffer(device, fb, nullptr);

    vkDestroyPipeline(device, pipeline, nullptr);
    vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
    vkDestroyRenderPass(device, renderPass, nullptr);

    for (auto v : imageViews) vkDestroyImageView(device, v, nullptr);

    vkDestroySwapchainKHR(device, swapchain, nullptr);
    vkDestroyDevice(device, nullptr);

    vkDestroySurfaceKHR(instance, surface, nullptr);
    vkDestroyInstance(instance, nullptr);

    glfwDestroyWindow(window);
    glfwTerminate();
  }
};

// ---------- main ----------
int main() {
  App app;

  try {
    app.run();
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}