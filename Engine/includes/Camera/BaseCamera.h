#pragma once

// lve
#include "LveEngine/lve_constants.h"

// Librairies
#include <glm.hpp>

// std
#include <string>

namespace lve
{
	/**
 * @brief Enum�ration d�crivant les diff�rents types de cam�ras.
 *
 * Cette �num�ration d�finit les diff�rents types de cam�ras pouvant �tre utilis�s dans le moteur de jeu.
 */
	enum class CameraType
	{
		DEBUG_CAM,
		///< Cam�ra de d�bogage.
		OVERHEAD,
		///< Vue en hauteur.
		FIRST_PERSON,
		///< Vue � la premi�re personne.
		TERMINAL,
		///< Cam�ra de terminal.
		VEHICLE,
		///< Cam�ra de v�hicule.
		NONE ///< Aucun type sp�cifi�.
	};

	/**
 * @brief Classe de base pour une cam�ra dans un moteur de jeu.
 *
 * Cette classe d�finit les fonctionnalit�s de base d'une cam�ra dans un moteur de jeu.
 */
	class BaseCamera
	{
		public:
			/**
 * @brief Constructeur de la classe BaseCamera.
 *
 * @param _cameraName Nom de la cam�ra.
 * @param _type Type de la cam�ra.
 * @param _bIsGameplayCam Bool�en indiquant si c'est une cam�ra de gameplay.
 * @param _fov Champ de vision de la cam�ra en radians (par d�faut : 45 degr�s).
 * @param _zNear Distance du plan de coupe avant (par d�faut : 0.5 unit�s).
 * @param _zFar Distance du plan de coupe arri�re (par d�faut : 1000.0 unit�s).
 */
			BaseCamera(std::string _cameraName, CameraType _type, bool _bIsGameplayCam,
			           float       _fov   = glm::radians(45.0f),
			           float       _zNear = 0.5f, float _zFar = 1000.0f);

			/**
			 * @brief Destructeur virtuel de la classe BaseCamera.
			 */
			virtual ~BaseCamera();

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
			virtual void FixedUpdate(const float& _deltaTime);

			/**
			 * @brief Met � jour le module.
			 */
			virtual void Update(const float& _deltaTime);

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

			/**
 * @brief Appel�e apr�s un changement de sc�ne.
 *
 * Cette fonction est appel�e apr�s qu'une sc�ne a �t� chang�e dans le moteur de jeu.
 * Elle peut �tre utilis�e pour effectuer des actions sp�cifiques li�es au changement de sc�ne.
 * Par d�faut, cette fonction ne fait rien et doit �tre red�finie dans les classes d�riv�es selon les besoins.
 */
			virtual void OnPostSceneChange();

			/**
			 * @brief Appel�e lorsqu'un objet est poss�d� par cette cam�ra.
			 *
			 * Cette fonction est appel�e lorsqu'un objet est poss�d� par cette cam�ra dans le moteur de jeu.
			 * Elle peut �tre utilis�e pour effectuer des actions sp�cifiques lors de la possession d'un objet par la cam�ra.
			 * Par d�faut, cette fonction ne fait rien et doit �tre red�finie dans les classes d�riv�es selon les besoins.
			 */
			virtual void OnPossess();

			/**
			 * @brief Appel�e lorsqu'un objet est d�poss�d� par cette cam�ra.
			 *
			 * Cette fonction est appel�e lorsqu'un objet est d�poss�d� par cette cam�ra dans le moteur de jeu.
			 * Elle peut �tre utilis�e pour effectuer des actions sp�cifiques lors de la d�possession d'un objet par la cam�ra.
			 * Par d�faut, cette fonction ne fait rien et doit �tre red�finie dans les classes d�riv�es selon les besoins.
			 */
			virtual void OnDepossess();

			/**
 * @brief Obtient la matrice de vue-projection.
 *
 * Cette fonction retourne la matrice de vue-projection associ�e � cette cam�ra.
 *
 * @return La matrice de vue-projection.
 */
			[[nodiscard]] glm::mat4 GetViewProjection() const { return viewProjection; }

			/**
			 * @brief Obtient la matrice de vue.
			 *
			 * Cette fonction retourne la matrice de vue associ�e � cette cam�ra.
			 *
			 * @return La matrice de vue.
			 */
			[[nodiscard]] glm::mat4 GetView() const { return view; }

			/**
			 * @brief Obtient la matrice de projection.
			 *
			 * Cette fonction retourne la matrice de projection associ�e � cette cam�ra.
			 *
			 * @return La matrice de projection.
			 */
			[[nodiscard]] glm::mat4 GetProjection() const { return projection; }


