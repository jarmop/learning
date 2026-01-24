#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define WIDTH 800
#define HEIGHT 600

#define VK_CHECK(x) do { \
    VkResult err = (x); \
    if (err != VK_SUCCESS) { \
        fprintf(stderr, "Vulkan error %d at %s:%d\n", err, __FILE__, __LINE__); \
        exit(1); \
    } \
} while(0)

static int framebufferResized = 0;

static void framebuffer_resize_cb(GLFWwindow* window, int w, int h) {
    (void)window; (void)w; (void)h;
    framebufferResized = 1;
}

static uint8_t* read_file(const char* path, size_t* out_size) {
    FILE* f = fopen(path, "rb");
    if (!f) { perror(path); exit(1); }
    fseek(f, 0, SEEK_END);
    long sz = ftell(f);
    fseek(f, 0, SEEK_SET);
    if (sz <= 0) { fprintf(stderr, "Empty file: %s\n", path); exit(1); }
    uint8_t* buf = (uint8_t*)malloc((size_t)sz);
    if (!buf) { fprintf(stderr, "OOM\n"); exit(1); }
    if (fread(buf, 1, (size_t)sz, f) != (size_t)sz) { fprintf(stderr, "Read failed\n"); exit(1); }
    fclose(f);
    *out_size = (size_t)sz;
    return buf;
}

/* ---------------- Vulkan state ---------------- */

typedef struct {
    GLFWwindow* window;

    VkInstance instance;
    VkSurfaceKHR surface;

    VkPhysicalDevice physical;
    VkDevice device;

    uint32_t graphicsFamily;
    uint32_t presentFamily;
    VkQueue graphicsQueue;
    VkQueue presentQueue;

    VkSwapchainKHR swapchain;
    VkFormat swapchainFormat;
    VkExtent2D swapchainExtent;

    uint32_t swapchainImageCount;
    VkImage* swapchainImages;
    VkImageView* swapchainImageViews;

    VkRenderPass renderPass;
    VkPipelineLayout pipelineLayout;
    VkPipeline pipeline;

    VkFramebuffer* framebuffers;

    VkCommandPool commandPool;
    VkCommandBuffer* commandBuffers;

    VkSemaphore imageAvailable[2];
    VkSemaphore renderFinished[2];
    VkFence inFlight[2];
    uint32_t frameIndex;
} App;

/* ---------------- Helpers ---------------- */

static int has_extension(const char* name, const VkExtensionProperties* exts, uint32_t n) {
    for (uint32_t i = 0; i < n; i++) {
        if (strcmp(name, exts[i].extensionName) == 0) return 1;
    }
    return 0;
}

static void pick_physical_device(App* a) {
    uint32_t n = 0;
    VK_CHECK(vkEnumeratePhysicalDevices(a->instance, &n, NULL));
    if (n == 0) { fprintf(stderr, "No Vulkan physical devices found\n"); exit(1); }
    VkPhysicalDevice* devs = (VkPhysicalDevice*)malloc(sizeof(VkPhysicalDevice) * n);
    VK_CHECK(vkEnumeratePhysicalDevices(a->instance, &n, devs));

    // Pick first device that supports graphics + present + swapchain.
    for (uint32_t i = 0; i < n; i++) {
        VkPhysicalDevice d = devs[i];

        // Check swapchain extension
        uint32_t extN = 0;
        VK_CHECK(vkEnumerateDeviceExtensionProperties(d, NULL, &extN, NULL));
        VkExtensionProperties* exts = (VkExtensionProperties*)malloc(sizeof(VkExtensionProperties) * extN);
        VK_CHECK(vkEnumerateDeviceExtensionProperties(d, NULL, &extN, exts));

        int okExt = has_extension(VK_KHR_SWAPCHAIN_EXTENSION_NAME, exts, extN);
        free(exts);
        if (!okExt) continue;

        // Find queue families that do graphics + present
        uint32_t qn = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(d, &qn, NULL);
        VkQueueFamilyProperties* qprops = (VkQueueFamilyProperties*)malloc(sizeof(VkQueueFamilyProperties) * qn);
        vkGetPhysicalDeviceQueueFamilyProperties(d, &qn, qprops);

        int foundG = 0, foundP = 0;
        uint32_t gf = 0, pf = 0;

        for (uint32_t q = 0; q < qn; q++) {
            if (qprops[q].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                foundG = 1; gf = q;
            }
            VkBool32 present = VK_FALSE;
            VK_CHECK(vkGetPhysicalDeviceSurfaceSupportKHR(d, q, a->surface, &present));
            if (present) {
                foundP = 1; pf = q;
            }
        }
        free(qprops);

        if (foundG && foundP) {
            a->physical = d;
            a->graphicsFamily = gf;
            a->presentFamily = pf;
            free(devs);
            return;
        }
    }

    free(devs);
    fprintf(stderr, "No suitable GPU found (needs graphics+present+swapchain)\n");
    exit(1);
}

