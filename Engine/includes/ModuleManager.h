#pragma once

#include <vector>
#include "Modules/Module.h"

/**
 * @brief Classe ModuleManager.
 *
 * Cette classe représente le gestionnaire de modules dans le système.
 * Le ModuleManager est responsable de la création, de la gestion et de la communication entre les différents modules de l'application.
 */
class ModuleManager
{
public:
	/**
	 * @brief Constructeur par défaut de ModuleManager.
	 */
	ModuleManager() = default;

	/**
	 * @brief Destructeur de ModuleManager.
	 */
	~ModuleManager();

	/**
	 * @brief Crée les modules par défaut.
	 */
	void CreateDefaultModules();

	/**
	 * @brief Initialise tous les modules.
	 */
	void Init() const;

	/**
	 * @brief Démarre tous les modules.
	 */
	void Start() const;

	/**
	 * @brief Effectue la mise à jour fixe de tous les modules.
	 */
	void FixedUpdate() const;

	/**
	 * @brief Effectue la mise à jour de tous les modules.
	 */
	void Update() const;

	/**
	 * @brief Effectue les opérations pré-rendu de tous les modules.
	 */
	void PreRender() const;

	/**
	 * @brief Rend tous les modules.
	 */
	void Render() const;

	/**
	 * @brief Rend l'interface graphique de tous les modules.
	 */
	void RenderGui() const;

	/**
	 * @brief Effectue les opérations post-rendu de tous les modules.
	 */
	void PostRender() const;

	/**
	 * @brief Libère les ressources de tous les modules.
	 */
	void Release() const;

	/**
	 * @brief Finalise tous les modules.
	 */
	void Finalize() const;

	/**
	 * @brief Crée un module de type T et l'ajoute au gestionnaire de modules.
	 *
	 * @tparam T Le type de module à créer.
	 * @return Un pointeur vers le module créé.
	 */
	template <class T>
	T* CreateModule();

	/**
	 * @brief Récupère un module de type T du gestionnaire de modules.
	 *
	 * @tparam T Le type de module à récupérer.
	 * @return Un pointeur vers le module récupéré, ou nullptr s'il n'existe pas.
	 */
	template <class T>
	T* GetModule();

private:
	std::vector<Module*> modules; /**< Vecteur de pointeurs vers les modules gérés par le ModuleManager. */
};

/**
 * @brief Crée un module de type T et l'ajoute au gestionnaire de modules.
 *
 * @tparam T Le type de module à créer.
 * @return Un pointeur vers le module créé.
 */
template <class T>
T* ModuleManager::CreateModule()
{
	T* module = new T();
	module->moduleManager = this;
	modules.push_back(module);
	return module;
}

/**
 * @brief Récupère un module de type T du gestionnaire de modules.
 *
 * @tparam T Le type de module à récupérer.
 * @return Un pointeur vers le module récupéré, ou nullptr s'il n'existe pas.
 */
template <class T>
T* ModuleManager::GetModule()
{
	for (Module*& module : modules)
	{
		if (T* module_cast = dynamic_cast<T*>(module)) return module_cast;
	}
	return nullptr;
}