			/**
 * @brief Oriente la cam�ra vers un point donn�.
 *
 * Cette fonction oriente la cam�ra vers un point donn� dans l'espace.
 *
 * @param _point Le point vers lequel orienter la cam�ra.
 * @param _speed La vitesse � laquelle effectuer l'orientation (par d�faut : 1.0).
 */
			void LookAt(glm::vec3 _point, float _speed = 1.0f); // speed: Lerp amount to new rotation

			/**
			 * @brief Translate la cam�ra selon un vecteur de translation donn�.
			 *
			 * Cette fonction translate la position de la cam�ra selon un vecteur de translation donn�.
			 *
			 * @param _translation Le vecteur de translation � appliquer � la position de la cam�ra.
			 */
			void Translate(glm::vec3 _translation);

			/**
			 * @brief D�finit la direction de vue de la cam�ra en utilisant des angles de tangage et de lacet.
			 *
			 * Cette fonction d�finit la direction de vue de la cam�ra en utilisant des angles de tangage (yaw) et de lacet (pitch)
			 * sp�cifi�s en radians.
			 *
			 * @param _yawRad   L'angle de lacet en radians.
			 * @param _pitchRad L'angle de tangage en radians.
			 */
			void SetViewDirection(float _yawRad, float _pitchRad);


			/**
 * @brief R�initialise la position de la cam�ra.
 *
 * Cette fonction r�initialise la position de la cam�ra � sa position par d�faut.
 */
			void ResetPosition();

			/**
			 * @brief R�initialise l'orientation de la cam�ra.
			 *
			 * Cette fonction r�initialise l'orientation de la cam�ra � son orientation par d�faut.
			 */
			void ResetOrientation();

			/**
			 * @brief Obtient le nom de la cam�ra.
			 *
			 * Cette fonction retourne le nom de la cam�ra.
			 *
			 * @return Le nom de la cam�ra.
			 */
			[[nodiscard]] std::string GetName() const { return name; }

			/**
			 * @brief Calcule l'exposition de la cam�ra.
			 *
			 * Cette fonction calcule l'exposition de la cam�ra en fonction de son environnement ou de ses param�tres internes.
			 * Elle peut �tre utilis�e pour ajuster automatiquement les param�tres de la cam�ra en fonction de la luminosit� ambiante ou d'autres facteurs.
			 */
			void CalculateExposure();


			// Contr�le de l'exposition
			float aperture         = 1.0f;        /**< Ouverture du diaphragme en f-stops. */
			float shutterSpeed     = 1.0f / 8.0f; /**< Vitesse d'obturation en secondes. */
			float lightSensitivity = 800.0f;      /**< Sensibilit� ISO. */
			float exposure         = 0.0f;        /**< Exposition. */

			float fov   = 0.0f; /**< Champ de vision (Field of View). */
			float zNear = 0.0f; /**< Plan rapproch� du volume de vue. */
			float zFar  = 0.0f; /**< Plan �loign� du volume de vue. */

			float moveSpeed = 0.0f;
			/**< Vitesse de d�placement (WASD ou stick analogique gauche de la manette de jeu). */
			float panSpeed = 0.0f; /**< Vitesse de panoramique (bouton du milieu de la souris). */
			float dragDollySpeed = 0.0f; /**< Vitesse de d�placement par glisser (bouton droit de la souris). */
			float scrollDollySpeed = 0.0f; /**< Vitesse de d�placement par d�filement (molette de la souris). */
			float orbitingSpeed = 0.0f; /**< Vitesse de rotation orbitale (Alt + glisser bouton gauche de la souris). */
			float mouseRotationSpeed = 0.0f;
			/**< Vitesse de rotation de la souris (glisser bouton gauche de la souris). */
			float gamepadRotationSpeed    = 0.0f; /**< Vitesse de rotation du stick droit de la manette de jeu. */
			float moveSpeedFastMultiplier = 0.0f; /**< Multiplicateur de vitesse de d�placement rapide. */
			float moveSpeedSlowMultiplier = 0.0f; /**< Multiplicateur de vitesse de d�placement lente. */
			float turnSpeedFastMultiplier = 0.0f; /**< Multiplicateur de vitesse de rotation rapide. */
			float turnSpeedSlowMultiplier = 0.0f; /**< Multiplicateur de vitesse de rotation lente. */
			float panSpeedFastMultiplier  = 0.0f; /**< Multiplicateur de vitesse de panoramique rapide. */
			float panSpeedSlowMultiplier  = 0.0f; /**< Multiplicateur de vitesse de panoramique lente. */
			float rollRestorationSpeed    = 0.0f; /**< Vitesse de restauration du roulis. */

			glm::vec3 position;     /**< Position de la cam�ra. */
			glm::vec3 prevPosition; /**< Position pr�c�dente de la cam�ra. */
			glm::vec3 velocity;     /**< V�locit� de la cam�ra. */

