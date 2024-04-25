#pragma once

#include "LveEngine/lve_device.h"

#include <vulkan/vulkan.hpp>

#include <string>
#include <vector>

namespace lve
{
	/**
	 * @brief Structure contenant les informations de configuration pour la cr�ation d'un pipeline.
	 */
	struct PipelineConfigInfo
	{
		PipelineConfigInfo()                                     = default; /**< Constructeur par d�faut. */
		PipelineConfigInfo(const PipelineConfigInfo&)            = delete;  /**< Constructeur de copie supprim�. */
		PipelineConfigInfo& operator=(const PipelineConfigInfo&) = delete;  /**< Op�rateur d'affectation supprim�. */

		std::vector<vk::VertexInputAttributeDescription> attributeDescriptions{};
		/**< Descriptions des attributs de vertex. */
		std::vector<vk::VertexInputBindingDescription> bindingDescriptions{};
		/**< Descriptions des liaisons d'entr�e de vertex. */
		vk::PipelineViewportStateCreateInfo      viewportInfo; /**< Informations sur le viewport du pipeline. */
		vk::PipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
		/**< Informations sur l'assemblage d'entr�e du pipeline. */
		vk::PipelineRasterizationStateCreateInfo rasterizationInfo;
		/**< Informations sur la rasterisation du pipeline. */
		vk::PipelineMultisampleStateCreateInfo multisampleInfo;
		/**< Informations sur le multi-�chantillonnage du pipeline. */
		vk::PipelineColorBlendAttachmentState colorBlendAttachment;
		/**< Attachement de m�lange de couleur du pipeline. */
		vk::PipelineColorBlendStateCreateInfo colorBlendInfo;
		/**< Informations sur le m�lange de couleur du pipeline. */
		vk::PipelineDepthStencilStateCreateInfo depthStencilInfo;
		/**< Informations sur la profondeur et le stencil du pipeline. */
		std::vector<vk::DynamicState>      dynamicStateEnables; /**< �tats dynamiques activ�s du pipeline. */
		vk::PipelineDynamicStateCreateInfo dynamicStateInfo; /**< Informations sur l'�tat dynamique du pipeline. */
		vk::PipelineLayout                 pipelineLayout = nullptr; /**< Layout du pipeline. */
		vk::RenderPass                     renderPass     = nullptr; /**< Passe de rendu associ�e au pipeline. */
		uint32_t                           subpass        = 0; /**< Index de la sous-passe du pipeline. */
	};

	/**
	 * @brief Classe repr�sentant un pipeline Vulkan.
	 */
	class LvePipeline
	{
		public:
			/**
			 * @brief Constructeur de la classe LvePipeline.
			 *
			 * Initialise un pipeline graphique en cr�ant les shaders � partir des fichiers sp�cifi�s.
			 *
			 * @param _vertFilepath Le chemin d'acc�s au fichier contenant le code source du shader de vertex.
			 * @param _fragFilepath Le chemin d'acc�s au fichier contenant le code source du shader de fragment.
			 */
			LvePipeline(LveDevice& _device, const std::string& _vertFilepath, const std::string& _fragFilepath,
			            const PipelineConfigInfo& _configInfo);

			/**
			 * @brief Destructeur de la classe LvePipeline.
			 */
			~LvePipeline();

			/**
			 * @brief Constructeur de copie supprim�.
			 */
			LvePipeline(const LvePipeline&) = delete;

			/**
			 * @brief Op�rateur d'affectation supprim�.
			 */
			LvePipeline& operator=(const LvePipeline&) = delete;


			/**
			 * @brief Lie le pipeline graphique au tampon de commandes sp�cifi�.
			 *
			 * Cette m�thode lie le pipeline graphique associ� � ce LvePipeline � un tampon de commandes sp�cifi�.
			 *
			 * @param _commandBuffer Le tampon de commandes auquel le pipeline graphique doit �tre li�.
			 */
			void Bind(vk::CommandBuffer _commandBuffer) const;

			/**
			 * @brief Configure les param�tres par d�faut du pipeline.
			 *
			 * Cette m�thode configure les param�tres par d�faut du pipeline en remplissant la structure PipelineConfigInfo fournie.
			 *
			 * @param _configInfo Une r�f�rence vers la structure PipelineConfigInfo � remplir avec les param�tres par d�faut du pipeline.
			 */
			static void DefaultPipelineConfigInfo(PipelineConfigInfo& _configInfo);

			/**
			 * @brief Active le m�lange alpha dans la configuration de pipeline sp�cifi�e.
			 *
			 * @param _configInfo Les informations de configuration de pipeline.
			 */
			static void EnableAlphaBlending(PipelineConfigInfo& _configInfo);

		private:
			/**
			* @brief Lit le contenu d'un fichier binaire et le retourne sous forme de vecteur de caract�res.
			*
			* Cette fonction ouvre un fichier binaire, lit son contenu dans un vecteur de caract�res et le retourne.
			*
			* @param _filepath Le chemin d'acc�s au fichier � lire.
			* @return Un vecteur de caract�res contenant le contenu du fichier.
			* @throws std::runtime_error si le fichier ne peut pas �tre ouvert.
			*/
			static std::vector<char> ReadFile(const std::string& _filepath);

			/**
			 * @brief Cr�e un pipeline graphique � partir des fichiers de shader sp�cifi�s et des informations de configuration de pipeline.
			 *
			 * Cette fonction charge les shaders � partir des fichiers sp�cifi�s, configure le pipeline graphique en utilisant les informations
			 * de configuration fournies, et cr�e le pipeline graphique correspondant.
			 *
			 * @param _vertFilepath Le chemin d'acc�s au fichier du shader de sommet.
			 * @param _fragFilepath Le chemin d'acc�s au fichier du shader de fragment.
			 * @param _configInfo Les informations de configuration de pipeline.
			 *
			 * @note Cette fonction doit �tre appel�e apr�s l'initialisation de la fen�tre et du p�riph�rique Vulkan.
			 */
			void CreateGraphicsPipeline(const std::string&        _vertFilepath, const std::string& _fragFilepath,
			                            const PipelineConfigInfo& _configInfo);


			/**
			 * @brief Cr�e un module de shader � partir du code du shader sp�cifi�.
			 *
			 * Cette m�thode cr�e un module de shader Vulkan � partir du code du shader sp�cifi�.
			 *
			 * @param _code Le code du shader sous forme d'un vecteur de caract�res.
			 * @param _shaderModule Un pointeur vers l'emplacement o� stocker le module de shader cr��.
			 *
			 * @throw std::runtime_error si la cr�ation du module de shader �choue.
			 */
			void CreateShaderModule(const std::vector<char>& _code, vk::ShaderModule* _shaderModule) const;


			/**
			 * @brief R�f�rence vers le p�riph�rique Vulkan associ� � ce pipeline.
			 */
			LveDevice& lveDevice;

			/**
			 * @brief Le pipeline graphique Vulkan.
			 */
			vk::Pipeline graphicsPipeline;

			/**
			 * @brief Le module de shader de sommet Vulkan.
			 */
			vk::ShaderModule vertShaderModule;

			/**
			 * @brief Le module de shader de fragment Vulkan.
			 */
			vk::ShaderModule fragShaderModule;
	};
} //namespace lve
