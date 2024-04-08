#pragma
#include "Component.h"
#include "Convertisseur.h"

#include <glm.hpp>

#include <string>

#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <BulletCollision/CollisionShapes/btBoxShape.h>
#include <BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>


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
		btBoxShape*    collisionShape;
		btGhostObject* ghostObject;
		btRigidBody*   rigidBody;
};
