#pragma once

#include <string>
#include "lve_window.h"
#include "GameObject/GameObject.h"
/**
 * @brief Interface de base pour les scènes du jeu.
 *
 * Cette classe définit une interface abstraite pour les scènes du jeu.
 * Les classes dérivées sont responsables de définir le comportement spécifique
 * de chaque scène
 */
class BaseScene
{
	public:
		/**
		 * @brief Constructeur explicite de la classe BaseScene.
		 *
		 * Initialise une nouvelle instance de la classe BaseScene avec le nom spécifié.
		 *
		 * @param _fileName Le nom de la scène.
		 */
		explicit BaseScene(const std::string& _fileName) : name(_fileName){}
		~BaseScene() = default;

		BaseScene(const BaseScene&)            = delete;
		BaseScene& operator=(const BaseScene&) = delete;

#pragma region Event

		/**
		 * @brief Initialise le module.
		 */
		virtual void Init();

		/**
		 * @brief D�marre le module.
		 */
		virtual void Start();

		/**
		 * @brief Effectue une mise � jour fixe du module.
		 */
		virtual void FixedUpdate();

		/**
		 * @brief Met � jour le module.
		 */
		virtual void Update();

		virtual void UpdateEditor();

		/**
		 * @brief Fonction pr�-rendu du module.
		 */
		virtual void PreRender();

		/**
		 * @brief Rendu du module.
		 */
		virtual void Render();

		/**
		 * @brief Rendu de l'interface graphique du module.
		 */
		virtual void RenderGui();

		/**
		 * @brief Fonction post-rendu du module.
		 */
		virtual void PostRender();

		/**
		 * @brief Lib�re les ressources utilis�es par le module.
		 */
		virtual void Release();

		/**
		 * @brief Finalise le module.
		 */
		virtual void Finalize();

#pragma endregion

#pragma region Getter
		GameObject* GetGameObjectById(const GameObject::id_t& _gameObjectId) const;

		std::vector<GameObject*> FindGameObjectsByName(const std::string& _name) const;
		std::string GetName() const;
		std::string GetDefaultRelativeFilePath() const;
#pragma endregion
#pragma region Setter
		void        SetName(const std::string& _name);

		bool        SetFileName(const std::string& _fileName, bool _bDeletePreviousFiles) const;

#pragma endregion
#pragma region Create
		GameObject* CreateGameObject();
#pragma endregion
#pragma region Destroy
		void                     DestroyGameObject(const GameObject* _gameObject);
#pragma endregion




		bool IsInitialized() const;

		bool        FileExists(const std::string& _filePath);
		bool        IsUsingSaveFile() const;

		void        DeleteSaveFiles();
		bool        Contains(const std::vector<GameObject::id_t>& _container, const GameObject::id_t& _value);
		std::string GetFileName() const;


		std::vector<GameObject*>& GetRootObjects();

		GameObject* AddRootObject(GameObject* _gameObject);
		GameObject* AddRootObjectImmediate(GameObject* _gameObject);
		GameObject* AddChildObject(GameObject* _parent, GameObject* _child);
		GameObject* AddChildObjectImmediate(GameObject* _parent, GameObject* _child);
		GameObject* AddSiblingObjectImmediate(GameObject* _gameObject, GameObject* _newSibling);

		void SetRootObjectIndex(GameObject* _rootObject, uint32_t _newIndex);


		void RemoveAllObjects();          // Removes and destroys all objects in scene at end of frame
		void RemoveAllObjectsImmediate(); // Removes and destroys all objects in scene
		void RemoveAllEditorObjectsImmediate();
		void RemoveObject(const GameObject::id_t& _gameObjectId, bool _bDestroy);
		void RemoveObject(const GameObject* _gameObject, bool _bDestroy);
		//void RemoveObjectImmediate(const GameObject::id_t& _gameObjectID, bool _bDestroy);
		//void RemoveObjectImmediate(GameObject* _gameObject, bool _bDestroy);
		void RemoveObjects(const std::vector<GameObject::id_t>& _gameObjects, bool _bDestroy);
		void RemoveObjects(const std::vector<GameObject*>& _gameObjects, bool _bDestroy);
		void RemoveObjectsImmediate(const std::vector<GameObject::id_t>& _gameObjects, bool _bDestroy);
		void RemoveObjectsImmediate(const std::vector<GameObject*>& _gameObjects, bool _bDestroy);

		GameObject::id_t FirstObjectWithTag(const std::string& _tag);

		void TestLoadGameObjects();


		std::vector<GameObject*> GetAllGameObject() {
			std::vector<GameObject*> allGameObject;
			for (const auto& gameObject : rootObjects) {
				AddGameObjectInVector(allGameObject, gameObject);
			}
			return allGameObject;
		}


		void AddGameObjectInVector(std::vector<GameObject*>& _gameObjects, GameObject* _gameObject) {
			_gameObjects.push_back(_gameObject);
			for (auto& gameObject : _gameObject->GetChildren()) {
				AddGameObjectInVector(_gameObjects, gameObject);
			}
		}

		std::string name;
		std::string fileName;

		std::vector<GameObject::id_t> pendingDestroyObjects; // Objects to destroy at LateUpdate this frame
		std::vector<GameObject*>      pendingAddObjects;     // Objects to add as root objects at LateUpdate
		std::vector<GameObject*>      rootObjects;
		std::vector<GameObject::id_t> pendingRemoveObjects;

		const GameObject::id_t invalidGameObjectId = {};


		bool bInitialized = false;
		bool bLoaded      = false;

		GameObject* CreateCubeGameObject(int _type = 0);
		GameObject* CreateLightGameObject();
		GameObject* CreatePlaneGameObject();
		GameObject* CreateVaseGameObject(int _type = 0);
};
