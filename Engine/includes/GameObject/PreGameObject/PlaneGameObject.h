#pragma once
#include "lve_constants.h"
#include "Transform.h"
#include "GameObject/GameObject.h"

namespace lve
{
	/**
	 * @brief Repr�sente un objet plan dans le jeu.
	 *
	 * Cette classe repr�sente un objet plan dans le jeu. Un objet plan est un type d'objet g�om�trique bidimensionnel
	 * d�fini par sa position, sa taille et son orientation dans l'espace tridimensionnel.
	 */
	class PlaneGameObject
	{
		public:
			/**
			 * @brief Cr�e un nouvel objet plan.
			 *
			 * Cette m�thode statique cr�e un nouvel objet plan avec les param�tres sp�cifi�s.
			 *
			 * @param _lveDevice Une r�f�rence vers l'objet LveDevice utilis� pour cr�er l'objet plan.
			 * @param _position La position initiale de l'objet plan dans l'espace tridimensionnel (par d�faut : glm::vec3(0, 0, 0)).
			 * @param _scale L'�chelle initiale de l'objet plan (par d�faut : glm::vec3(1, 1, 1)).
			 * @param _rotation La rotation initiale de l'objet plan (par d�faut : glm::vec3(0, 0, 0)).
			 * @return Un pointeur vers le nouvel objet plan cr��.
			 */
			static GameObject* Create(LveDevice& _lveDevice, glm::vec3 _position = VEC3_ZERO,
				glm::vec3  _scale = VEC3_ONE, glm::vec3 _rotation = VEC3_ZERO);
	};
}
