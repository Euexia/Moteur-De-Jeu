#pragma once


#include <glm.hpp>

#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <BulletCollision/CollisionShapes/btSphereShape.h>
#include <BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>

class SphereCollider
{
	/**
	* @brief Constructeur de la classe BoxCollider.
	*
	* @param _dynamicsWorld Un pointeur vers le monde dynamique de physique Bullet.
	* @param _center La position centrale du box collider.
	* @param _position La position du box collider.
	* @param _collisionWorld Un pointeur vers le monde de collision Bullet.
	*/
	SphereCollider(btDiscreteDynamicsWorld* _dynamicsWorld, glm::vec3 _center, glm::vec3 _position,
	               btCollisionWorld*        _collisionWorld);
	/**
	* @brief D�finit la taille du box collider.
	*
	* @param _radius
	* @param _size La taille � d�finir pour le box collider.
	*/
	void SetRadius(float _radius);
	/**
	* @brief D�finit le centre du box collider.
	*
	* @param _center Le centre � d�finir pour le box collider.
	*/
	void SetCenter(glm::vec3 _center) const;

	bool OnTrigger() const
	{
		return ghostObject->getNumOverlappingObjects() > 0;
	}

	/**
	* @brief Destructeur de l'objet BoxCollider, lib�rant les ressources associ�es.
	*/
	~SphereCollider();

	private:
		btSphereShape* collisionShape;
		btGhostObject* ghostObject;
		btRigidBody*   rigidBody;
};
