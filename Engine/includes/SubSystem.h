#pragma once
#include <typeindex>
#include <unordered_map>

/**
 * @brief La classe SubSystem représente un sous-système générique.
 *
 * Les sous-systèmes sont des composants autonomes du système global qui peuvent être démarrés et arrêtés dans un ordre spécifique.
 * Ils peuvent être utilisés pour encapsuler des fonctionnalités ou des ressources spécifiques dans une application.
 */
class SubSystem
{
public:
	/**
	 * @brief Constructeur par défaut de SubSystem.
	 */
	SubSystem() = default;

	/**
	 * @brief Destructeur de SubSystem.
	 */
	~SubSystem() = default;

	/**
	 * @brief Démarre tous les sous-systèmes dans un ordre spécifique.
	 */
	static void StartUp();

	/**
	 * @brief Arrête tous les sous-systèmes dans un ordre spécifique.
	 */
	static void ShutDown();

	/**
	 * @brief Définit une instance du type T dans le sous-système.
	 *
	 * @tparam T Le type de l'instance à définir.
	 * @param _instance Un pointeur vers l'instance à définir.
	 */
	template <class T>
	static void Set(T* _instance);

	/**
	 * @brief Récupère l'instance du type T du sous-système.
	 *
	 * @tparam T Le type de l'instance à récupérer.
	 * @return Un pointeur vers l'instance du type T, ou nullptr si aucune instance n'est définie.
	 */
	template <class T>
	static T* Get();

private:
	/**
	 * @brief Map associant les types aux instances correspondantes.
	 */
	static std::unordered_map<std::type_index, void*> instances;
};
