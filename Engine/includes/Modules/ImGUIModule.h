#pragma once

#include <functional>
#include <imgui.h>
#include "Module.h"
#include "Modules/WindowModule.h"
#include "Scene/SceneManager.h"

class RHIModule;

/**
 * @brief Classe d�di�e � la gestion de l'interface utilisateur avec ImGui dans le contexte du module de rendu.
 */
class ImGuiModule final : public Module
{
public:

	//TimeModule* timeModule = nullptr;
	vk::Fence         immFence;
	vk::CommandBuffer immCommandBuffer;
	vk::CommandPool   immCommandPool;

	/**
	 * @brief Initialise le module ImGui, pr�parant les ressources n�cessaires pour son fonctionnement.
	 */
	void Init() override;

	/**
	 * @brief D�marre le module ImGui et pr�pare l'interface utilisateur pour l'interaction.
	 */
	void Start() override;

	/**
	 * @brief Ex�cute des mises � jour r�guli�res � intervalles fixes, typiquement pour la logique de jeu.
	 */
	void FixedUpdate() override;

	/**
	 * @brief Met � jour l'�tat du module ImGui avant le rendu.
	 */
	void Update() override;

	/**
	 * @brief Pr�pare le module ImGui pour le rendu, mais avant que le rendu ne soit effectu�.
	 */
	void PreRender() override;

	/**
	 * @brief Ex�cute le rendu de l'interface utilisateur ImGui.
	 */
	void Render() override;

	/**
	 * @brief Dessine l'interface graphique sp�cifique au module, comme les menus et les fen�tres.
	 */
	void RenderGui() override;

	/**
	 * @brief Op�rations � effectuer apr�s le rendu principal, souvent utilis�es pour les nettoyages.
	 */
	void PostRender() override;

	/**
	 * @brief Lib�re les ressources utilis�es par le module ImGui.
	 */
	void Release() override;

	/**
	 * @brief Nettoie l'�tat du module ImGui et ferme l'interface utilisateur.
	 */
	void Finalize() override;

	/**
	 * @brief Construit les composants de l'interface utilisateur � �tre affich�s.
	 */
	void GetGui();

	/**
	 * @brief Soumet une commande graphique imm�diate � ex�cuter.
	 * @param _function La fonction qui contient les commandes graphiques � ex�cuter.
	 */
	void ImmediateSubmit(std::function<void(vk::CommandBuffer _cmd)>&& _function) const;

	/**
	 * @brief Dessine la hi�rarchie des objets de la sc�ne dans l'interface utilisateur.
	 */
	void DrawHierarchy();

	/**
	 * @brief Dessine l'inspecteur des propri�t�s pour l'objet s�lectionn�.
	 */
	void DrawInspector();

	/**
	 * @brief Affiche et permet l'�dition des propri�t�s de transformation d'un objet.
	 * @param _transform Pointeur vers l'objet Transform � afficher et �diter.
	 */
	void DisplayTransform(Transform* _transform);

	/**
	 * @brief Fixe la fen�tre de l'interface utilisateur � une position sp�cifique.
	 * @param _windowName Nom de la fen�tre � ancrer.
	 */
	void AnchorWindow(const std::string& _windowName);

	/**
	 * @brief Dessine les param�tres de l'interface utilisateur du moteur de jeu.
	 */
	void DrawEngineGUISettings();

	/**
	 * @brief Affiche la popup de renommage pour l'entit� s�lectionn�e.
	 */
	void ShowRenamePopup();

	/**
	 * @brief Renomme l'objet de jeu s�lectionn�.
	 * @param _gameObject Pointeur vers l'objet de jeu � renommer.
	 * @param _newName Nouveau nom � attribuer � l'objet de jeu.
	 */
	void RenameGameObject(GameObject* _gameObject, const std::string& _newName);

	/**
	 * @brief Supprime l'objet de jeu s�lectionn� de la sc�ne.
	 * @param _gameObject Pointeur vers l'objet de jeu � supprimer.
	 */
	void DeleteGameObject(GameObject* _gameObject);

	/**
	 * @brief Duplique l'objet de jeu situ� � l'index sp�cifi�.
	 * @param _index Index de l'objet de jeu � dupliquer.
	 */
	void DuplicateGameObject(int _index);

protected:
	vk::Device device; ///< P�riph�rique utilis� pour le rendu avec Vulkan.
	vk::Queue graphicsQueue; ///< File d'attente graphique pour la soumission des commandes Vulkan.

	WindowModule* windowModule = nullptr; ///< Module de gestion de la fen�tre de l'application.
	RHIModule* rhiModule = nullptr; ///< Module de gestion du rendu bas-niveau (RHI).
	SceneManager* sceneManager = nullptr; ///< Gestionnaire de sc�nes de l'application.

	GameObject* selectedGameObject = nullptr; ///< Pointeur vers l'objet de jeu actuellement s�lectionn�.

	bool isRenamePopupOpen = false; ///< Indique si la popup de renommage est ouverte ou non.
	int entityToRename = -1; ///< Index de l'entit� � renommer.
	char renameBuffer[256]; ///< Buffer pour le nouveau nom de l'entit� � renommer.
	int sceneToRename = -1; ///< Index de la sc�ne � renommer.
	char renameSceneBuffer[100]; ///< Buffer pour le nouveau nom de la sc�ne � renommer.

	bool openPositionEdit = false; ///< Indique si l'�dition de la position est ouverte ou non.
	bool openScaleEdit = false; ///< Indique si l'�dition de l'�chelle est ouverte ou non.
	glm::vec3 positionEdit; ///< Stocke la position actuelle pour l'�dition.
	float rotationEdit; ///< Stocke la rotation actuelle pour l'�dition.
	glm::vec3 scaleEdit; ///< Stocke l'�chelle actuelle pour l'�dition.

	std::vector<bool> isOpen; ///< Vecteur pour suivre l'�tat ouvert/ferm� des diff�rents panneaux de l'interface utilisateur.

	/**
	 * @brief Destructeur par d�faut.
	 */
	~ImGuiModule() = default;
};
