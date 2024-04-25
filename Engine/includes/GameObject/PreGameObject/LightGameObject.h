#pragma once
#include "GameObject/GameObject.h"
#include "LveEngine/lve_constants.h"

namespace lve
{
	/**
	 * @brief Repr�sente un objet lumi�re dans le jeu.
	 *
	 * Cette classe repr�sente un objet lumi�re dans le jeu. Un objet lumi�re est un �l�ment
	 * utilis� pour simuler l'�clairage dans une sc�ne tridimensionnelle.
	 */
	class LightGameObject
	{
		public:
			/**
			 * @brief Cr�e un nouvel objet lumi�re.
			 *
			 * Cette m�thode statique cr�e un nouvel objet lumi�re avec les param�tres sp�cifi�s.
			 *
			 * @param _intensity L'intensit� de la lumi�re (par d�faut : 10.0).
			 * @param _radius Le rayon d'illumination de la lumi�re (par d�faut : 1.0).
			 * @param _position La position initiale de l'objet lumi�re dans l'espace tridimensionnel (par d�faut : glm::vec3(0, 0, 0)).
			 * @param _rotate La rotation initiale de l'objet lumi�re (par d�faut : glm::vec3(0, 0, 0)).
			 * @param _color La couleur de la lumi�re (par d�faut : glm::vec3(1, 1, 1)).
			 * @return Un pointeur vers le nouvel objet lumi�re cr��.
			 */
			static GameObject* Create(float     _intensity = 10.f, float _radius = 1.f, glm::vec3 _position = VEC3_ZERO,
			                          glm::vec3 _rotate    = VEC3_ZERO, glm::vec3 _color = VEC3_ONE);
	};
}
