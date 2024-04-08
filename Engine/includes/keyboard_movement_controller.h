#pragma once

#include "lve_game_object.h"
#include "lve_window.h"

namespace lve
{
	class KeyboardMovementController
	{
		public:
			struct KeyMappings
			{
				int moveLeft     = GLFW_KEY_A;
				int moveRight    = GLFW_KEY_D;
				int moveForward  = GLFW_KEY_W;
				int moveBackward = GLFW_KEY_S;
				int moveUp       = GLFW_KEY_E;
				int moveDown     = GLFW_KEY_Q;
				int lookLeft     = GLFW_KEY_LEFT;
				int lookRight    = GLFW_KEY_RIGHT;
				int lookUp       = GLFW_KEY_UP;
				int lookDown     = GLFW_KEY_DOWN;
			};

			/**
			 * @brief D�place l'objet dans le plan XZ en r�ponse aux entr�es clavier.
			 *
			 * Cette fonction ajuste la rotation et la translation d'un objet en fonction des entr�es clavier, pour lui permettre de se d�placer dans le plan XZ.
			 *
			 * @param _window La fen�tre GLFW o� les entr�es clavier sont d�tect�es.
			 * @param _deltaTime Le temps �coul� depuis la derni�re mise � jour.
			 * @param _gameObject L'objet � d�placer.
			 *
			 * @note Les touches du clavier utilis�es pour contr�ler le mouvement et la rotation sont configur�es dans l'attribut "keys" de cet objet.
			 * @note L'objet est d�plac� vers l'avant et vers l'arri�re le long de l'axe Z, et vers la gauche et vers la droite le long de l'axe X. Il peut �galement monter et descendre le long de l'axe Y.
			 */
			void MoveInPlaneXZ(GLFWwindow* _window, float _deltaTime, LveGameObject& _gameObject) const;

			KeyMappings keys{};
			float       moveSpeed{3.f};
			float       lookSpeed{1.5f};
	};
} // namespace lve
