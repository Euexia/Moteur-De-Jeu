#pragma once
#include <glm.hpp>
#include <BulletCollision/CollisionShapes/btBoxShape.h>

/**
 * @brief Classe pour la conversion de types.
 *
 * Cette classe fournit des m�thodes pour la conversion entre diff�rents types de donn�es.
 */
class Convertisseur
{
	public:
		/**
		 * @brief Convertit un vecteur glm::vec3 en btVector3.
		 *
		 * @param _glmVec Le vecteur glm::vec3 � convertir.
		 * @return Le vecteur btVector3 converti.
		 */
		static btVector3 GlmVec3ToBtVector3(const glm::vec3& _glmVec)
		{
			return btVector3(_glmVec.x, _glmVec.y, _glmVec.z);
		}
};
