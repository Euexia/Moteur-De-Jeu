#pragma once

// std
#include <cstdint>
#include <stack>
#include <string>
#include <vector>

// Camera
#include "Camera/BaseCamera.h"
#include "Modules/Module.h"

namespace lve
{
	class BaseCamera;

	// TODO: Make inherit from System
	/**
	 * @brief G�re les cam�ras dans le moteur de jeu.
	 *
	 * Cette classe g�re les diff�rentes cam�ras utilis�es dans le moteur de jeu.
	 * Elle permet de cr�er, de supprimer et de manipuler les cam�ras, ainsi que de g�rer les vues et projections associ�es.
	 * La classe CameraManager est une classe finale, ce qui signifie qu'elle ne peut pas �tre d�riv�e.
	 */
	class CameraManager final : public Module
	{
		public:
			/**
			 * @brief Constructeur de la classe CameraManager.
			 *
			 * Ce constructeur initialise un nouvel objet CameraManager.
			 */
			CameraManager();

			/**
			 * @brief Destructeur de la classe CameraManager.
			 *
			 * Ce destructeur lib�re les ressources associ�es � l'objet CameraManager.
			 */
			~CameraManager();

			/**
			 * @brief Appel�e apr�s un changement de sc�ne.
			 *
			 * Cette m�thode est appel�e apr�s qu'une sc�ne a �t� chang�e dans le moteur de jeu.
			 * Elle peut �tre utilis�e pour effectuer des actions sp�cifiques li�es au changement de sc�ne.
			 *
			 * @note Cette m�thode est constante et ne modifie pas l'�tat de l'objet CameraManager.
			 */
			void OnPostSceneChange() const;


			/**
			 * @brief Ajoute une nouvelle cam�ra au gestionnaire de cam�ras.
			 *
			 * Cette m�thode ajoute une nouvelle cam�ra au gestionnaire de cam�ras.
			 *
			 * @param _camera Un pointeur vers la cam�ra � ajouter.
			 * @param _bSwitchTo Indique si la nouvelle cam�ra doit �tre d�finie comme cam�ra actuelle (par d�faut : false).
			 */
			void AddCamera(BaseCamera* _camera, bool _bSwitchTo = false);

			// Clears stack and pushes the given camera onto it, then returns a pointer to it


			/**
			 * @brief Fait passer la cam�ra actuelle � la suivante ou � la pr�c�dente dans la liste des cam�ras g�r�es.
			 *
			 * Cette m�thode fait passer la cam�ra actuelle � la suivante ou � la pr�c�dente dans la liste des cam�ras g�r�es.
			 *
			 * @param _deltaIndex Le delta pour l'index de la cam�ra (positif pour passer � la cam�ra suivante, n�gatif pour passer � la pr�c�dente).
			 * @param _bAlignWithPrevious Indique si la nouvelle cam�ra doit �tre align�e avec la position et l'orientation de la cam�ra pr�c�dente (par d�faut : true).
			 * @return Un pointeur vers la cam�ra actuelle apr�s le changement. Si aucune cam�ra n'est actuellement d�finie ou si l'index sp�cifi� est en dehors de la plage des cam�ras g�r�es, retourne nullptr.
			 */
			BaseCamera* CycleCamera(uint32_t _deltaIndex, bool _bAlignWithPrevious = true);

			/**
			 * @brief Ajoute une nouvelle cam�ra � la pile et la d�finit comme cam�ra actuelle.
			 *
			 * Cette m�thode ajoute une nouvelle cam�ra � la pile des cam�ras g�r�es et la d�finit comme cam�ra actuelle.
			 *
			 * @param _camera Un pointeur vers la cam�ra � ajouter � la pile et � d�finir comme cam�ra actuelle.
			 * @param _bAlignWithPrevious Indique si la nouvelle cam�ra doit �tre align�e avec la position et l'orientation de la cam�ra pr�c�dente (par d�faut : true).
			 * @param _bInitialize Indique si la nouvelle cam�ra doit �tre initialis�e apr�s son ajout (par d�faut : true).
			 * @return Un pointeur vers la cam�ra actuelle apr�s l'ajout. Si _bInitialize est true et que l'initialisation �choue, retourne nullptr.
			 */
			BaseCamera* PushCamera(BaseCamera* _camera, bool _bAlignWithPrevious, bool _bInitialize);

			/**
			 * @brief Ajoute une nouvelle cam�ra par son nom � la pile et la d�finit comme cam�ra actuelle.
			 *
			 * Cette m�thode ajoute une nouvelle cam�ra par son nom � la pile des cam�ras g�r�es et la d�finit comme cam�ra actuelle.
			 *
			 * @param _name Le nom de la cam�ra � ajouter � la pile et � d�finir comme cam�ra actuelle.
			 * @param _bAlignWithPrevious Indique si la nouvelle cam�ra doit �tre align�e avec la position et l'orientation de la cam�ra pr�c�dente (par d�faut : true).
			 * @param _bInitialize Indique si la nouvelle cam�ra doit �tre initialis�e apr�s son ajout (par d�faut : true).
			 * @return Un pointeur vers la cam�ra actuelle apr�s l'ajout. Si _bInitialize est true et que l'initialisation �choue, retourne nullptr.
			 */
			BaseCamera* PushCameraByName(const std::string& _name, bool _bAlignWithPrevious, bool _bInitialize);