static VkSurfaceFormatKHR choose_surface_format(VkSurfaceFormatKHR* formats, uint32_t n) {
    // Prefer SRGB if available
    for (uint32_t i = 0; i < n; i++) {
        if (formats[i].format == VK_FORMAT_B8G8R8A8_SRGB &&
            formats[i].colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            return formats[i];
        }
    }
    return formats[0];
}

static VkPresentModeKHR choose_present_mode(VkPresentModeKHR* modes, uint32_t n) {
    // MAILBOX if available, else FIFO (always available)
    for (uint32_t i = 0; i < n; i++) {
        if (modes[i] == VK_PRESENT_MODE_MAILBOX_KHR) return modes[i];
    }
    return VK_PRESENT_MODE_FIFO_KHR;
}

static VkExtent2D choose_extent(App* a, const VkSurfaceCapabilitiesKHR* caps) {
    if (caps->currentExtent.width != UINT32_MAX) return caps->currentExtent;

    int w, h;
    glfwGetFramebufferSize(a->window, &w, &h);
    VkExtent2D e = { (uint32_t)w, (uint32_t)h };

    if (e.width < caps->minImageExtent.width) e.width = caps->minImageExtent.width;
    if (e.width > caps->maxImageExtent.width) e.width = caps->maxImageExtent.width;
    if (e.height < caps->minImageExtent.height) e.height = caps->minImageExtent.height;
    if (e.height > caps->maxImageExtent.height) e.height = caps->maxImageExtent.height;
    return e;
}

static void create_device(App* a) {
    float prio = 1.0f;

    VkDeviceQueueCreateInfo qci[2];
    memset(qci, 0, sizeof(qci));

    uint32_t uniqueCount = 0;
    uint32_t families[2] = { a->graphicsFamily, a->presentFamily };
    if (a->graphicsFamily == a->presentFamily) {
        uniqueCount = 1;
        qci[0] = (VkDeviceQueueCreateInfo){
            .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
            .queueFamilyIndex = a->graphicsFamily,
            .queueCount = 1,
            .pQueuePriorities = &prio
        };
    } else {
        uniqueCount = 2;
        qci[0] = (VkDeviceQueueCreateInfo){
            .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
            .queueFamilyIndex = families[0],
            .queueCount = 1,
            .pQueuePriorities = &prio
        };
        qci[1] = (VkDeviceQueueCreateInfo){
            .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
            .queueFamilyIndex = families[1],
            .queueCount = 1,
            .pQueuePriorities = &prio
        };
    }

    const char* devExts[] = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

    VkDeviceCreateInfo dci = {
        .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        .queueCreateInfoCount = uniqueCount,
        .pQueueCreateInfos = qci,
        .enabledExtensionCount = 1,
        .ppEnabledExtensionNames = devExts
    };

    VK_CHECK(vkCreateDevice(a->physical, &dci, NULL, &a->device));
    vkGetDeviceQueue(a->device, a->graphicsFamily, 0, &a->graphicsQueue);
    vkGetDeviceQueue(a->device, a->presentFamily, 0, &a->presentQueue);
}

