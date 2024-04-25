#pragma once
#include <filesystem>
#include <map>
#include <string>

#include "Modules/Module.h"
#include "Modules/WindowModule.h"
#include "Scene/BaseScene.h"

/**
 * @brief Gestionnaire de sc�ne.
 *
 * G�re les op�rations li�es aux sc�nes, telles que la cr�ation, la destruction et la manipulation des objets de la sc�ne.
 */
class SceneManager final : public Module
{
	public:
		SceneManager() = default;

		SceneManager(const SceneManager&&)            = delete;
		SceneManager(const SceneManager&)             = delete;
		SceneManager& operator=(const SceneManager&&) = delete;
		SceneManager& operator=(const SceneManager&)  = delete;


#pragma region Getter

		/**
		 * @brief Obtient la sc�ne principale.
		 *
		 * @return Un pointeur vers la sc�ne principale.
		 */
		[[nodiscard]] BaseScene* GetMainScene() const { return mainScene; }

		/**
		 * @brief Obtient une sc�ne sp�cifi�e par son nom.
		 *
		 * @param _sceneName Le nom de la sc�ne � r�cup�rer.
		 * @return Un pointeur vers la sc�ne demand�e, ou nullptr si elle n'est pas trouv�e.
		 */
		BaseScene* GetScene(const std::string& _sceneName);

		/**
		 * @brief Obtient le nom de la sc�ne active.
		 *
		 * @return Le nom de la sc�ne active.
		 */
		std::string GetActiveScene() const;

		/**
		 * @brief Obtient une liste des noms de toutes les sc�nes disponibles.
		 *
		 * @return Une cha�ne contenant les noms de toutes les sc�nes disponibles.
		 */
		std::string GetListScenes() const;

		/**
		 * @brief Obtient la sc�ne � l'index sp�cifi�.
		 *
		 * @param _index L'index de la sc�ne � r�cup�rer.
		 * @return Une paire contenant le nom de la sc�ne et un bool�en indiquant si elle est active.
		 */
		std::pair<std::string, bool> GetSceneAt(int _index);

		/**
		 * @brief Obtient la sc�ne actuellement active.
		 *
		 * @return Un pointeur vers la sc�ne actuellement active.
		 */
		BaseScene* GetCurrentScene() const;

		/**
		 * @brief Obtient une r�f�rence � la liste des sc�nes.
		 *
		 * @return Une r�f�rence � la liste des sc�nes.
		 */
		std::vector<std::unique_ptr<BaseScene>>& GetScenes() { return scenes; }

#pragma endregion

#pragma region Setter

		/**
		 * @brief D�finit la sc�ne principale.
		 *
		 * @param _sceneName Le nom de la sc�ne principale.
		 */
		void       SetMainScene(const std::string& _sceneName);

		/**
			 * @brief D�finit la sc�ne � activer en fonction de son index dans la liste des sc�nes.
			 *
			 * @param _sceneIndex L'index de la sc�ne � activer.
			 */
		void SetCurrentScene(int _sceneIndex);

		/**
		 * @brief Active la sc�ne suivante dans la liste des sc�nes.
		 */
		void SetNextSceneActive();

		/**
		 * @brief Active la sc�ne pr�c�dente dans la liste des sc�nes.
		 */
		void SetPreviousSceneActive();

#pragma endregion 

		/**
		 * @brief Ex�cute une sc�ne sp�cifi�e.
		 *
		 * @param _sceneName Le nom de la sc�ne � ex�cuter.
		 */
		void RunScene(const std::string& _sceneName);


		/**
		 * @brief Renomme une sc�ne.
		 *
		 * @param _oldName Le nom actuel de la sc�ne.
		 * @param _newName Le nouveau nom de la sc�ne.
		 */
		void       RenameScene(const std::string& _oldName, const std::string& _newName);

		/**
		 * @brief Cr�e une nouvelle sc�ne avec le nom sp�cifi�.
		 *
		 * @param _name Le nom de la nouvelle sc�ne.
		 * @param _isActive Indique si la nouvelle sc�ne doit �tre active ou non.
		 */
		void CreateScene(std::string _name, bool _isActive);

		/**
		 * @brief D�truit une sc�ne sp�cifi�e par son nom.
		 *
		 * @param _sceneName Le nom de la sc�ne � d�truire.
		 */
		void DestroyScene(const std::string& _sceneName);

		/**
		 * @brief Charge une sc�ne � partir d'un fichier avec le nom sp�cifi�.
		 *
		 * @param _fileName Le nom du fichier de la sc�ne � charger.
		 * @return true si la sc�ne a �t� charg�e avec succ�s, sinon false.
		 */
		bool LoadSceneFromFile(const std::string& _fileName);

		/**
		 * @brief Obtient le nombre total de sc�nes.
		 *
		 * @return Le nombre total de sc�nes.
		 */
		int SceneCount() const;

#pragma region Event

		/**
			* @brief Initialise le module.
			*/
		void Init() override;

		/**
		 * @brief D�marre le module.
		 */
		void Start() override;

		/**
		 * @brief Effectue une mise � jour fixe du module.
		 */
		void FixedUpdate() override;

		/**
		 * @brief Met � jour le module.
		 */
		void Update() override;

		/**
		 * @brief Fonction pr�-rendu du module.
		 */
		void PreRender() override;

		/**
		 * @brief Rendu du module.
		 */
		void Render() override;

		/**
		 * @brief Rendu de l'interface graphique du module.
		 */
		void RenderGui() override;

		/**
		 * @brief Fonction post-rendu du module.
		 */
		void PostRender() override;

		/**
		 * @brief Lib�re les ressources utilis�es par le module.
		 */
		void Release() override;

		/**
		 * @brief Finalise le module.
		 */
		void Finalize() override;

#pragma endregion

	private:

		/**
		 * @brief V�rifie si le fichier de la sc�ne existe.
		 *
		 * @param _filePath Le chemin du fichier de la sc�ne.
		 * @return true si le fichier de la sc�ne existe, sinon false.
		 */
		bool SceneFileExists(const std::string& _filePath) const;

		/**
		 * @brief Cr�e un GameObject � partir des donn�es de la sc�ne.
		 *
		 * @return Un pointeur vers le GameObject cr��.
		 */
		GameObject* CreateGameObjectFromSceneData();

		WindowModule* windowModule = nullptr; /**< Le module de la fen�tre associ� au SceneManager. */

		std::map<std::string, bool> listScenes; /**< La liste des noms de sc�nes avec leur �tat actif. */

		int sceneCount; /**< Le nombre total de sc�nes. */

		bool sceneActive; /**< Indique si une sc�ne est active. */

		BaseScene* mainScene = nullptr; /**< La sc�ne principale du SceneManager. */

		std::vector<std::unique_ptr<BaseScene>> scenes; /**< Les sc�nes g�r�es par le SceneManager. */

		int currentSceneIndex = -1; /**< L'index de la sc�ne actuellement active. */

};
