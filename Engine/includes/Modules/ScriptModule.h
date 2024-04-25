#pragma once

#include <vector>
#include <queue>
#include <string> 
#include "IScript.h"

/**
 * @brief Classe ScriptModule.
 *
 * Cette classe g�re l'ajout, le d�marrage et la mise � jour des scripts dans le moteur de jeu.
 */
class ScriptModule {
public:
    /**
     * @brief Constructeur par d�faut.
     */
    ScriptModule() = default;

    /**
     * @brief Destructeur par d�faut.
     */
    ~ScriptModule() = default;

    /**
     * @brief Ajoute un script au module.
     * @tparam T Le type de script � ajouter.
     * @tparam U La classe du script � ajouter.
     * @return True si le script a �t� ajout� avec succ�s, sinon false.
     */
    template <typename T, class U>
    bool AddScript()
    {
        IScript* script = static_cast<IScript*>(new U);

        script->Initialize();

        scriptsToStart.push(script);
        scripts.push_back(script);

        return true;
    }

    /**
     * @brief D�marre tous les scripts en attente.
     * @return True si tous les scripts ont �t� d�marr�s avec succ�s, sinon false.
     */
    bool StartScripts();

    /**
     * @brief Obtient un script par son nom.
     * @param _name Le nom du script � obtenir.
     * @return Un pointeur vers le script s'il est trouv�, sinon nullptr.
     */
    IScript* GetIScriptByName(const std::string& _name) const;

    /**
     * @brief Met � jour tous les scripts.
     */
    void UpdateScripts();

    /**
     * @brief Arr�te tous les scripts et lib�re les ressources.
     */
    void Stop();

private:
    std::vector<IScript*> scripts;
    std::queue<IScript*> scriptsToStart;
};
