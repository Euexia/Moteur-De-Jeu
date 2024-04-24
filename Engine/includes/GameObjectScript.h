#pragma once

#include "Script.h"
#include "GameObject/GameObject.h"
#include <string>

/**
 * @brief Classe GameObjectScript.
 *
 * Cette classe repr�sente un script attach� � un GameObject.
 * Elle h�rite de la classe de base Script.
 */
class GameObjectScript : public Script
{
public:
    /**
     * @brief Constructeur par d�faut.
     */
    GameObjectScript();

    /**
     * @brief Destructeur par d�faut.
     */
    ~GameObjectScript() = default;

protected:
    GameObject object; 
};
