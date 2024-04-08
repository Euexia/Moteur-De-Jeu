#pragma once

#include "lve_descriptors.h"
#include "lve_device.h"
#include "lve_game_object.h"
#include "lve_renderer.h"
#include "lve_window.h"

//std
#include <memory>
#include <vector>

namespace lve
{
	/**
	 * @brief Classe repr�sentant la premi�re application utilisant Vulkan.
	 *
	 * Cette classe g�re l'ex�cution de la premi�re application Vulkan, incluant la cr�ation de la fen�tre.
	 */
	class FirstApp
	{
		public:
			static constexpr int WIDTH  = 800; ///< Largeur de la fen�tre par d�faut.
			static constexpr int HEIGHT = 600; ///< Hauteur de la fen�tre par d�faut.

			FirstApp();
			~FirstApp();

			FirstApp(const FirstApp&)           = delete;
			FirstApp operator=(const FirstApp&) = delete;

			/**
			 * @brief Lance l'ex�cution de l'application.
			 *
			 * Cette fonction d�marre l'ex�cution de l'application Vulkan, y compris la cr�ation de la fen�tre.
			 */
			void Run();

		private:
			void LoadGameObjects();

			LveWindow   lveWindow{WIDTH, HEIGHT, "Hello Vulkan!"}; ///< Fen�tre de l'application.
			LveDevice   lveDevice{lveWindow};
			LveRenderer lveRenderer{lveWindow, lveDevice};

			// note : order of declarations matters
			std::unique_ptr<LveDescriptorPool> globalPool{};
			LveGameObject::Map                 gameObjects;
	};
} // namespace lve
