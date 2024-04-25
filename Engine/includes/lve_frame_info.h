#pragma once

#include "lve_game_object.h"
#include "Camera/lve_camera.h"

//lib
#include <vulkan/vulkan.hpp>

namespace lve
{
	#define MAX_LIGHTS 10

	/**
	 * @brief Repr�sente une lumi�re ponctuelle dans l'espace 3D.
	 */
	struct PointLight
	{
		glm::vec4 position{}; /**< Position de la lumi�re. */
		glm::vec4 color{};    /**< Couleur de la lumi�re. */
	};

	/**
	 * @brief Repr�sente les donn�es uniformes globales utilis�es dans le pipeline de rendu.
	 */
	struct GlobalUbo
	{
		glm::mat4  projection{1.f};                        /**< Matrice de projection. */
		glm::mat4  view{1.f};                              /**< Matrice de vue. */
		glm::mat4  inverseView{1.f};                       /**< Matrice de vue inverse. */
		glm::vec4  ambientLightColor{1.f, 1.f, 1.f, .02f}; /**< Couleur de lumi�re ambiante. */
		PointLight pointLights[MAX_LIGHTS];                /**< Tableau des lumi�res ponctuelles. */
		int        numLights;                              /**< Nombre de lumi�res ponctuelles. */
	};

	/**
	 * @brief Contient les informations de trame utilis�es lors du rendu.
	 */
	struct FrameInfo
	{
		int                 frameIndex;          /**< Index de la trame. */
		float               frameTime;           /**< Temps de la trame. */
		vk::CommandBuffer   commandBuffer;       /**< Tampon de commandes Vulkan. */
		LveCamera&          camera;              /**< Cam�ra utilis�e pour le rendu. */
		vk::DescriptorSet   globalDescriptorSet; /**< Ensemble de descripteurs global. */
		vk::DescriptorSet   tex1DescriptorSet;
		vk::DescriptorSet   tex2DescriptorSet;
		LveGameObject::Map& gameObjects;         /**< Carte des objets de jeu. */
	};
} // namespace lve