			float     yaw;     /**< Angle de lacet (yaw). */
			float     pitch;   /**< Angle de tangage (pitch). */
			float     roll;    /**< Angle de roulis (roll). */
			glm::vec3 forward; /**< Vecteur de direction avant de la cam�ra. */
			glm::vec3 up;      /**< Vecteur de direction vers le haut de la cam�ra. */
			glm::vec3 right;   /**< Vecteur de direction droite de la cam�ra. */

			bool bIsGameplayCam = true;  /**< Indique si la cam�ra est en mode gameplay. */
			bool bIsFirstPerson = false; /**< Indique si la cam�ra est en mode premi�re personne. */
			bool bDebugCyclable = true;  /**< Indique si le mode de d�bogage est cyclable. */
			bool bPossessPlayer = false; /**< Indique si le joueur est poss�d� par la cam�ra. */


			CameraType type;

		protected:
			/**
 * @brief Calcule les vecteurs d'axe � partir des angles de tangage et de lacet.
 *
 * Cette fonction calcule les vecteurs d'axe (avant, haut et droite) � partir des angles de tangage (pitch) et de lacet (yaw)
 * actuellement d�finis pour la cam�ra.
 */
			void CalculateAxisVectorsFromPitchAndYaw(); // Sets right, up, and forward based on yaw and pitch

			/**
			 * @brief Calcule les angles de tangage et de lacet � partir de la direction avant de la cam�ra.
			 *
			 * Cette fonction calcule les angles de tangage (pitch) et de lacet (yaw) � partir de la direction avant actuelle de la cam�ra.
			 */
			void CalculateYawAndPitchFromForward();

			/**
			 * @brief Recalcule la matrice de vue-projection.
			 *
			 * Cette fonction recalcule la matrice de vue-projection de la cam�ra en fonction de ses param�tres internes, tels que la position et l'orientation.
			 * Elle est g�n�ralement appel�e apr�s avoir modifi� la position ou l'orientation de la cam�ra pour mettre � jour la vue et la projection.
			 */
			void RecalculateViewProjection();


			/**
 * @brief Applique un jitter � une matrice donn�e.
 *
 * Cette fonction applique un jitter � une matrice de transformation donn�e.
 * Le jitter peut �tre utilis� pour ajouter un l�ger d�calage � la matrice, souvent utilis� pour cr�er un effet de tremblement ou de mouvement al�atoire.
 *
 * @param _matrix La matrice � laquelle appliquer le jitter. La matrice est modifi�e directement.
 */
			void JitterMatrix(glm::mat4& _matrix);

			/**
			 * @brief Limite l'angle de tangage de la cam�ra.
			 *
			 * Cette fonction limite l'angle de tangage (pitch) de la cam�ra dans une plage acceptable.
			 * Elle est utilis�e pour s'assurer que la cam�ra ne d�passe pas des limites d�finies par l'utilisateur ou par le moteur de jeu.
			 */
			void ClampPitch();


			// Exposure calculations taken from Google's Filament rendering engine
			// Computes the camera's EV100
			// aperture measured in f-stops
			// shutterSpeed measured in seconds
			// sensitivity measured in ISO

			/**
 * @brief Calcule la valeur EV100 � partir des param�tres de l'appareil photo.
 *
 * Cette fonction calcule la valeur EV100 (exposition relative � 100 ISO et une seconde de temps d'exposition) � partir des param�tres de l'appareil photo :
 * l'ouverture de l'objectif, la vitesse d'obturation et la sensibilit� ISO.
 *
 * @param _aperture L'ouverture de l'objectif de l'appareil photo.
 * @param _shutterSpeed La vitesse d'obturation de l'appareil photo.
 * @param _sensitivity La sensibilit� ISO de l'appareil photo.
 * @return La valeur EV100 calcul�e.
 */
			static float CalculateEv100(float _aperture, float _shutterSpeed, float _sensitivity);

			/**
			 * @brief Calcule le facteur de normalisation d'exposition � partir de la valeur EV100 de la cam�ra.
			 *
			 * Cette fonction calcule le facteur de normalisation de l'exposition � partir de la valeur EV100 de la cam�ra.
			 * Le facteur de normalisation est utilis� pour ajuster automatiquement les param�tres d'exposition de la cam�ra en fonction de la luminosit� ambiante ou d'autres facteurs.
			 *
			 * @param _ev100 La valeur EV100 de la cam�ra.
			 * @return Le facteur de normalisation de l'exposition calcul�.
			 */
			static float ComputeExposureNormFactor(float _ev100);
			// Computes the exposure normalization factor from the camera's EV100


			bool mBInitialized = false; ///< Bool�en indiquant si la cam�ra a �t� initialis�e ou non.

			std::string name; ///< Nom de la cam�ra.

			glm::mat4 view;           ///< Matrice de vue de la cam�ra.
			glm::mat4 projection;     ///< Matrice de projection de la cam�ra.
			glm::mat4 viewProjection; ///< Matrice de vue-projection combinant la vue et la projection.
	};
} // namespace lve