static void destroy_swapchain(App* a) {
    if (a->commandBuffers) {
        vkFreeCommandBuffers(a->device, a->commandPool, a->swapchainImageCount, a->commandBuffers);
        free(a->commandBuffers);
        a->commandBuffers = NULL;
    }

    if (a->framebuffers) {
        for (uint32_t i = 0; i < a->swapchainImageCount; i++) {
            vkDestroyFramebuffer(a->device, a->framebuffers[i], NULL);
        }
        free(a->framebuffers);
        a->framebuffers = NULL;
    }

    if (a->pipeline) vkDestroyPipeline(a->device, a->pipeline, NULL);
    if (a->pipelineLayout) vkDestroyPipelineLayout(a->device, a->pipelineLayout, NULL);
    if (a->renderPass) vkDestroyRenderPass(a->device, a->renderPass, NULL);

    if (a->swapchainImageViews) {
        for (uint32_t i = 0; i < a->swapchainImageCount; i++) {
            vkDestroyImageView(a->device, a->swapchainImageViews[i], NULL);
        }
        free(a->swapchainImageViews);
        a->swapchainImageViews = NULL;
    }

    if (a->swapchainImages) {
        free(a->swapchainImages);
        a->swapchainImages = NULL;
    }

    if (a->swapchain) {
        vkDestroySwapchainKHR(a->device, a->swapchain, NULL);
        a->swapchain = VK_NULL_HANDLE;
    }
}

static VkShaderModule create_shader_module(App* a, const char* path) {
    size_t sz = 0;
    uint8_t* bytes = read_file(path, &sz);

    VkShaderModuleCreateInfo smci = {
        .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
        .codeSize = sz,
        .pCode = (const uint32_t*)bytes
    };

    VkShaderModule mod;
    VK_CHECK(vkCreateShaderModule(a->device, &smci, NULL, &mod));
    free(bytes);
    return mod;
}

static void create_renderpass(App* a) {
    VkAttachmentDescription color = {
        .format = a->swapchainFormat,
        .samples = VK_SAMPLE_COUNT_1_BIT,
        .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
        .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
        .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
        .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
        .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
        .finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
    };

    VkAttachmentReference colorRef = {
        .attachment = 0,
        .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
    };

    VkSubpassDescription subpass = {
        .pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
        .colorAttachmentCount = 1,
        .pColorAttachments = &colorRef
    };

    VkSubpassDependency dep = {
        .srcSubpass = VK_SUBPASS_EXTERNAL,
        .dstSubpass = 0,
        .srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
        .dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
        .dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT
    };

    VkRenderPassCreateInfo rpci = {
        .sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
        .attachmentCount = 1,
        .pAttachments = &color,
        .subpassCount = 1,
        .pSubpasses = &subpass,
        .dependencyCount = 1,
        .pDependencies = &dep
    };

    VK_CHECK(vkCreateRenderPass(a->device, &rpci, NULL, &a->renderPass));
}

static void create_pipeline(App* a) {
    VkShaderModule vert = create_shader_module(a, "shaders/vert.spv");
    VkShaderModule frag = create_shader_module(a, "shaders/frag.spv");

    VkPipelineShaderStageCreateInfo stages[2] = {
        {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
            .stage = VK_SHADER_STAGE_VERTEX_BIT,
            .module = vert,
            .pName = "main"
        },
        {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
            .stage = VK_SHADER_STAGE_FRAGMENT_BIT,
            .module = frag,
            .pName = "main"
        }
    };

    VkPipelineVertexInputStateCreateInfo vi = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
        // No vertex buffers: we use gl_VertexIndex in the vertex shader
        .vertexBindingDescriptionCount = 0,
        .vertexAttributeDescriptionCount = 0
    };

    VkPipelineInputAssemblyStateCreateInfo ia = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
        .topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
        .primitiveRestartEnable = VK_FALSE
    };

    VkViewport vp = {
        .x = 0.0f, .y = 0.0f,
        .width = (float)a->swapchainExtent.width,
        .height = (float)a->swapchainExtent.height,
        .minDepth = 0.0f, .maxDepth = 1.0f
    };

    VkRect2D sc = { .offset = {0,0}, .extent = a->swapchainExtent };

    VkPipelineViewportStateCreateInfo vs = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
        .viewportCount = 1,
        .pViewports = &vp,
        .scissorCount = 1,
        .pScissors = &sc
    };

    VkPipelineRasterizationStateCreateInfo rs = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
        .depthClampEnable = VK_FALSE,
        .rasterizerDiscardEnable = VK_FALSE,
        .polygonMode = VK_POLYGON_MODE_FILL,
        .cullMode = VK_CULL_MODE_BACK_BIT,
        .frontFace = VK_FRONT_FACE_CLOCKWISE, // NOTE: GLFW uses inverted Y compared to some tutorials; if triangle disappears, flip to COUNTER_CLOCKWISE
        .lineWidth = 1.0f
    };

    VkPipelineMultisampleStateCreateInfo ms = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
        .rasterizationSamples = VK_SAMPLE_COUNT_1_BIT
    };

    VkPipelineColorBlendAttachmentState cbAttach = {
        .blendEnable = VK_FALSE,
        .colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
                          VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT
    };

    VkPipelineColorBlendStateCreateInfo cb = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
        .logicOpEnable = VK_FALSE,
        .attachmentCount = 1,
        .pAttachments = &cbAttach
    };

    VkPipelineLayoutCreateInfo plci = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO
    };
    VK_CHECK(vkCreatePipelineLayout(a->device, &plci, NULL, &a->pipelineLayout));

    VkGraphicsPipelineCreateInfo gpci = {
        .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
        .stageCount = 2,
        .pStages = stages,
        .pVertexInputState = &vi,
        .pInputAssemblyState = &ia,
        .pViewportState = &vs,
        .pRasterizationState = &rs,
        .pMultisampleState = &ms,
        .pColorBlendState = &cb,
        .layout = a->pipelineLayout,
        .renderPass = a->renderPass,
        .subpass = 0
    };

    VK_CHECK(vkCreateGraphicsPipelines(a->device, VK_NULL_HANDLE, 1, &gpci, NULL, &a->pipeline));

    vkDestroyShaderModule(a->device, frag, NULL);
    vkDestroyShaderModule(a->device, vert, NULL);
}

