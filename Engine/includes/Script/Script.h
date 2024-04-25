#pragma once

#include "IScript.h"

#include "Modules/InputModule.h"
#include "Modules/rhi.h"
#include "Modules/TimeModule.h"
#include "Modules/WindowModule.h"

/**
 * @brief Classe Script.
 *
 * Cette classe repr�sente un script de base utilis� dans le moteur de jeu.
 * Elle fournit des m�thodes pour acc�der aux diff�rents modules du moteur, tels que le module de temps, le module de fen�tre, le module d'interface de rendu et le module d'entr�e.
 * Elle h�rite de la classe de base IScript.
 */
class Script : public IScript
{
public:
    /**
     * @brief Constructeur par d�faut.
     */
    Script();

    /**
     * @brief Destructeur par d�faut.
     */
    ~Script() = default;

    /**
     * @brief Obtient une r�f�rence vers le module de temps.
     * @return Un pointeur vers le module de temps.
     */
    TimeModule* GetTimeModule() const;

    /**
     * @brief Obtient une r�f�rence vers le module de fen�tre.
     * @return Un pointeur vers le module de fen�tre.
     */
    WindowModule* GetWindowModule() const;

    /**
     * @brief Obtient une r�f�rence vers le module d'interface de rendu.
     * @return Un pointeur vers le module d'interface de rendu.
     */
    RHIModule* GetRenderInterfaceModule() const;

    /**
     * @brief Obtient une r�f�rence vers le module d'entr�e.
     * @return Un pointeur vers le module d'entr�e.
     */
    InputModule* GetInputModule() const;

private:

    TimeModule* timeModuleReference = nullptr; /**< R�f�rence au module de gestion du temps. */
    WindowModule* windowModuleReference = nullptr; /**< R�f�rence au module de fen�tre. */
    RHIModule* renderInterfaceModuleReference = nullptr; /**< R�f�rence au module d'interface de rendu. */
    InputModule* inputModuleReference = nullptr; /**< R�f�rence au module d'entr�e. */
};
