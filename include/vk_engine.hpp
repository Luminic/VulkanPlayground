#ifndef VK_ENGINE_HPP
#define VK_ENGINE_HPP

#include "vk_types.hpp"
#include "vk_initializers.hpp"

#include <unordered_map>


class VulkanEngine {
public:
    VulkanEngine();
    ~VulkanEngine();

    enum class InitializationState {
        Uninitialized,
        FailedVulkanObjectsInitialization,
        VulkanObjectsInitialized,
        FailedSwapChainInitialization,
        Initialized // Swapchain initialized
    };

    // Returns `true` if initialization succeeds and `false` otherwise
    InitializationState init();
    void run();
    void cleanup();

    InitializationState get_initialization_state();

private:
    InitializationState initialization_state{ InitializationState::Uninitialized };
	uint64_t frame_number{0};

    // Updated in `init_swapchain`; should always be correct
	vk::Extent2D window_extent{ 1700, 900 };

    // Initialized in `init`

    struct SDL_Window* window{ nullptr };

    // Initialized in `init_vulkan_resources`

    vk::Instance instance;
    vk::SurfaceKHR surface;
    vk::PhysicalDevice chosen_gpu;
    vk::Device device;

    // After `select_physical_device`, the `bool` represents if the extension is enabled
    // Before, the `bool` replresents if the extension is required
    // Not all possible extensions will be in `extensions`; only the requested ones
    std::unordered_map<std::string, bool> extensions;
    std::unordered_map<std::string, bool> device_extensions;
    
    vk_init::GPUProperties gpu_properties;

    // Just an alias for `gpu_properties.graphics_present_queue_family`
    uint32_t& graphics_queue_family{ gpu_properties.graphics_present_queue_family };
    vk::Queue graphics_queue;

    // Initialized in `init_command_pool`

    vk::CommandPool command_pool;

    // Initialized in `choose_surface_format`

    vk::SurfaceFormatKHR surface_format;
    vk::PresentModeKHR present_mode;

    // Initialized in `init_default_renderpass`

    vk::RenderPass render_pass;

    // Initialized in `init_pipelines`

    vk::PipelineLayout triangle_pipeline_layout;
    vk::Pipeline triangle_pipeline;

    // Initialized in `init_command_buffers`

    vk::CommandBuffer main_command_buffer;

    // Initialized in `init_swapchain`

    vk::SwapchainKHR swap_chain;

    // Initialized in `init_framebuffers`

    uint32_t image_count{0};
    std::vector<vk::Image> swap_chain_images;
	std::vector<vk::ImageView> swap_chain_image_views;
    std::vector<vk::Framebuffer> framebuffers;

    // Initialized in `init_sync_structures`

    vk::Fence render_fence;
    vk::Semaphore present_semaphore, render_semaphore;


    bool init_vulkan_resources();
    void cleanup_vulkan_resources();
    bool init_swapchain_resources();
    void cleanup_swapchain_resources();

    bool init_command_pool();
    bool choose_surface_format();
    bool init_default_renderpass();
    bool init_pipelines();

    bool init_command_buffers();
    bool init_swapchain();
    bool init_framebuffers();
    bool init_sync_structures();


    void draw();
};

#endif
