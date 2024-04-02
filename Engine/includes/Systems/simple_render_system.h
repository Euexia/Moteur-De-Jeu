#pragma once

#include <vulkan/vulkan.hpp>

#include "lve_camera.h"
#include "lve_pipeline.h"
#include "lve_device.h"
#include "lve_game_object.h"
#include "lve_frame_info.h"

//std
#include <memory>
#include <vector>

namespace lve {

    struct SimplePushConstantData {
        glm::mat4 modelMatrix{ 1.f };
        glm::mat4 normalMatrix{ 1.f };
        
    };

    /**
     * @brief Classe repr�sentant la premi�re application utilisant Vulkan.
     *
     * Cette classe g�re l'ex�cution de la premi�re application Vulkan, incluant la cr�ation de la fen�tre.
     */
    class LineRender {
    public:

        LineRender(LveDevice& _device, vk::RenderPass _renderPass, vk::DescriptorSetLayout _globalSetLayout);
        ~LineRender();

        LineRender(const LineRender&) = delete;
        LineRender operator=(const LineRender&) = delete;

        /**
         * @brief Rend les objets de jeu avec le syst�me de rendu simple.
         *
         * Cette m�thode rend les objets de jeu avec le syst�me de rendu simple en utilisant le pipeline graphique
         * et le layout du pipeline sp�cifi�s dans le cadre d'une Frame.
         *
         * @param _frameInfo Les informations de frame, y compris le tampon de commandes, le descripteur global, et les objets de jeu.
         */
        void RenderGameObjects(FrameInfo &_frameInfo);

    private:

        /**
         * @brief Cr�e le layout du pipeline pour le syst�me de rendu simple.
         *
         * Cette m�thode cr�e le layout du pipeline pour le syst�me de rendu simple en sp�cifiant le layout du descripteur global
         * ainsi que la plage des constantes pouss�es utilis�es par les shaders.
         *
         * @param _globalSetLayout Le layout du descripteur global.
         */
        void CreatePipelineLayout(vk::DescriptorSetLayout _globalSetLayout);

        /**
         * @brief Cr�e le pipeline pour le syst�me de rendu simple.
         *
         * Cette m�thode cr�e le pipeline pour le syst�me de rendu simple en utilisant les shaders sp�cifi�s et le layout de pipeline
         * pr�alablement cr��. Le rendu se fait selon le render pass sp�cifi�.
         *
         * @param _renderPass Le render pass utilis� pour le rendu.
         */
        void CreatePipeline(vk::RenderPass _renderPass);

        LveDevice &lveDevice;


        std::unique_ptr<LvePipeline> lvePipeline;
        vk::PipelineLayout pipelineLayout;
    };

} // namespace lve