static void create_framebuffers(App* a) {
    a->framebuffers = (VkFramebuffer*)malloc(sizeof(VkFramebuffer) * a->swapchainImageCount);

    for (uint32_t i = 0; i < a->swapchainImageCount; i++) {
        VkImageView attachments[] = { a->swapchainImageViews[i] };

        VkFramebufferCreateInfo fbci = {
            .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
            .renderPass = a->renderPass,
            .attachmentCount = 1,
            .pAttachments = attachments,
            .width = a->swapchainExtent.width,
            .height = a->swapchainExtent.height,
            .layers = 1
        };

        VK_CHECK(vkCreateFramebuffer(a->device, &fbci, NULL, &a->framebuffers[i]));
    }
}

static void create_command_pool(App* a) {
    VkCommandPoolCreateInfo cpci = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
        .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
        .queueFamilyIndex = a->graphicsFamily
    };
    VK_CHECK(vkCreateCommandPool(a->device, &cpci, NULL, &a->commandPool));
}

static void record_command_buffers(App* a) {
    a->commandBuffers = (VkCommandBuffer*)malloc(sizeof(VkCommandBuffer) * a->swapchainImageCount);

    VkCommandBufferAllocateInfo cbai = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .commandPool = a->commandPool,
        .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandBufferCount = a->swapchainImageCount
    };
    VK_CHECK(vkAllocateCommandBuffers(a->device, &cbai, a->commandBuffers));

    for (uint32_t i = 0; i < a->swapchainImageCount; i++) {
        VkCommandBufferBeginInfo bi = {
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO
        };
        VK_CHECK(vkBeginCommandBuffer(a->commandBuffers[i], &bi));

        VkClearValue clear;
        clear.color.float32[0] = 0.05f;
        clear.color.float32[1] = 0.05f;
        clear.color.float32[2] = 0.10f;
        clear.color.float32[3] = 1.0f;

        VkRenderPassBeginInfo rpbi = {
            .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
            .renderPass = a->renderPass,
            .framebuffer = a->framebuffers[i],
            .renderArea = { {0,0}, a->swapchainExtent },
            .clearValueCount = 1,
            .pClearValues = &clear
        };

        vkCmdBeginRenderPass(a->commandBuffers[i], &rpbi, VK_SUBPASS_CONTENTS_INLINE);
        vkCmdBindPipeline(a->commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, a->pipeline);
        vkCmdDraw(a->commandBuffers[i], 3, 1, 0, 0);
        vkCmdEndRenderPass(a->commandBuffers[i]);

        VK_CHECK(vkEndCommandBuffer(a->commandBuffers[i]));
    }
}

