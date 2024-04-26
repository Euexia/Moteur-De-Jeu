#pragma once
#include <string>
#include "GameObject/GameObject.h"

#ifdef ENGINECORE_EXPORTS
#define ENGINEDLL_API __declspec(dllexport)
#else
#define ENGINEDLL_API __declspec(dllimport)
#endif

class GameObject;

/**
 * @brief Classe de base pour tous les composants attach�s � un GameObject.
 *
 * Les composants sont des entit�s modulaires qui ajoutent des fonctionnalit�s sp�cifiques
 * � un GameObject, telles que la transformation, la physique, le rendu, etc.
 */
class ENGINEDLL_API Component
//class  Component
{
	public:
		Component() = default;
		explicit Component(GameObject* _owner);
		virtual  ~Component() = default;

#pragma region Event
		virtual void Init();
		virtual void Start();
		virtual void FixedUpdate();
		virtual void Update() ;
		virtual void UpdateEditor() ;
		virtual void PreRender();
		virtual void Render();
		virtual void RenderGui();
		virtual void PostRender();
		virtual void Release();
		virtual void Finalize();

#pragma endregion

#pragma region Getter

		/**
		 * @brief Obtient le GameObject propri�taire de ce composant.
		 *
		 * @return Un pointeur vers le GameObject propri�taire de ce composant.
		 */
		GameObject* GetOwner() const { return owner; }

		/**
		 * @brief Obtient le nom de ce composant.
		 *
		 * @return Le nom de ce composant.
		 */
		std::string GetName() const { return name; }

		/**
		 * @brief Obtient l'�tat d'activit� de ce composant.
		 *
		 * @return true si le composant est actif, sinon false.
		 */
		bool GetActive() const { return isActive; }

		/**
		 * @brief Obtient l'�tat de visibilit� de ce composant.
		 *
		 * @return true si le composant est visible, sinon false.
		 */
		bool GetVisible() const { return isVisible; }

#pragma endregion

#pragma region Setter

		/**
		 * @brief D�finit le GameObject propri�taire de ce composant.
		 *
		 * @param _owner Le GameObject � d�finir comme propri�taire de ce composant.
		 */
		void        SetOwner(GameObject* _owner) { owner = _owner; }

		/**
		 * @brief D�finit le nom de ce composant.
		 *
		 * @param _name Le nom � attribuer � ce composant.
		 */
		void        SetName(const std::string& _name) { name = _name; }

		/**
		 * @brief D�finit le nom de ce composant � partir d'une cha�ne de caract�res.
		 *
		 * @param _name La cha�ne de caract�res � utiliser comme nom pour ce composant.
		 */
		void        SetName(const char* _name) { name = _name; }

		/**
		 * @brief D�finit l'�tat d'activit� de ce composant.
		 *
		 * @param _state L'�tat d'activit� � d�finir pour ce composant.
		 */
		void SetActive(const bool& _state) { isActive = _state; }

		/**
		 * @brief D�finit l'�tat de visibilit� de ce composant.
		 *
		 * @param _state L'�tat de visibilit� � d�finir pour ce composant.
		 */
		void SetVisible(const bool& _state) { isVisible = _state; }

		/**
		 * @brief Active ou d�sactive ce composant et le rend visible ou invisible.
		 *
		 * Cette fonction active ou d�sactive ce composant et le rend visible ou invisible en fonction de l'�tat sp�cifi�.
		 *
		 * @param _state L'�tat d'activation et de visibilit� � d�finir.
		 */
		void SetActiveAndVisible(const bool& _state)
		{
			isVisible = _state;
			isActive  = _state;
		}

#pragma endregion

	protected:

		GameObject* owner = nullptr; /**< Pointeur vers le GameObject propri�taire de ce composant. */
		std::string name = "Component"; /**< Nom de ce composant. */
		bool isActive = true; /**< Indique si ce composant est actif. */
		bool isVisible = true; /**< Indique si ce composant est visible. */
};
