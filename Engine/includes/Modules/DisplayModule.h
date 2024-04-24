#pragma once
#include "Modules/Module.h"


/**
 * @brief Classe DisplayModule.
 *
 * Cette classe repr�sente un module de d�bogage dans le moteur de jeu.
 * Elle h�rite de la classe de base Module.
 */
class DisplayModule final : public Module
{

#pragma region Event

	/**
	 * @brief Initialise le module de d�bogage.
	 *
	 * Cette m�thode est appel�e pour initialiser le module de d�bogage.
	 */
	void Init() override;

	/**
	 * @brief D�marre le module de d�bogage.
	 *
	 * Cette m�thode est appel�e pour d�marrer le module de d�bogage.
	 */
	void Start() override;

	/**
	 * @brief Met � jour le module de d�bogage � intervalles de temps fixes.
	 *
	 * Cette m�thode est appel�e � intervalles de temps fixes pour mettre � jour le module de d�bogage.
	 */
	void FixedUpdate() override;

	/**
	 * @brief Met � jour le module de d�bogage.
	 *
	 * Cette m�thode est appel�e pour mettre � jour le module de d�bogage.
	 */
	void Update() override;

	/**
	 * @brief Effectue des op�rations de pr�-rendu.
	 *
	 * Cette m�thode est appel�e pour effectuer des op�rations de pr�-rendu dans le module de d�bogage.
	 */
	void PreRender() override;

	/**
	 * @brief Rendu du module de d�bogage.
	 *
	 * Cette m�thode est appel�e pour effectuer le rendu du module de d�bogage.
	 */
	void Render() override;

	/**
	 * @brief Rendu des �l�ments de l'interface graphique du module de d�bogage.
	 *
	 * Cette m�thode est appel�e pour effectuer le rendu des �l�ments de l'interface graphique du module de d�bogage.
	 */
	void RenderGui() override;

	/**
	 * @brief Effectue des op�rations de post-rendu.
	 *
	 * Cette m�thode est appel�e pour effectuer des op�rations de post-rendu dans le module de d�bogage.
	 */
	void PostRender() override;

	/**
	 * @brief Lib�re les ressources utilis�es par le module de d�bogage.
	 *
	 * Cette m�thode est appel�e pour lib�rer les ressources utilis�es par le module de d�bogage.
	 */
	void Release() override;

	/**
	 * @brief Finalise le module de d�bogage.
	 *
	 * Cette m�thode est appel�e pour finaliser le module de d�bogage avant sa destruction.
	 */
	void Finalize() override;

#pragma endregion

};

