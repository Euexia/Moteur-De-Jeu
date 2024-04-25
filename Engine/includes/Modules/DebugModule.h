#pragma once
#include <iostream>

#include "Module.h"


/**
 * @brief Classe repr�sentant un module de d�bogage.
 *
 * Cette classe h�rite de la classe Module et permet de g�rer les fonctionnalit�s de d�bogage dans le jeu.
 */
class DebugModule : public Module
{
	public:

		/**
	 * @brief Destructeur virtuel de la classe DebugModule.
	 *
	 * Ce destructeur virtuel permet une destruction appropri�e des objets de la classe DebugModule lorsqu'ils sont utilis�s polymorphiquement.
	 */
		virtual ~DebugModule();

		/**
		 * @brief �num�ration repr�sentant les types de journaux.
		 *
		 * Cette �num�ration d�finit les diff�rents types de journaux utilis�s pour les messages de d�bogage.
		 */
		enum class TypeLog
		{
			INFO,
			WARNING,
			ERROR,
			SUCCESS
		};

#pragma region Logs

		/**
		 * @brief Enregistre un message de journal avec un niveau de priorit� sp�cifi�.
		 *
		 * Cette m�thode statique enregistre un message de journal avec un niveau de priorit� sp�cifi�.
		 *
		 * @param _message Le message � enregistrer dans le journal.
		 * @param _level Le niveau de priorit� du message (par d�faut: TypeLog::INFO).
		 *        Les valeurs possibles sont TypeLog::INFO, TypeLog::WARNING, TypeLog::ERROR et TypeLog::SUCCESS.
		 */
		static void Log(const std::string& _message, const TypeLog _level = TypeLog::INFO);

		/**
		 * @brief Enregistre un message d'erreur dans le journal.
		 *
		 * Cette m�thode statique enregistre un message d'erreur dans le journal avec le niveau de priorit� TypeLog::ERROR.
		 *
		 * @param _message Le message d'erreur � enregistrer dans le journal.
		 */
		static void LogError(const std::string& _message)
		{
			Log(_message, TypeLog::ERROR);
		}

		/**
		 * @brief Enregistre un message d'avertissement dans le journal.
		 *
		 * Cette m�thode statique enregistre un message d'avertissement dans le journal avec le niveau de priorit� TypeLog::WARNING.
		 *
		 * @param _message Le message d'avertissement � enregistrer dans le journal.
		 */
		static void LogWarning(const std::string& _message)
		{
			Log(_message, TypeLog::WARNING);
		}

		/**
		 * @brief Enregistre un message d'information dans le journal.
		 *
		 * Cette m�thode statique enregistre un message d'information dans le journal avec le niveau de priorit� TypeLog::INFO.
		 *
		 * @param _message Le message d'information � enregistrer dans le journal.
		 */
		static void LogInfo(const std::string& _message)
		{
			Log(_message, TypeLog::INFO);
		}

		/**
		 * @brief Enregistre un message de r�ussite dans le journal.
		 *
		 * Cette m�thode statique enregistre un message de r�ussite dans le journal avec le niveau de priorit� TypeLog::SUCCESS.
		 *
		 * @param _message Le message de r�ussite � enregistrer dans le journal.
		 */
		static void LogSuccess(const std::string& _message)
		{
			Log(_message, TypeLog::SUCCESS);
		}

#pragma endregion

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

};
