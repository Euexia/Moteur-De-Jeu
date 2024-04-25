#pragma once

#include <vulkan/vulkan.hpp>

#include "LveEngine/lve_device.h"
#include "LveEngine/lve_pipeline.h"
#include "Camera/lve_camera.h"

//std
#include <memory>
#include <vector>

class GameObject;

namespace lve
{
	struct SimplePushConstantData
	{
		glm::mat4 modelMatrix{1.f};
		glm::mat4 normalMatrix{1.f};
	};

	/**
	 * @brief Classe repr�sentant la premi�re application utilisant Vulkan.
	 *
	 * Cette classe g�re l'ex�cution de la premi�re application Vulkan, incluant la cr�ation de la fen�tre.
	 */
	class SimpleRenderSystem
	{
		public:
			SimpleRenderSystem(LveDevice&              _device, vk::RenderPass _renderPass,
			                   vk::DescriptorSetLayout _globalSetLayout);
			~SimpleRenderSystem();

			SimpleRenderSystem(const SimpleRenderSystem&)           = delete;
			SimpleRenderSystem operator=(const SimpleRenderSystem&) = delete;

			/**
			 * @brief Rend les objets de jeu avec le syst�me de rendu simple.
			 *
			 * Cette m�thode rend les objets de jeu avec le syst�me de rendu simple en utilisant le pipeline graphique
			 * et le layout du pipeline sp�cifi�s dans le cadre d'une Frame.
			 *
			 * @param _gameObjects
			 * @param _camera
			 * @param _commandBuffer
			 * @param _globalDescriptorSet
			 * @param _tex1DescriptorSet
			 * @param _tex2DescriptorSet
			 */
			void RenderGameObjects(const std::vector<GameObject*>& _gameObjects, const LveCamera& _camera, const vk::CommandBuffer _commandBuffer, std::vector<std::vector<vk::DescriptorSet>*>* _DescriptorSetsAll, int _frameIndex)  const;

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

			LveDevice& lveDevice;


			std::unique_ptr<LvePipeline> lvePipeline;
			vk::PipelineLayout           pipelineLayout;
	};
} // namespace lve
