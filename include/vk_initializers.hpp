#ifndef VK_INITIALIZERS_HPP
#define VK_INITIALIZERS_HPP

#include "vk_types.hpp"

#define CHECK_VK_RESULT(vk_result, msg) do { \
        if ((vk_result) != vk::Result::eSuccess) { \
            std::cerr << (msg) << ": " << int(vk_result) << std::endl; \
        } \
    } while (0)

#define CHECK_VK_RESULT_RF(vk_result, msg) do { \
        if ((vk_result) != vk::Result::eSuccess) { \
            std::cerr << (msg) << ": " << int(vk_result) << std::endl; \
            return false; \
        } \
    } while (0)

namespace vk_init {
    // Returns every `const char*` in both `requested` and `supported`.
    // The ptr will point to the the keys in `requested`.
    // 
    // If `modify` is `true`, The values in `requested` will be set to
    // `true` or `false` depending on if it is or isn't in `supported`.
    std::pair<bool, std::vector<const char*>> get_requested_and_supported_extensions(
        std::unordered_map<std::string, bool>& requested,
        const std::vector<vk::ExtensionProperties>& supported,
        bool modify = true,
        std::optional<std::string> error_msg = {}
    );


    struct SwapChainSupportDetails {
        vk::SurfaceCapabilitiesKHR capabilities;
        std::vector<vk::SurfaceFormatKHR> formats;
        std::vector<vk::PresentModeKHR> present_modes;

        SwapChainSupportDetails(
            vk::SurfaceCapabilitiesKHR capabilities = {},
            std::vector<vk::SurfaceFormatKHR> formats = {},
            std::vector<vk::PresentModeKHR> present_modes = {}
        );

        SwapChainSupportDetails(vk::PhysicalDevice gpu, vk::SurfaceKHR surface);
    };


    struct GPUProperties {
        int score;

        std::vector<vk::ExtensionProperties> supported_device_extensions;
        std::vector<const char*> supported_requested_device_extensions;

        std::vector<vk::QueueFamilyProperties> supported_queue_families;
        uint32_t graphics_present_queue_family;

        SwapChainSupportDetails sw_ch_support;

        GPUProperties(
            int score = 0,
            std::vector<vk::ExtensionProperties> supported_device_extensions = {},
            std::vector<const char*> supported_requested_device_extensions = {},
            std::vector<vk::QueueFamilyProperties> supported_queue_families = {},
            uint32_t graphics_present_queue_family = UINT32_MAX,
            SwapChainSupportDetails sw_ch_support = {}
        );

        GPUProperties(
            vk::PhysicalDevice gpu,
            std::unordered_map<std::string, bool>& requested_device_extensions,
            vk::SurfaceKHR surface
        );
    };
}

#endif