static void create_swapchain(App* a) {
    VkSurfaceCapabilitiesKHR caps;
    VK_CHECK(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(a->physical, a->surface, &caps));

    uint32_t fmtN = 0;
    VK_CHECK(vkGetPhysicalDeviceSurfaceFormatsKHR(a->physical, a->surface, &fmtN, NULL));
    VkSurfaceFormatKHR* fmts = (VkSurfaceFormatKHR*)malloc(sizeof(VkSurfaceFormatKHR) * fmtN);
    VK_CHECK(vkGetPhysicalDeviceSurfaceFormatsKHR(a->physical, a->surface, &fmtN, fmts));
    VkSurfaceFormatKHR fmt = choose_surface_format(fmts, fmtN);
    free(fmts);

    uint32_t pmN = 0;
    VK_CHECK(vkGetPhysicalDeviceSurfacePresentModesKHR(a->physical, a->surface, &pmN, NULL));
    VkPresentModeKHR* pms = (VkPresentModeKHR*)malloc(sizeof(VkPresentModeKHR) * pmN);
    VK_CHECK(vkGetPhysicalDeviceSurfacePresentModesKHR(a->physical, a->surface, &pmN, pms));
    VkPresentModeKHR pm = choose_present_mode(pms, pmN);
    free(pms);

    VkExtent2D extent = choose_extent(a, &caps);

    uint32_t imageCount = caps.minImageCount + 1;
    if (caps.maxImageCount > 0 && imageCount > caps.maxImageCount) imageCount = caps.maxImageCount;

    VkSwapchainCreateInfoKHR sci = {
        .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        .surface = a->surface,
        .minImageCount = imageCount,
        .imageFormat = fmt.format,
        .imageColorSpace = fmt.colorSpace,
        .imageExtent = extent,
        .imageArrayLayers = 1,
        .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
        .preTransform = caps.currentTransform,
        .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
        .presentMode = pm,
        .clipped = VK_TRUE,
        .oldSwapchain = VK_NULL_HANDLE
    };

    uint32_t qidx[2] = { a->graphicsFamily, a->presentFamily };
    if (a->graphicsFamily != a->presentFamily) {
        sci.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        sci.queueFamilyIndexCount = 2;
        sci.pQueueFamilyIndices = qidx;
    } else {
        sci.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    }

    VK_CHECK(vkCreateSwapchainKHR(a->device, &sci, NULL, &a->swapchain));
    a->swapchainFormat = fmt.format;
    a->swapchainExtent = extent;

    VK_CHECK(vkGetSwapchainImagesKHR(a->device, a->swapchain, &a->swapchainImageCount, NULL));
    a->swapchainImages = (VkImage*)malloc(sizeof(VkImage) * a->swapchainImageCount);
    VK_CHECK(vkGetSwapchainImagesKHR(a->device, a->swapchain, &a->swapchainImageCount, a->swapchainImages));

    a->swapchainImageViews = (VkImageView*)malloc(sizeof(VkImageView) * a->swapchainImageCount);
    for (uint32_t i = 0; i < a->swapchainImageCount; i++) {
        VkImageViewCreateInfo ivci = {
            .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
            .image = a->swapchainImages[i],
            .viewType = VK_IMAGE_VIEW_TYPE_2D,
            .format = a->swapchainFormat,
            .components = { VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY,
                            VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY },
            .subresourceRange = {
                .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                .baseMipLevel = 0,
                .levelCount = 1,
                .baseArrayLayer = 0,
                .layerCount = 1
            }
        };
        VK_CHECK(vkCreateImageView(a->device, &ivci, NULL, &a->swapchainImageViews[i]));
    }

    create_renderpass(a);
    create_pipeline(a);
    create_framebuffers(a);
    record_command_buffers(a);
}

static void recreate_swapchain(App* a) {
    int w = 0, h = 0;
    glfwGetFramebufferSize(a->window, &w, &h);
    while (w == 0 || h == 0) {
        glfwWaitEvents();
        glfwGetFramebufferSize(a->window, &w, &h);
    }

    vkDeviceWaitIdle(a->device);
    destroy_swapchain(a);
    create_swapchain(a);
}

static void create_sync(App* a) {
    VkSemaphoreCreateInfo sci = { .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO };
    VkFenceCreateInfo fci = { .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO, .flags = VK_FENCE_CREATE_SIGNALED_BIT };

    for (int i = 0; i < 2; i++) {
        VK_CHECK(vkCreateSemaphore(a->device, &sci, NULL, &a->imageAvailable[i]));
        VK_CHECK(vkCreateSemaphore(a->device, &sci, NULL, &a->renderFinished[i]));
        VK_CHECK(vkCreateFence(a->device, &fci, NULL, &a->inFlight[i]));
    }
    a->frameIndex = 0;
}

