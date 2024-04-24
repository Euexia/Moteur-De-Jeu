#pragma once
#include <filesystem>
#include <iostream>
#include <map>
#include <string>
#include "Modules/Module.h"
#include "Modules/WindowModule.h"
#include "Scene/BaseScene.h"

/**
 * @brief Classe SceneManager.
 *
 * Cette classe g�re les sc�nes du jeu, y compris leur chargement, leur cr�ation et leur destruction.
 * Elle h�rite de la classe de base Module.
 */
class SceneManager final : public Module
{
public:
    /**
     * @brief Constructeur par d�faut.
     */
    SceneManager() = default;

    // D�sactivation des op�rations de copie et de d�placement
    SceneManager(const SceneManager&&) = delete;
    SceneManager(const SceneManager&) = delete;
    SceneManager& operator=(const SceneManager&&) = delete;
    SceneManager& operator=(const SceneManager&) = delete;

    /**
     * @brief Lance une sc�ne sp�cifique.
     * @param _sceneName Le nom de la sc�ne � lancer.
     */
    void RunScene(const std::string& _sceneName);

    /**
     * @brief D�finit la sc�ne principale.
     * @param _sceneName Le nom de la sc�ne principale.
     */
    void SetMainScene(const std::string& _sceneName);

    /**
     * @brief Obtient la sc�ne principale.
     * @return Un pointeur vers la sc�ne principale.
     */
    BaseScene* GetMainScene() const { return mainScene; }

    /**
     * @brief Obtient une sc�ne par son nom.
     * @param _sceneName Le nom de la sc�ne � obtenir.
     * @return Un pointeur vers la sc�ne correspondante, ou nullptr si elle n'existe pas.
     */
    BaseScene* GetScene(const std::string& _sceneName);

    /**
     * @brief Renomme une sc�ne.
     * @param _oldName L'ancien nom de la sc�ne.
     * @param _newName Le nouveau nom de la sc�ne.
     */
    void RenameScene(const std::string& _oldName, const std::string& _newName);

    // M�thodes pour la gestion des sc�nes
    void CreateScene(std::string _name, bool _isActive);
    void DestroyScene(const std::string& _sceneName);
    bool LoadSceneFromFile(const std::string& _fileName);

    // M�thodes d'information sur les sc�nes
    std::string GetActiveScene() const;
    std::string GetListScenes() const;
    int SceneCount() const;
    std::pair<std::string, bool> GetSceneAt(int _index);

    // M�thodes pour la gestion de la sc�ne actuelle
    BaseScene* GetCurrentScene() const;
    std::vector<std::unique_ptr<BaseScene>>& GetScenes() { return scenes; }
    void SetCurrentScene(int _sceneIndex);
    void SetNextSceneActive();
    void SetPreviousSceneActive();

    // M�thodes h�rit�es de la classe Module
    void Init() override;
    void Start() override;
    void FixedUpdate() override;
    void Update() override;
    void PreRender() override;
    void Render() override;
    void RenderGui() override;
    void PostRender() override;
    void Release() override;
    void Finalize() override;

private:
    // M�thodes priv�es
    bool SceneFileExists(const std::string& _filePath) const;
    GameObject* CreateGameObjectFromSceneData();

    // Membres
    WindowModule* windowModule = nullptr; /**< Module de fen�tre associ�. */
    std::map<std::string, bool>             listScenes; /**< Liste des sc�nes avec leur �tat d'activation. */
    int                                     sceneCount; /**< Nombre total de sc�nes. */
    bool                                    sceneActive; /**< Indique si une sc�ne est active. */
    BaseScene* mainScene = nullptr; /**< Sc�ne principale. */
    std::vector<std::unique_ptr<BaseScene>> scenes; /**< Liste des sc�nes. */
    int                                     currentSceneIndex = -1; /**< Index de la sc�ne actuelle. */
};
