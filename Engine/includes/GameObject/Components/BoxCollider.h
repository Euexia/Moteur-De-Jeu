#pragma

#include <glm.hpp>

#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <BulletCollision/CollisionShapes/btBoxShape.h>
#include <BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>

#include "Component.h"

/**
 * @brief Repr�sente un composant de collision de type bo�te.
 *
 * Cette classe h�rite de la classe Component et contient des fonctionnalit�s sp�cifiques
 * aux bo�tes de collision dans un environnement physique.
 */
class BoxCollider : public Component
{
	public:
		/**
		 * @brief Constructeur de la classe BoxCollider.
		 *
		 * @param _dynamicsWorld Un pointeur vers le monde dynamique de physique Bullet.
		 * @param _center La position centrale du box collider.
		 * @param _position La position du box collider.
		 * @param _collisionWorld Un pointeur vers le monde de collision Bullet.
		 */
		BoxCollider(btDiscreteDynamicsWorld* _dynamicsWorld, glm::vec3 _center, glm::vec3 _position,
		            btCollisionWorld*        _collisionWorld);
		/**
		 * @brief D�finit la taille du box collider.
		 *
		 * @param _size La taille � d�finir pour le box collider.
		 */
		void SetSize(glm::vec3 _size);
		/**
		* @brief D�finit la position centrale du box collider.
		*
		* @param _center La position centrale � d�finir pour le box collider.
		*/
		void SetCenter(glm::vec3 _center) const;
		/**
		* @brief V�rifie si le box collider est en collision avec un autre objet.
		*
		* @return Vrai si le box collider est en collision avec un autre objet, sinon faux.
		*/
		bool OnTrigger() const
		{
			return ghostObject->getNumOverlappingObjects() > 0;
		}

		/**
		* @brief Destructeur de l'objet BoxCollider, lib�rant les ressources associ�es.
		*/
		~BoxCollider() override;

	private:
		btBoxShape* collisionShape = nullptr; /**< La forme de collision de la bo�te. */
		btGhostObject* ghostObject = nullptr; /**< L'objet fant�me associ� � la bo�te. */
		btRigidBody* rigidBody = nullptr;     /**< Le corps rigide associ� � la bo�te. */
};
