#pragma once

#include <vulkan/vulkan.hpp>

//libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm.hpp>

namespace lve
{
	class LveCamera
	{
		public:
			/**
			* @brief D�finit une projection orthographique.
			*
			* Cette fonction configure la matrice de projection pour une projection orthographique.
			* Une projection orthographique est une m�thode de projection en infographie o� les objets sont projet�s sur un plan parall�le � l'�cran, ce qui donne une repr�sentation sans perspective.
			*
			* @param _left La position du plan de gauche de la vue orthographique.
			* @param _right La position du plan de droite de la vue orthographique.
			* @param _top La position du plan de dessus de la vue orthographique.
			* @param _bottom La position du plan de dessous de la vue orthographique.
			* @param _near La distance au plan de vue proche.
			* @param _far La distance au plan de vue �loign�e.
			*/
			void SetOrthographicProjection(float _left, float _right, float _top, float _bottom, float _near,
			                               float _far);

			/**
			 * @brief D�finit une projection en perspective.
			 *
			 * Cette fonction configure la matrice de projection pour une projection en perspective.
			 * Une projection en perspective est une m�thode de projection en infographie qui cr�e une illusion de profondeur en donnant aux objets plus �loign�s l'air d'�tre plus petits qu'ils ne le sont r�ellement.
			 *
			 * @param _fovY L'angle de champ vertical de la vue en degr�s.
			 * @param _aspect Le rapport d'aspect de la vue, d�fini comme la largeur de la fen�tre divis�e par sa hauteur.
			 * @param _near La distance au plan de vue proche.
			 * @param _far La distance au plan de vue �loign�e.
			 *
			 * @note Cette fonction exige que `_aspect` soit diff�rent de z�ro.
			 * @note L'angle de champ vertical `_fovY` est sp�cifi� en degr�s.
			 */
			void SetPerspectiveProjection(float _fovY, float _aspect, float _near, float _far);

			/**
			 * @brief D�finit la direction de vue de la cam�ra.
			 *
			 * Cette fonction configure la matrice de vue pour orienter la cam�ra dans une direction sp�cifi�e.
			 * La matrice de vue est utilis�e en infographie pour transformer les coordonn�es des objets de l'espace monde � l'espace cam�ra, en tenant compte de la position de la cam�ra et de sa direction de vue.
			 *
			 * @param _position La position de la cam�ra dans l'espace.
			 * @param _direction La direction vers laquelle la cam�ra fait face.
			 * @param _up La direction consid�r�e comme �tant "vers le haut" pour la cam�ra.
			 *
			 * @note Les vecteurs _direction et _up doivent �tre normalis�s pour garantir des r�sultats corrects.
			 */
			void SetViewDirection(glm::vec3 _position, glm::vec3 _direction, glm::vec3 _up = glm::vec3{0.f, -1.f, 0.f});

			/**
			 * @brief D�finit la cible de vue de la cam�ra.
			 *
			 * Cette fonction configure la matrice de vue pour orienter la cam�ra vers une cible sp�cifi�e.
			 * La matrice de vue est utilis�e en infographie pour transformer les coordonn�es des objets de l'espace monde � l'espace cam�ra, en tenant compte de la position de la cam�ra et de sa direction de vue.
			 *
			 * @param _position La position de la cam�ra dans l'espace.
			 * @param _target La position de la cible vers laquelle la cam�ra doit �tre orient�e.
			 * @param _up La direction consid�r�e comme �tant "vers le haut" pour la cam�ra.
			 *
			 * @note La diff�rence entre _target et _position est utilis�e pour calculer la direction de vue de la cam�ra.
			 */
			void SetViewTarget(glm::vec3 _position, glm::vec3 _target, glm::vec3 _up = glm::vec3{0.f, -1.f, 0.f});

			/**
			 * @brief D�finit la vue de la cam�ra en utilisant les rotations YXZ.
			 *
			 * Cette fonction configure la matrice de vue pour orienter la cam�ra en fonction des rotations autour des axes Y, X et Z.
			 * La matrice de vue est utilis�e en infographie pour transformer les coordonn�es des objets de l'espace monde � l'espace cam�ra, en tenant compte de la position de la cam�ra et de son orientation.
			 *
			 * @param _position La position de la cam�ra dans l'espace.
			 * @param _rotation Les rotations autour des axes Y, X et Z, respectivement, sp�cifi�es en radians.
			 *
			 * @note Les rotations sont appliqu�es dans l'ordre suivant : d'abord autour de l'axe Y, puis autour de l'axe X et enfin autour de l'axe Z.
			 */
			void SetViewYXZ(glm::vec3 _position, glm::vec3 _rotation);

			/**
			 * @brief Renvoie la matrice de projection.
			 *
			 * Cette fonction renvoie la matrice de projection stock�e en interne.
			 * La matrice de projection est g�n�ralement utilis�e en infographie pour projeter des coordonn�es 3D sur un �cran 2D.
			 *
			 * @return const glm::mat4& La matrice de projection.
			 */
			[[nodiscard]] const glm::mat4& GetProjection() const { return projectionMatrix; }

			/**
			* @brief Renvoie la matrice de vue.
			*
			* Cette fonction renvoie la matrice de vue stock�e en interne.
			* La matrice de vue est utilis�e dans les syst�mes de graphiques 3D pour transformer les coordonn�es des objets de l'espace monde � l'espace cam�ra.
			*
			* @return const glm::mat4& La matrice de vue.
			*/
			[[nodiscard]] const glm::mat4& GetView() const { return viewMatrix; }


			/**
			 * @brief Obtient la matrice de vue inverse.
			 *
			 * @return La r�f�rence constante vers la matrice de vue inverse.
			 */
			[[nodiscard]] const glm::mat4& GetInverseView() const { return inverseViewMatrix; }

			/**
			 * @brief Obtient la position de la cam�ra.
			 *
			 * @return La position de la cam�ra en tant que vecteur glm::vec3.
			 */
			glm::vec3 GetPosition() const { return glm::vec3(inverseViewMatrix[3]); }

		private:
			glm::mat4 projectionMatrix{1.f};  /**< Matrice de projection. */
			glm::mat4 viewMatrix{1.f};        /**< Matrice de vue. */
			glm::mat4 inverseViewMatrix{1.f}; /**< Matrice de vue inverse. */
	};
}
