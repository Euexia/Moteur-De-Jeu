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
 * Cette classe gère les scènes du jeu, y compris leur chargement, leur création et leur destruction.
 * Elle hérite de la classe de base Module.
 */
class SceneManager final : public Module
{
public:
    /**
     * @brief Constructeur par défaut.
     */
    SceneManager() = default;

    // Désactivation des opérations de copie et de déplacement
    SceneManager(const SceneManager&&) = delete;
    SceneManager(const SceneManager&) = delete;
    SceneManager& operator=(const SceneManager&&) = delete;
    SceneManager& operator=(const SceneManager&) = delete;

    /**
     * @brief Lance une scène spécifique.
     * @param _sceneName Le nom de la scène à lancer.
     */
    void RunScene(const std::string& _sceneName);

    /**
     * @brief Définit la scène principale.
     * @param _sceneName Le nom de la scène principale.
     */
    void SetMainScene(const std::string& _sceneName);

    /**
     * @brief Obtient la scène principale.
     * @return Un pointeur vers la scène principale.
     */
    BaseScene* GetMainScene() const { return mainScene; }

    /**
     * @brief Obtient une scène par son nom.
     * @param _sceneName Le nom de la scène à obtenir.
     * @return Un pointeur vers la scène correspondante, ou nullptr si elle n'existe pas.
     */
    BaseScene* GetScene(const std::string& _sceneName);

    /**
     * @brief Renomme une scène.
     * @param _oldName L'ancien nom de la scène.
     * @param _newName Le nouveau nom de la scène.
     */
    void RenameScene(const std::string& _oldName, const std::string& _newName);

    // Méthodes pour la gestion des scènes
    void CreateScene(std::string _name, bool _isActive);
    void DestroyScene(const std::string& _sceneName);
    bool LoadSceneFromFile(const std::string& _fileName);

    // Méthodes d'information sur les scènes
    std::string GetActiveScene() const;
    std::string GetListScenes() const;
    int SceneCount() const;
    std::pair<std::string, bool> GetSceneAt(int _index);

    // Méthodes pour la gestion de la scène actuelle
    BaseScene* GetCurrentScene() const;
    std::vector<std::unique_ptr<BaseScene>>& GetScenes() { return scenes; }
    void SetCurrentScene(int _sceneIndex);
    void SetNextSceneActive();
    void SetPreviousSceneActive();

    // Méthodes héritées de la classe Module
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
    // Méthodes privées
    bool SceneFileExists(const std::string& _filePath) const;
    GameObject* CreateGameObjectFromSceneData();

    // Membres
    WindowModule* windowModule = nullptr; /**< Module de fenêtre associé. */
    std::map<std::string, bool>             listScenes; /**< Liste des scènes avec leur état d'activation. */
    int                                     sceneCount; /**< Nombre total de scènes. */
    bool                                    sceneActive; /**< Indique si une scène est active. */
    BaseScene* mainScene = nullptr; /**< Scène principale. */
    std::vector<std::unique_ptr<BaseScene>> scenes; /**< Liste des scènes. */
    int                                     currentSceneIndex = -1; /**< Index de la scène actuelle. */
};
