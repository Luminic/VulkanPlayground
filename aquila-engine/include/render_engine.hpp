#ifndef AQUILA_RENDER_ENGINE_HPP
#define AQUILA_RENDER_ENGINE_HPP

#include <array>
#include <memory>

#include <glm/glm.hpp>

#include "init_engine.hpp"
#include "util/vk_types.hpp"
#include "scene/aq_camera.hpp"
#include "scene/aq_mesh.hpp"
#include "scene/aq_node.hpp"
#include "scene/aq_texture.hpp"
#include "scene/aq_material.hpp"

namespace aq {

    struct PushConstants {
        glm::mat4 model;
        uint material_index;
    };

    struct GPUCameraData {
        glm::mat4 view_projection;
    };

    class RenderEngine : public InitializationEngine {
    public:
        RenderEngine();
        virtual ~RenderEngine();

        void update();
        void draw(AbstractCamera* camera, std::shared_ptr<Node> object_hierarchy);

        glm::ivec2 get_render_window_size() const {return {window_extent.width, window_extent.height};};

        uint64_t get_frame_number() const {return frame_number;}

        MaterialManager material_manager;

    protected:
        virtual bool init_render_resources() override;
        // If redefined in child class, either `RenderEngine::cleanup_render_resources` MUST still be called 
        // or `init_render_resources` should also be redefined in order to not leak resources
        virtual void cleanup_render_resources() override;

        bool init_pipelines();
        vk::PipelineLayout triangle_pipeline_layout;
        vk::Pipeline triangle_pipeline;

        virtual bool resize_window() override; // Calls inherited `resize_window` method from `InitializationEngine`

        bool init_data();

        unsigned char* p_cam_buff_mem;
        AllocatedBuffer camera_buffer; // Holds GPUCameraData

        vk::Sampler default_sampler;
        Texture placeholder_texture;

        bool init_descriptors();

        vk::DescriptorPool descriptor_pool;
        vk::DescriptorSetLayout global_set_layout;
        // vk::DescriptorSetLayout texture_set_layout;
        // vk::DescriptorSet default_texture_descriptor;
        
        struct FrameData {
            vk::DescriptorSet global_descriptor;
        };
        std::array<FrameData, FRAME_OVERLAP> frame_data{};
        FrameData& get_frame_data(uint64_t frame_number) {return frame_data[frame_number%FRAME_OVERLAP];}

        void draw_objects(AbstractCamera* camera, std::shared_ptr<Node>& object_hierarchy);
        uint64_t frame_number{0};

        friend class AquilaEngine;
    
    private:
        DeletionQueue deletion_queue;
    };

}

#endif
