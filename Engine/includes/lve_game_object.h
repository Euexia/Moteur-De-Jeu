#pragma once

#include "lve_model.h"
#include "lve_texture.h"

#include <vulkan/vulkan.hpp>

//std
#include <memory>
#include <unordered_map>
#include <gtc/matrix_transform.hpp>

namespace lve
{
	struct TransformComponent
	{
		glm::vec3 translation{};        /**< D�calage de position. */
		glm::vec3 scale{1.f, 1.f, 1.f}; /**< Facteurs d'�chelle sur les axes x, y et z. */
		glm::vec3 rotation{};           /**< Angles de rotation autour des axes x, y et z (en radians). */

		/**
		 * @brief Retourne la matrice de transformation correspondante.
		 *
		 * Cette fonction retourne la matrice de transformation correspondant aux d�calages de position,
		 * �chelles et rotations sp�cifi�s dans la structure TransformComponent.
		 *
		 * @return La matrice de transformation.
		 */
		glm::mat4 Mat4();

		/**
		 * @brief Retourne la matrice de transformation normale.
		 *
		 * Cette fonction retourne la matrice de transformation normale correspondante aux d�calages de position,
		 * �chelles et rotations sp�cifi�s dans la structure TransformComponent.
		 * Cette matrice est utilis�e pour la transformation des normales des surfaces afin de les maintenir correctement orient�es
		 * lors de transformations non uniformes.
		 *
		 * @return La matrice de transformation normale.
		 */
		glm::mat3 NormalMatrix() const;
	};


	struct PointLightComponent
	{
		float lightIntensity = 1.0f; /**< Intensit� lumineuse de la lumi�re ponctuelle. */
	};


	class LveGameObject
	{
		public:
			using id_t = unsigned int; /**< Type pour identifier les objets. */
			using Map = std::unordered_map<id_t, LveGameObject>;
			/**< Type pour repr�senter une carte d'objets avec des identifiants. */

			/**
			 * @brief Cr�e un nouvel objet de jeu.
			 *
			 * Cette fonction cr�e un nouvel objet de jeu en g�n�rant un identifiant unique pour cet objet.
			 *
			 * @return L'objet de jeu cr��.
			 */
			static LveGameObject CreateGameObject()
			{
				static id_t current_id = 0; /**< Identifiant actuel pour les objets cr��s. */
				return LveGameObject{current_id++};
				/**< Retourne un nouvel objet de jeu avec l'identifiant incr�ment�. */
			}

			LveGameObject(const LveGameObject&)            = delete;  /**< Constructeur de copie supprim�. */
			LveGameObject& operator=(const LveGameObject&) = delete;  /**< Op�rateur d'affectation supprim�. */
			LveGameObject(LveGameObject&&)                 = default; /**< Constructeur de d�placement par d�faut. */
			LveGameObject& operator=(LveGameObject&&)      = default; /**< Op�rateur de d�placement par d�faut. */

			/**
			 * @brief Obtient l'identifiant de l'objet.
			 *
			 * @return L'identifiant de l'objet.
			 */
			id_t GetId() const { return id; }


			/**
			 * @brief Cr�e un objet de jeu repr�sentant une lumi�re ponctuelle.
			 *
			 * Cette m�thode statique cr�e un objet de jeu repr�sentant une lumi�re ponctuelle avec les param�tres sp�cifi�s.
			 *
			 * @param _intensity L'intensit� de la lumi�re.
			 * @param _radius Le rayon d'�clairage de la lumi�re.
			 * @param _color La couleur de la lumi�re.
			 * @return L'objet de jeu repr�sentant la lumi�re ponctuelle.
			 */
			static LveGameObject MakePointLight(float     _intensity = 10.f, float _radius = 0.1f,
			                                    glm::vec3 _color     = glm::vec3(1.f));

		// Optional pointer components
		std::shared_ptr<LveModel> model{};
		std::unique_ptr<PointLightComponent> pointLight = nullptr;
		std::unique_ptr<lve::LveTexture> texture = nullptr;
	private:
		LveGameObject(id_t _objId) : id(_objId) {}
			glm::vec3          color{};     /**< Couleur de l'objet. */
			TransformComponent transform{}; /**< Composant de transformation de l'objet. */

			// Optional pointer components
			//std::shared_ptr<LveModel>            model{}; /**< Mod�le de l'objet. */
			std::unique_ptr<PointLightComponent> pointLight = nullptr;
			/**< Composant de lumi�re ponctuelle de l'objet, s'il y en a un. */

		private:
			id_t id; /**< Identifiant de l'objet. */

			/**
			 * @brief Constructeur de la classe LveGameObject.
			 *
			 * @param _objId Identifiant de l'objet.
			 */
			explicit LveGameObject(const id_t _objId) : id(_objId)
			{
			}
	};
} //namespace lve
