#pragma once

// lve
#include "functionHelpers.h"
#include "lve_constants.h"

// Librairies
#include <glm.hpp>

// std
#include <string>

namespace lve
{
	enum class CameraType
	{
		DEBUG_CAM,
		OVERHEAD,
		FIRST_PERSON,
		TERMINAL,
		VEHICLE,

		NONE
	};

	class BaseCamera
	{
		public:
			BaseCamera(std::string _cameraName, CameraType _type, bool _bIsGameplayCam,
			           float       _fov   = glm::radians(45.0f),
			           float       _zNear = 0.5f, float _zFar = 1000.0f);
			virtual ~BaseCamera();

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


			virtual void OnPostSceneChange();

			virtual void OnPossess();
			virtual void OnDepossess();

			[[nodiscard]] glm::mat4 GetViewProjection() const { return viewProjection; }
			[[nodiscard]] glm::mat4 GetView() const { return view; }
			[[nodiscard]] glm::mat4 GetProjection() const { return projection; }

			// speed: Lerp amount to new rotation
			void LookAt(glm::vec3 _point, float _speed = 1.0f);

			void Translate(glm::vec3 _translation);

			void SetViewDirection(float _yawRad, float _pitchRad);

			void ResetPosition();
			void ResetOrientation();

			[[nodiscard]] std::string GetName() const
			{
				return name;
			}

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
			// Sets right, up, and forward based on yaw and pitch
			void CalculateAxisVectorsFromPitchAndYaw();
			void CalculateYawAndPitchFromForward();
			void RecalculateViewProjection();

			void JitterMatrix(glm::mat4& _matrix);
			void ClampPitch();

			// Exposure calculations taken from Google's Filament rendering engine
			// Computes the camera's EV100
			// aperture measured in f-stops
			// shutterSpeed measured in seconds
			// sensitivity measured in ISO
			static float CalculateEv100(float _aperture, float _shutterSpeed, float _sensitivity);

			// Computes the exposure normalization factor from the camera's EV100
			static float ComputeExposureNormFactor(float _ev100);

			bool mBInitialized = false;

			std::string name;

			glm::mat4 view;
			glm::mat4 projection;
			glm::mat4 viewProjection;
	};
} // namespace lve