			/**
			 * @brief Supprime la cam�ra actuelle de la pile et restaure la cam�ra pr�c�dente.
			 *
			 * Cette m�thode supprime la cam�ra actuelle de la pile des cam�ras g�r�es et restaure la cam�ra pr�c�dente comme cam�ra actuelle.
			 *
			 * @param _bAlignWithCurrent Indique si la nouvelle cam�ra actuelle doit �tre align�e avec la position et l'orientation de la cam�ra supprim�e (par d�faut : false).
			 */
			void PopCamera(bool _bAlignWithCurrent = false);

			#pragma region Getter

			/**
			 * @brief Obtient un pointeur vers la cam�ra actuelle.
			 *
			 * Cette m�thode retourne un pointeur vers la cam�ra actuellement active dans le gestionnaire de cam�ras.
			 *
			 * @return Un pointeur vers la cam�ra actuelle. Si aucune cam�ra n'est actuellement d�finie, retourne nullptr.
			 */
			[[nodiscard]] BaseCamera* GetCurrentCamera() const;

			/**
			 * @brief Obtient un pointeur vers une cam�ra par son nom.
			 *
			 * Cette m�thode recherche une cam�ra par son nom dans le gestionnaire de cam�ras et retourne un pointeur vers la premi�re cam�ra trouv�e avec le nom sp�cifi�.
			 *
			 * @param _name Le nom de la cam�ra � rechercher.
			 * @return Un pointeur vers la cam�ra avec le nom sp�cifi�. Si aucune cam�ra avec ce nom n'est trouv�e, retourne nullptr.
			 */
			[[nodiscard]] BaseCamera* GetCameraByName(const std::string& _name) const;

			#pragma endregion

			#pragma region Setter

			/** @brief D�finit une cam�ra sp�cifique comme cam�ra actuelle.
			 *
			 * Cette m�thode d�finit une cam�ra sp�cifique comme cam�ra actuelle dans le gestionnaire de cam�ras.
			 *
			 * @param _camera Un pointeur vers la cam�ra � d�finir comme cam�ra actuelle.
			 * @param _bAlignWithPrevious Indique si la nouvelle cam�ra doit �tre align�e avec la position et l'orientation de la cam�ra pr�c�dente (par d�faut : true).
			 * @return Un pointeur vers la cam�ra actuelle apr�s le changement. Si la cam�ra sp�cifi�e n'est pas dans la liste des cam�ras g�r�es, retourne nullptr.
			 */
			BaseCamera* SetCamera(BaseCamera* _camera, bool _bAlignWithPrevious);
			/**
			 * @brief D�finit une cam�ra sp�cifique par son nom comme cam�ra actuelle.
			 *
			 * Cette m�thode d�finit une cam�ra sp�cifique par son nom comme cam�ra actuelle dans le gestionnaire de cam�ras.
			 *
			 * @param _name Le nom de la cam�ra � d�finir comme cam�ra actuelle.
			 * @param _bAlignWithPrevious Indique si la nouvelle cam�ra doit �tre align�e avec la position et l'orientation de la cam�ra pr�c�dente (par d�faut : true).
			 * @return Un pointeur vers la cam�ra actuelle apr�s le changement. Si aucune cam�ra avec le nom sp�cifi� n'est trouv�e, retourne nullptr.
			 */
			BaseCamera* SetCameraByName(const std::string& _name, bool _bAlignWithPrevious);
			#pragma endregion

			/**
			 * @brief Affiche les objets ImGui associ�s au gestionnaire de cam�ras.
			 *
			 * Cette m�thode affiche les objets ImGui associ�s au gestionnaire de cam�ras, permettant ainsi de visualiser et de modifier les param�tres des cam�ras.
			 *
			 * @note Cette m�thode est g�n�ralement utilis�e pour afficher une interface utilisateur graphique dans le cadre d'un �diteur de niveau ou d'un mode de d�bogage.
			 */
			void DrawImGuiObjects();

			/**
			 * @brief Aligne les param�tres de position, de rotation et de champ de vision d'une cam�ra source avec une cam�ra cible.
			 *
			 * Cette m�thode copie les param�tres de position, de rotation et de champ de vision d'une cam�ra source vers une cam�ra cible.
			 *
			 * @param _from La cam�ra source � partir de laquelle copier les param�tres.
			 * @param _to La cam�ra cible vers laquelle copier les param�tres.
			 */
			void AlignCameras(const BaseCamera* _from, BaseCamera* _to);


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
			/*EventReply OnActionEvent(Action action, ActionEvent actionEvent);
			ActionCallback<CameraManager> m_ActionCallback;*/

			/**
			 * @brief Obtient l'index d'une cam�ra dans la liste des cam�ras g�r�es.
			 *
			 * Cette m�thode recherche une cam�ra sp�cifi�e dans la liste des cam�ras g�r�es par le gestionnaire de cam�ras
			 * et retourne son index dans la liste.
			 *
			 * @param _camera Un pointeur vers la cam�ra � rechercher.
			 * @return L'index de la cam�ra dans la liste des cam�ras g�r�es. Si la cam�ra sp�cifi�e n'est pas trouv�e, retourne UINT32_MAX.
			 */
			uint32_t GetCameraIndex(const BaseCamera* _camera) const;


			// TODO: Roll custom stack class
			// Stack containing temporary cameras, the topmost of which is the current camera
			// Always contains at least one element
			std::stack<BaseCamera*> cameraStack; ///< Pile contenant les cam�ras g�r�es par le gestionnaire de cam�ras.

			// All cameras, unordered
			std::vector<BaseCamera*> cameras;    ///< Vecteur contenant toutes les cam�ras g�r�es par le gestionnaire de cam�ras, sans ordre sp�cifique.

			bool bInitialized = false;            ///< Bool�en indiquant si le gestionnaire de cam�ras a �t� initialis� ou non.

	};
};
