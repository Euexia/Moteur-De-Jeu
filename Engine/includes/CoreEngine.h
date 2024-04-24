#pragma once

#include "ModuleManager.h"

/**
 * @brief Classe Engine.
 *
 * Cette classe représente le moteur principal de jeu.
 */
class Engine
{
public:
    /**
     * @brief Obtient l'instance unique de la classe Engine.
     * @return Un pointeur vers l'instance unique de Engine.
     */
    static Engine* GetInstance();

    /**
     * @brief Initialise le moteur.
     */
    void Init() const;

    /**
     * @brief Lance l'exécution du moteur.
     */
    void Run() const;

    /**
     * @brief Arrête l'exécution du moteur.
     */
    void Quit() { shouldQuit = true; }

    /**
     * @brief Obtient le gestionnaire de modules du moteur.
     * @return Un pointeur vers le gestionnaire de modules.
     */
    [[nodiscard]] ModuleManager* GetModuleManager() const { return moduleManager; }

private:
    static Engine* instance; /**< Instance unique de la classe Engine. */
    ModuleManager* moduleManager = new ModuleManager; /**< Gestionnaire de modules. */
    bool shouldQuit = false; /**< Indique si le moteur doit être arrêté. */
};
