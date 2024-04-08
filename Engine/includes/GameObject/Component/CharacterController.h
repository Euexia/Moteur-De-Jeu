#pragma once

#include "Component.h"
#include "GameObject/GameObject.h"
#include <iostream>
#include <glm.hpp>

/**
 * @brief La classe CharacterController représente un composant pour contrôler un personnage dans le jeu.
 */
class CharacterController : public Component
{
public:
    /**
     * @brief Constructeur de la classe CharacterController.
     * @param gameobject Pointeur vers l'objet GameObject auquel ce composant est attaché.
     */
    CharacterController(GameObject* gameobject);

    /**
     * @brief Destructeur de la classe CharacterController.
     */
    ~CharacterController() {}

    /**
     * @brief Définit le centre du CharacterController.
     */
    glm::vec3 setCenter(float _x, float _y, float _z);

    /**
     * @brief Définit la largeur de la peau du CharacterController.
     * @param _width Largeur de la peau à définir.
     */
    void setSkinWidth(float _width);

    /**
     * @brief Définit le rayon du CharacterController.
     */
    void setRadius();

    /**
     * @brief Définit la hauteur du CharacterController.
     */
    void setHeight();

protected:
    GameObject* gameobject; /**< Pointeur vers l'objet GameObject auquel ce composant est attaché. */
};
