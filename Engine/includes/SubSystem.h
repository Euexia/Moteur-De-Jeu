#pragma once
#include <typeindex>
#include <unordered_map>

/**
 * @brief La classe SubSystem repr�sente un sous-syst�me g�n�rique.
 *
 * Les sous-syst�mes sont des composants autonomes du syst�me global qui peuvent �tre d�marr�s et arr�t�s dans un ordre sp�cifique.
 * Ils peuvent �tre utilis�s pour encapsuler des fonctionnalit�s ou des ressources sp�cifiques dans une application.
 */
class SubSystem
{
public:
	/**
	 * @brief Constructeur par d�faut de SubSystem.
	 */
	SubSystem() = default;

	/**
	 * @brief Destructeur de SubSystem.
	 */
	~SubSystem() = default;

	/**
	 * @brief D�marre tous les sous-syst�mes dans un ordre sp�cifique.
	 */
	static void StartUp();

	/**
	 * @brief Arr�te tous les sous-syst�mes dans un ordre sp�cifique.
	 */
	static void ShutDown();

	/**
	 * @brief D�finit une instance du type T dans le sous-syst�me.
	 *
	 * @tparam T Le type de l'instance � d�finir.
	 * @param _instance Un pointeur vers l'instance � d�finir.
	 */
	template <class T>
	static void Set(T* _instance);

	/**
	 * @brief R�cup�re l'instance du type T du sous-syst�me.
	 *
	 * @tparam T Le type de l'instance � r�cup�rer.
	 * @return Un pointeur vers l'instance du type T, ou nullptr si aucune instance n'est d�finie.
	 */
	template <class T>
	static T* Get();

private:
	/**
	 * @brief Map associant les types aux instances correspondantes.
	 */
	static std::unordered_map<std::type_index, void*> instances;
};
