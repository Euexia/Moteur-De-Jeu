#pragma once

#include "Component.h"
#include "GameObject/GameObject.h"
#include <iostream>
#include <glm.hpp>

/**
 * @brief La classe CharacterController repr�sente un composant pour contr�ler un personnage dans le jeu.
 */
class CharacterController : public Component
{
public:
    /**
     * @brief Constructeur de la classe CharacterController.
     * @param gameobject Pointeur vers l'objet GameObject auquel ce composant est attach�.
     */
    CharacterController(GameObject* gameobject);

    /**
     * @brief Destructeur de la classe CharacterController.
     */
    ~CharacterController() {}

    /**
     * @brief D�finit le centre du CharacterController.
     */
    glm::vec3 setCenter(float _x, float _y, float _z);

    /**
     * @brief D�finit la largeur de la peau du CharacterController.
     * @param _width Largeur de la peau � d�finir.
     */
    void setSkinWidth(float _width);

    /**
     * @brief D�finit le rayon du CharacterController.
     */
    void setRadius();

    /**
     * @brief D�finit la hauteur du CharacterController.
     */
    void setHeight();

protected:
    GameObject* gameobject; /**< Pointeur vers l'objet GameObject auquel ce composant est attach�. */
};
