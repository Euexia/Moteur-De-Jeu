#pragma once

#include <vulkan/vulkan.hpp>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>

namespace lve
{
	class LveWindow
	{
		public:
			/**
			 * @brief Enum�ration des types de curseurs GLFW.
			 *
			 * Cette �num�ration repr�sente les diff�rents types de curseurs GLFW disponibles.
			 * Ces types peuvent �tre utilis�s pour sp�cifier le style du curseur � afficher.
			 */
			enum GlfwCursorType
			{
				ARROW,
				/**< Curseur fl�che. */
				IBEAM,
				/**< Curseur I-beam (curseur de texte). */
				CROSSHAIR,
				/**< Curseur croix. */
				POINTING_HAND,
				/**< Curseur main pointant. */
				RESIZE_EW,
				/**< Curseur redimensionner horizontal. */
				RESIZE_NS,
				/**< Curseur redimensionner vertical. */
				RESIZE_NWSE,
				/**< Curseur redimensionner diagonale NW-SE. */
				RESIZE_NESW,
				/**< Curseur redimensionner diagonale NE-SW. */
				RESIZE_ALL,
				/**< Curseur redimensionner dans toutes les directions. */
				NOT_ALLOWED /**< Curseur non autoris�. */
			};

			/**
			 * @brief Enum�ration des modes de curseurs GLFW.
			 *
			 * Cette �num�ration repr�sente les diff�rents modes de curseurs GLFW.
			 * Ces modes d�finissent le comportement du curseur sur l'�cran.
			 */
			enum GlfwCursorMode
			{
				DISABLED,
				/**< Curseur d�sactiv�. */
				HIDDEN,
				/**< Curseur cach�. */
				NORMAL,
				/**< Curseur normal (visible). */
				CAPTURED /**< Curseur captur� (exclusif � la fen�tre). */
			};

			/**
			 * @brief Constructeur de la classe LveWindow.
			 *
			 * Initialise une instance de LveWindow avec les dimensions sp�cifi�es et le nom de la fen�tre.
			 *
			 * @param _width La largeur de la fen�tre.
			 * @param _height La hauteur de la fen�tre.
			 * @param _name Le nom de la fen�tre.
			 * @return Rien.
			 */
			LveWindow(int _width, int _height, const std::string& _name);

			/**
			 * @brief Destructeur de la classe LveWindow.
			 *
			 * D�truit la fen�tre GLFW.
			 */
			~LveWindow();

			/**
			 * @brief Constructeur de copie supprim�.
			 *
			 * La copie d'objets de type LveWindow est explicitement interdite pour �viter les probl�mes de gestion
			 * des ressources associ�es � la fen�tre.
			 */
			LveWindow(const LveWindow&) = delete;

			/**
			 * @brief Op�rateur d'affectation par copie supprim�.
			 *
			 * L'affectation par copie d'objets de type LveWindow est explicitement interdite pour �viter les probl�mes
			 * de gestion des ressources associ�es � la fen�tre.
			 *
			 * @return Une r�f�rence vers l'objet LveWindow apr�s affectation.
			 */
			LveWindow& operator=(const LveWindow&) = delete;

			/**
			 * @brief V�rifie si la fen�tre doit �tre ferm�e.
			 *
			 * Cette fonction renvoie true si la fen�tre doit �tre ferm�e, false sinon.
			 *
			 * @return true si la fen�tre doit �tre ferm�e, false sinon.
			 */
			bool ShouldClose() const { return glfwWindowShouldClose(window); }

			/**
			 * @brief Obtient les dimensions de la fen�tre.
			 *
			 * Cette fonction retourne les dimensions de la fen�tre sous forme d'une structure vk::Extent2D.
			 *
			 * @return Les dimensions de la fen�tre sous forme d'une structure vk::Extent2D.
			 */
			vk::Extent2D GetExtent() const { return {static_cast<uint32_t>(width), static_cast<uint32_t>(height)}; }

