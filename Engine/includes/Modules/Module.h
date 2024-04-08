#pragma once

class ModuleManager;

/**
 * @brief Classe Module.
 *
 * Cette classe repr�sente un module de l'application.
 * Les modules peuvent �tre initialis�s, d�marr�s, mis � jour et lib�r�s de mani�re contr�l�e par le ModuleManager.
 */
class Module
{
	friend class ModuleManager;

	protected:
		/**
		 * @brief Destructeur prot�g�.
		 *
		 * Le destructeur est prot�g� car les modules doivent �tre d�truits uniquement par le ModuleManager.
		 */
		~Module() = default;

	public:
		/**
		 * @brief Initialise le module.
		 */
		virtual void Init()
		{
		}

		/**
		 * @brief D�marre le module.
		 */
		virtual void Start()
		{
		}

		/**
		 * @brief Effectue une mise � jour fixe du module.
		 */
		virtual void FixedUpdate()
		{
		}

		/**
		 * @brief Met � jour le module.
		 */
		virtual void Update()
		{
		}

		/**
		 * @brief Fonction pr�-rendu du module.
		 */
		virtual void PreRender()
		{
		}

		/**
		 * @brief Rendu du module.
		 */
		virtual void Render()
		{
		}

		/**
		 * @brief Rendu de l'interface graphique du module.
		 */
		virtual void RenderGui()
		{
		}

		/**
		 * @brief Fonction post-rendu du module.
		 */
		virtual void PostRender()
		{
		}

		/**
		 * @brief Lib�re les ressources utilis�es par le module.
		 */
		virtual void Release()
		{
		}

		/**
		 * @brief Finalise le module.
		 */
		virtual void Finalize()
		{
		}

	protected:
		ModuleManager* moduleModule = nullptr; /**< Pointeur vers le gestionnaire de modules. */
};
