#pragma once

#include "lve_device.h"
#include "lve_frame_info.h"
#include "lve_game_object.h"
#include "lve_pipeline.h"
#include "Camera/lve_camera.h"

//std
#include <memory>
#include <vector>

class GameObject;

namespace lve
{
	struct PointLightPushConstants
	{
		glm::vec4 position{};
		glm::vec4 color{};
		float     radius;
	};

	/**
	 * @brief Classe repr�sentant la premi�re application utilisant Vulkan.
	 *
	 * Cette classe g�re l'ex�cution de la premi�re application Vulkan, incluant la cr�ation de la fen�tre.
	 */
	class PointLightSystem
	{
		public:
			PointLightSystem(LveDevice& _device, vk::RenderPass _renderPass, vk::DescriptorSetLayout _globalSetLayout);
			~PointLightSystem();

			PointLightSystem(const PointLightSystem&)           = delete;
			PointLightSystem operator=(const PointLightSystem&) = delete;

			/**
			 * @brief Rendu du syst�me de lumi�res ponctuelles.
			 *
			 * Cette m�thode effectue le rendu du syst�me de lumi�res ponctuelles en utilisant le pipeline de rendu sp�cifi�.
			 * Elle lie �galement les ensembles de descripteurs globaux n�cessaires pour le rendu.
			 * Enfin, elle utilise une commande de dessin pour rendre les lumi�res ponctuelles.
			 *
			 * @param _gameObjects
			 * @param _ubo
			 */
			void Update(const std::vector<GameObject*>& _gameObjects, GlobalUbo& _ubo);
			void Render(const std::vector<GameObject*>& _gameObjects, const LveCamera& _camera, const vk::CommandBuffer _commandBuffer,const vk::DescriptorSet _globalDescriptorSet) const;

		private:
			/**
			 * @brief Cr�e le layout du pipeline pour le syst�me de lumi�res ponctuelles.
			 *
			 * Cette m�thode cr�e le layout du pipeline pour le syst�me de lumi�res ponctuelles en utilisant
			 * le layout du jeu de descripteurs globaux sp�cifi�.
			 *
			 * @param _globalSetLayout Layout du jeu de descripteurs globaux utilis� dans le pipeline.
			 */
			void CreatePipelineLayout(vk::DescriptorSetLayout _globalSetLayout);

			/**
			* @brief Cr�e le pipeline pour le syst�me de lumi�res ponctuelles.
			*
			* Cette m�thode cr�e le pipeline pour le syst�me de lumi�res ponctuelles en utilisant le passe de rendu
			* sp�cifi� et le layout du pipeline pr�c�demment cr��.
			*
			* @param _renderPass Le passe de rendu auquel ce pipeline est li�.
			* @throws std::runtime_error Si la cr�ation du pipeline �choue.
			*/
			void CreatePipeline(vk::RenderPass _renderPass);

			LveDevice& lveDevice;


			std::unique_ptr<LvePipeline> lvePipeline;
			vk::PipelineLayout           pipelineLayout;
	};
} // namespace lve