static void draw_frame(App* a) {
    uint32_t fi = a->frameIndex;

    VK_CHECK(vkWaitForFences(a->device, 1, &a->inFlight[fi], VK_TRUE, UINT64_MAX));
    VK_CHECK(vkResetFences(a->device, 1, &a->inFlight[fi]));

    uint32_t imageIndex = 0;
    VkResult acq = vkAcquireNextImageKHR(a->device, a->swapchain, UINT64_MAX,
                                        a->imageAvailable[fi], VK_NULL_HANDLE, &imageIndex);

    if (acq == VK_ERROR_OUT_OF_DATE_KHR) {
        recreate_swapchain(a);
        return;
    }
    VK_CHECK(acq);

    VkPipelineStageFlags waitStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

    VkSubmitInfo si = {
        .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        .waitSemaphoreCount = 1,
        .pWaitSemaphores = &a->imageAvailable[fi],
        .pWaitDstStageMask = &waitStage,
        .commandBufferCount = 1,
        .pCommandBuffers = &a->commandBuffers[imageIndex],
        .signalSemaphoreCount = 1,
        .pSignalSemaphores = &a->renderFinished[fi]
    };

    VK_CHECK(vkQueueSubmit(a->graphicsQueue, 1, &si, a->inFlight[fi]));

    VkPresentInfoKHR pi = {
        .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
        .waitSemaphoreCount = 1,
        .pWaitSemaphores = &a->renderFinished[fi],
        .swapchainCount = 1,
        .pSwapchains = &a->swapchain,
        .pImageIndices = &imageIndex
    };

    VkResult pres = vkQueuePresentKHR(a->presentQueue, &pi);
    if (pres == VK_ERROR_OUT_OF_DATE_KHR || pres == VK_SUBOPTIMAL_KHR || framebufferResized) {
        framebufferResized = 0;
        recreate_swapchain(a);
    } else {
        VK_CHECK(pres);
    }

    a->frameIndex = (a->frameIndex + 1) % 2;
}

/* ---------------- Main ---------------- */

int main(void) {
    App a;
    memset(&a, 0, sizeof(a));

    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    a.window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan Triangle", NULL, NULL);
    glfwSetFramebufferSizeCallback(a.window, framebuffer_resize_cb);

    // Instance
    VkApplicationInfo app = {
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pApplicationName = "Vulkan Triangle",
        .applicationVersion = VK_MAKE_VERSION(1,0,0),
        .pEngineName = "none",
        .engineVersion = VK_MAKE_VERSION(1,0,0),
        .apiVersion = VK_API_VERSION_1_0
    };

    uint32_t extCount = 0;
    const char** exts = glfwGetRequiredInstanceExtensions(&extCount);

    VkInstanceCreateInfo ici = {
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pApplicationInfo = &app,
        .enabledExtensionCount = extCount,
        .ppEnabledExtensionNames = exts
    };

    VK_CHECK(vkCreateInstance(&ici, NULL, &a.instance));
    VK_CHECK(glfwCreateWindowSurface(a.instance, a.window, NULL, &a.surface));

    pick_physical_device(&a);
    create_device(&a);
    create_command_pool(&a);
    create_swapchain(&a);
    create_sync(&a);

    while (!glfwWindowShouldClose(a.window)) {
        glfwPollEvents();
        draw_frame(&a);
    }

    vkDeviceWaitIdle(a.device);

    for (int i = 0; i < 2; i++) {
        vkDestroySemaphore(a.device, a.renderFinished[i], NULL);
        vkDestroySemaphore(a.device, a.imageAvailable[i], NULL);
        vkDestroyFence(a.device, a.inFlight[i], NULL);
    }

    destroy_swapchain(&a);
    vkDestroyCommandPool(a.device, a.commandPool, NULL);
    vkDestroyDevice(a.device, NULL);
    vkDestroySurfaceKHR(a.instance, a.surface, NULL);
    vkDestroyInstance(a.instance, NULL);

    glfwDestroyWindow(a.window);
    glfwTerminate();
    return 0;
}
