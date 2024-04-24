#pragma once

#include <vector>
#include "Modules/Module.h"

/**
 * @brief Classe ModuleManager.
 *
 * Cette classe repr�sente le gestionnaire de modules dans le syst�me.
 * Le ModuleManager est responsable de la cr�ation, de la gestion et de la communication entre les diff�rents modules de l'application.
 */
class ModuleManager
{
public:
	/**
	 * @brief Constructeur par d�faut de ModuleManager.
	 */
	ModuleManager() = default;

	/**
	 * @brief Destructeur de ModuleManager.
	 */
	~ModuleManager();

	/**
	 * @brief Cr�e les modules par d�faut.
	 */
	void CreateDefaultModules();

	/**
	 * @brief Initialise tous les modules.
	 */
	void Init() const;

	/**
	 * @brief D�marre tous les modules.
	 */
	void Start() const;

	/**
	 * @brief Effectue la mise � jour fixe de tous les modules.
	 */
	void FixedUpdate() const;

	/**
	 * @brief Effectue la mise � jour de tous les modules.
	 */
	void Update() const;

	/**
	 * @brief Effectue les op�rations pr�-rendu de tous les modules.
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
	 * @brief Effectue les op�rations post-rendu de tous les modules.
	 */
	void PostRender() const;

	/**
	 * @brief Lib�re les ressources de tous les modules.
	 */
	void Release() const;

	/**
	 * @brief Finalise tous les modules.
	 */
	void Finalize() const;

	/**
	 * @brief Cr�e un module de type T et l'ajoute au gestionnaire de modules.
	 *
	 * @tparam T Le type de module � cr�er.
	 * @return Un pointeur vers le module cr��.
	 */
	template <class T>
	T* CreateModule();

	/**
	 * @brief R�cup�re un module de type T du gestionnaire de modules.
	 *
	 * @tparam T Le type de module � r�cup�rer.
	 * @return Un pointeur vers le module r�cup�r�, ou nullptr s'il n'existe pas.
	 */
	template <class T>
	T* GetModule();

private:
	std::vector<Module*> modules; /**< Vecteur de pointeurs vers les modules g�r�s par le ModuleManager. */
};

/**
 * @brief Cr�e un module de type T et l'ajoute au gestionnaire de modules.
 *
 * @tparam T Le type de module � cr�er.
 * @return Un pointeur vers le module cr��.
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
 * @brief R�cup�re un module de type T du gestionnaire de modules.
 *
 * @tparam T Le type de module � r�cup�rer.
 * @return Un pointeur vers le module r�cup�r�, ou nullptr s'il n'existe pas.
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