			/**
			 * @brief V�rifie si la fen�tre a �t� redimensionn�e.
			 *
			 * Cette fonction v�rifie si la fen�tre a �t� redimensionn�e depuis la derni�re v�rification.
			 *
			 * @return true si la fen�tre a �t� redimensionn�e, sinon false.
			 */

			bool WasWindowResized() const { return frameBufferResize; }

			/**
			 * @brief R�initialise le drapeau de redimensionnement de la fen�tre.
			 *
			 * Cette fonction r�initialise le drapeau indiquant que la fen�tre a �t� redimensionn�e.
			 */
			void ResetWindowResizedFlag() { frameBufferResize = false; }

			/**
			 * @brief Cr�e une surface Vulkan associ�e � une fen�tre.
			 *
			 * Cette fonction cr�e une surface Vulkan associ�e � une fen�tre, permettant � Vulkan de dessiner dans cette fen�tre.
			 *
			 * @param _instance L'instance Vulkan utilis�e pour cr�er la surface.
			 * @param _surface Un pointeur vers l'objet de surface Vulkan � cr�er. Ce pointeur sera mis � jour pour contenir la surface cr��e.
			 * @throws Une exception en cas d'�chec lors de la cr�ation de la surface.
			 */
			void CreateWindowSurface(vk::Instance _instance, vk::SurfaceKHR* _surface) const;

			/**
			 * @brief Obtient un pointeur vers la fen�tre GLFW.
			 *
			 * Cette fonction retourne un pointeur vers la fen�tre GLFW associ�e � l'instance actuelle.
			 *
			 * @return Un pointeur vers la fen�tre GLFW.
			 */
			[[nodiscard]] GLFWwindow* GetGlfwWindow() const { return window; }

			/**
			 * @brief R�initialise le curseur de la fen�tre.
			 *
			 * Cette fonction r�initialise le curseur de la fen�tre � sa forme par d�faut.
			 */
			void ResetCursorWindow() const { glfwSetCursor(window, nullptr); }

			/**
			 * @brief Change la forme du curseur de la fen�tre en un cube color�.
			 *
			 * Cette fonction change la forme du curseur de la fen�tre en un cube color�, avec la possibilit� de sp�cifier la couleur du cube.
			 *
			 * @param _color
			 * @param color La couleur du cube. La valeur par d�faut est 255.
			 */
			void CubeCursorWindow(int _color = 255) const;

			/**
			 * @brief D�finit le curseur standard de la fen�tre.
			 *
			 * Cette fonction d�finit le curseur standard de la fen�tre � un type sp�cifi�.
			 *
			 * @param _cursorType
			 * @param CursorType Le type de curseur � d�finir.
			 */
			void StandardCursorWindow(GlfwCursorType _cursorType) const;

			/**
			 * @brief D�finit le mode de curseur d'entr�e pour la fen�tre.
			 *
			 * Cette fonction d�finit le mode de curseur d'entr�e pour la fen�tre � un mode sp�cifi�.
			 *
			 * @param _mode
			 * @param mode Le mode de curseur d'entr�e � d�finir.
			 */
			void SetInputCursorMode(GlfwCursorMode _mode) const;

		private:
			/**
			 * @brief Callback function for framebuffer resize events.
			 *
			 * This function is called when the framebuffer of the window is resized.
			 *
			 * @param _window The window that triggered the resize event.
			 * @param _width The new width of the framebuffer.
			 * @param _height The new height of the framebuffer.
			 */
			static void FrameBufferResizeCallBack(GLFWwindow* _window, int _width, int _height);

			/**
			 * @brief Initialise la fen�tre GLFW.
			 *
			 * Cette fonction initialise GLFW, configure la fen�tre pour �tre non redimensionnable et cr�e une fen�tre GLFW avec la taille et le nom sp�cifi�s.
			 */
			void InitWindow();

			int  width;                     // Largeur de la fen�tre
			int  height;                    // Hauteur de la fen�tre
			bool frameBufferResize = false; // Bool�en indiquant si le framebuffer a �t� redimensionn�.

			std::string windowName; // Nom de la fen�tre
			GLFWwindow* window;     // Fen�tre GLFW
	};
} // namespace lve
