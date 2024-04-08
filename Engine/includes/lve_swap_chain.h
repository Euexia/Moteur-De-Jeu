#pragma once

#include "lve_device.h"

// vulkan headers
#include <vulkan/vulkan.hpp>

// std lib headers
#include <memory>
#include <string>
#include <vector>

namespace lve
{
	class LveSwapChain
	{
		public:
			static constexpr int MAX_FRAMES_IN_FLIGHT = 2;

			/**
			 * @brief Constructeur prenant une r�f�rence � un objet LveDevice et une �tendue windowExtent.
			 *
			 * @param _deviceRef R�f�rence � un objet LveDevice.
			 * @param _windowExtent �tendue de la fen�tre.
			 */
			LveSwapChain(LveDevice& _deviceRef, vk::Extent2D _windowExtent);

			/**
			 * @brief Constructeur prenant une r�f�rence � un objet LveDevice, une �tendue extent
			 *        et un pointeur partag� vers une pr�c�dente cha�ne d'�changes.
			 *
			 * @param _deviceRef R�f�rence � un objet LveDevice.
			 * @param _extent �tendue de la cha�ne d'�changes.
			 * @param _previous Pointeur partag� vers une pr�c�dente cha�ne d'�changes.
			 */
			LveSwapChain(LveDevice& _deviceRef, vk::Extent2D _extent, const std::shared_ptr<LveSwapChain>& _previous);

			/**
			 * @brief Destructeur.
			 */
			~LveSwapChain();

			/**
			 * @brief Initialise la cha�ne d'�changes.
			 */
			void Init();

			/**
			 * @brief Constructeur de recopie supprim�.
			 */
			LveSwapChain(const LveSwapChain&) = delete;

			/**
			 * @brief Op�rateur d'assignation par recopie supprim�.
			 */
			LveSwapChain& operator=(const LveSwapChain&) = delete;

			/**
			 * @brief Obtient le framebuffer correspondant � l'indice sp�cifi�.
			 *
			 * Cette fonction retourne le framebuffer de la swap chain qui correspond � l'indice sp�cifi�.
			 *
			 * @param _index L'indice du framebuffer � r�cup�rer.
			 * @return vk::Framebuffer Le framebuffer correspondant � l'indice sp�cifi�.
			 */
			vk::Framebuffer GetFrameBuffer(const int _index) const { return swapChainFramebuffers[_index]; }

			/**
			 * @brief Obtient le passe de rendu.
			 *
			 * Cette fonction retourne le passe de rendu associ� � la swap chain.
			 *
			 * @return vk::RenderPass Le passe de rendu associ� � la swap chain.
			 */
			vk::RenderPass GetRenderPass() const { return renderPass; }

			/**
			 * @brief Obtient la vue d'image correspondant � l'indice sp�cifi�.
			 *
			 * Cette fonction retourne la vue d'image de la swap chain qui correspond � l'indice sp�cifi�.
			 *
			 * @param _index L'indice de la vue d'image � r�cup�rer.
			 * @return vk::ImageView La vue d'image correspondant � l'indice sp�cifi�.
			 */
			vk::ImageView GetImageView(const int _index) const { return swapChainImageViews[_index]; }

			/**
			 * @brief Obtient le nombre d'images dans la swap chain.
			 *
			 * Cette fonction retourne le nombre d'images actuellement pr�sentes dans la swap chain.
			 *
			 * @return size_t Le nombre d'images dans la swap chain.
			 */
			size_t ImageCount() const { return swapChainImages.size(); }

			/**
			 * @brief Obtient le format d'image de la swap chain.
			 *
			 * Cette fonction retourne le format d'image utilis� par la swap chain.
			 *
			 * @return vk::Format Le format d'image de la swap chain.
			 */
			vk::Format GetSwapChainImageFormat() const { return swapChainImageFormat; }

			/**
			 * @brief Obtient l'�tendue de la swap chain.
			 *
			 * Cette fonction retourne l'�tendue (largeur et hauteur) de la swap chain.
			 *
			 * @return vk::Extent2D L'�tendue de la swap chain.
			 */
			vk::Extent2D GetSwapChainExtent() const { return swapChainExtent; }

			/**
			 * @brief Obtient la largeur de la swap chain.
			 *
			 * Cette fonction retourne la largeur de la swap chain.
			 *
			 * @return uint32_t La largeur de la swap chain.
			 */
			uint32_t Width() const { return swapChainExtent.width; }

			/**
			 * @brief Obtient la hauteur de la swap chain.
			 *
			 * Cette fonction retourne la hauteur de la swap chain.
			 *
			 * @return uint32_t La hauteur de la swap chain.
			 */
			uint32_t Height() const { return swapChainExtent.height; }

			/**
			 * @brief Calcule le rapport d'aspect de l'�tendue de la swap chain.
			 *
			 * Cette fonction calcule le rapport d'aspect (largeur/hauteur) de l'�tendue de la swap chain.
			 *
			 * @return float Le rapport d'aspect de l'�tendue de la swap chain.
			 */
			float ExtentAspectRatio() const
			{
				return static_cast<float>(swapChainExtent.width) / static_cast<float>(swapChainExtent.height);
			}

			/**
			 * @brief Recherche le format de profondeur adapt�.
			 *
			 * Cette fonction recherche le format de profondeur adapt� parmi une liste de formats donn�e, en tenant compte des caract�ristiques sp�cifi�es.
			 *
			 * @return vk::Format Le format de profondeur adapt�.
			 */
			vk::Format FindDepthFormat() const;

			/**
			 * @brief Acquiert l'index de l'image suivante dans la cha�ne de swaps.
			 *
			 * Cette fonction acquiert l'index de l'image suivante dans la cha�ne de swaps.
			 *
			 * @param _imageIndex Pointeur vers la variable o� stocker l'index de l'image acquise.
			 * @return VkResult Le r�sultat de l'op�ration.
			 */
			vk::Result AcquireNextImage(uint32_t* _imageIndex) const;

			/**
			 * @brief Soumet les command buffers pour ex�cution et pr�sente le r�sultat.
			 *
			 * Cette fonction soumet les command buffers sp�cifi�s pour ex�cution, attend leur ach�vement, puis pr�sente le r�sultat � l'�cran.
			 *
			 * @param _buffers Tableau des command buffers � soumettre.
			 * @param _imageIndex Pointeur vers l'index de l'image � pr�senter.
			 * @return VkResult Le r�sultat de l'op�ration.
			 */
			vk::Result SubmitCommandBuffers(const vk::CommandBuffer* _buffers, uint32_t* _imageIndex);

			/**
			 * @brief Compare les formats de profondeur et d'image avec une autre LveSwapChain.
			 *
			 * Cette fonction compare les formats de profondeur et d'image de cette LveSwapChain avec ceux d'une autre LveSwapChain sp�cifi�e.
			 *
			 * @param _swapChain La LveSwapChain avec laquelle comparer les formats.
			 * @return bool True si les formats de profondeur et d'image sont identiques, sinon False.
			 */
			bool CompareSwapFormats(const LveSwapChain& _swapChain) const
			{
				return _swapChain.swapChainDepthFormat == swapChainDepthFormat &&
				       _swapChain.swapChainImageFormat == swapChainImageFormat;
			}

		private:
			/**
			 * @brief Cr�e la cha�ne de swaps.
			 *
			 * Cette fonction cr�e la cha�ne de swaps, qui est utilis�e pour la pr�sentation des images � l'�cran.
			 * La cr�ation de la cha�ne de swaps implique la s�lection des formats de surface, des modes de pr�sentation et des dimensions appropri�s, ainsi que la configuration de divers autres param�tres li�s � la pr�sentation.
			 *
			 * @throws std::runtime_error si la cr�ation de la cha�ne de swaps �choue.
			 */
			void CreateSwapChain();

			/**
			 * @brief Cr�e les vues d'image de la cha�ne de swaps.
			 *
			 * Cette fonction cr�e les vues d'image pour chaque image de la cha�ne de swaps.
			 * Chaque vue d'image est associ�e � une image de la cha�ne de swaps et sp�cifie comment cette image doit �tre interpr�t�e lorsqu'elle est utilis�e comme texture.
			 *
			 * @throws std::runtime_error si la cr�ation d'une vue d'image �choue.
			 */
			void CreateImageViews();

			/**
			* @brief Cr�e les ressources de profondeur pour la cha�ne de swaps.
			*
			* Cette fonction cr�e les ressources de profondeur n�cessaires pour chaque image de la cha�ne de swaps.
			* Ces ressources de profondeur sont utilis�es pour le rendu des sc�nes en 3D afin de d�terminer la profondeur des fragments et g�rer la d�tection des collisions.
			*
			* @throws std::runtime_error si la cr�ation d'une ressource de profondeur �choue.
			*/
			void CreateDepthResources();

			/**
			 * @brief Cr�e la passe de rendu.
			 *
			 * Cette fonction cr�e la passe de rendu utilis�e pour d�finir comment les donn�es de g�om�trie sont trait�es lors du rendu d'une image.
			 * La passe de rendu d�finit les attachements de couleur et de profondeur, ainsi que les sous-passes qui d�crivent les op�rations de rendu � ex�cuter.
			 *
			 * @throws std::runtime_error si la cr�ation de la passe de rendu �choue.
			 */
			void CreateRenderPass();

			/**
			 * @brief Cr�e les tampons de trame.
			 *
			 * Cette fonction cr�e les tampons de trame qui sont utilis�s pour stocker les informations de rendu associ�es � chaque image de la cha�ne de swaps.
			 * Chaque tampon de trame est associ� � une image de la cha�ne de swaps et est configur� avec les vues d'image et les profondeurs appropri�es pour le rendu.
			 *
			 * @throws std::runtime_error si la cr�ation d'un tampon de trame �choue.
			 */
			void CreateFrameBuffers();

			/**
			 * @brief Cr�e les objets de synchronisation.
			 *
			 * Cette fonction cr�e les objets de synchronisation utilis�s pour synchroniser l'ex�cution des op�rations de rendu avec la pr�sentation des images.
			 * Ces objets comprennent des s�maphores pour la synchronisation entre les diff�rentes �tapes du pipeline graphique, ainsi que des cl�tures de barri�re pour la synchronisation entre le CPU et le GPU.
			 *
			 * @throws std::runtime_error si la cr�ation d'un objet de synchronisation �choue.
			 */
			void CreateSyncObjects();

			// Helper functions

			/**
			 * @brief Choisissez le format de surface de la cha�ne de swaps.
			 *
			 * Cette fonction choisit le format de surface de la cha�ne de swaps en parcourant les formats disponibles et en recherchant un format sp�cifique (B8G8R8A8_SRGB) avec un espace colorim�trique compatible (VK_COLOR_SPACE_SRGB_NONLINEAR_KHR).
			 * Si un tel format est trouv�, il est renvoy�. Sinon, le premier format disponible est renvoy� par d�faut.
			 *
			 * @param _availableFormats Les formats de surface disponibles.
			 * @return Le format de surface choisi.
			 */
			static vk::SurfaceFormatKHR ChooseSwapSurfaceFormat(
				const std::vector<vk::SurfaceFormatKHR>& _availableFormats);

			/**
			 * @brief Choisissez le mode de pr�sentation de la cha�ne de swaps.
			 *
			 * Cette fonction choisit le mode de pr�sentation de la cha�ne de swaps en parcourant les modes de pr�sentation disponibles et en recherchant un mode sp�cifique (VK_PRESENT_MODE_IMMEDIATE_KHR) qui offre une pr�sentation imm�diate sans attente de synchronisation verticale (V-Sync).
			 * Si un tel mode est trouv�, il est renvoy�. Sinon, le mode de pr�sentation FIFO (VK_PRESENT_MODE_FIFO_KHR) avec synchronisation verticale est renvoy� par d�faut.
			 *
			 * @param _availablePresentModes Les modes de pr�sentation disponibles.
			 * @return Le mode de pr�sentation choisi.
			 */
			static vk::PresentModeKHR ChooseSwapPresentMode(
				const std::vector<vk::PresentModeKHR>& _availablePresentModes);

			/**
			 * @brief Choisissez l'�tendue de la cha�ne de swaps.
			 *
			 * Cette fonction choisit l'�tendue de la cha�ne de swaps en se basant sur les capacit�s de la surface de rendu.
			 * Si l'�tendue actuelle est d�j� d�finie dans les capacit�s de la surface, elle est retourn�e directement.
			 * Sinon, une �tendue appropri�e est calcul�e en fonction de la taille de la fen�tre et des contraintes minimales et maximales sp�cifi�es dans les capacit�s de la surface.
			 *
			 * @param _capabilities Les capacit�s de la surface de rendu.
			 * @return L'�tendue de la cha�ne de swaps choisie.
			 */
			vk::Extent2D ChooseSwapExtent(const vk::SurfaceCapabilitiesKHR& _capabilities) const;

			vk::Format   swapChainImageFormat; /**< Format des images de la cha�ne d'�changes. */
			vk::Format   swapChainDepthFormat; /**< Format de profondeur de la cha�ne d'�changes. */
			vk::Extent2D swapChainExtent;      /**< �tendue de la cha�ne d'�changes. */

			std::vector<vk::Framebuffer> swapChainFramebuffers;
			/**< Tableau des tampons de trame de la cha�ne d'�changes. */
			vk::RenderPass renderPass; /**< Passe de rendu de la cha�ne d'�changes. */

			std::vector<vk::Image> depthImages; /**< Tableau des images de profondeur. */
			std::vector<vk::DeviceMemory> depthImageMemorys; /**< Tableau de la m�moire des images de profondeur. */
			std::vector<vk::ImageView> depthImageViews; /**< Tableau des vues des images de profondeur. */
			std::vector<vk::Image> swapChainImages; /**< Tableau des images de la cha�ne d'�changes. */
			std::vector<vk::ImageView> swapChainImageViews; /**< Tableau des vues des images de la cha�ne d'�changes. */

			LveDevice&   lveDevice;    /**< R�f�rence � l'objet LveDevice associ�. */
			vk::Extent2D windowExtent; /**< �tendue de la fen�tre. */

			vk::SwapchainKHR              swapChain;    /**< Cha�ne d'�changes Vulkan. */
			std::shared_ptr<LveSwapChain> oldSwapChain; /**< Ancienne cha�ne d'�changes. */

			std::vector<vk::Semaphore> imageAvailableSemaphores; /**< S�maphores disponibles pour les images. */
			std::vector<vk::Semaphore> renderFinishedSemaphores; /**< S�maphores indiquant la fin du rendu. */
			std::vector<vk::Fence>     inFlightFences; /**< Barri�res d'attente pour la synchronisation. */
			std::vector<vk::Fence>     imagesInFlight; /**< Barri�res pour les images en cours de traitement. */
			size_t                     currentFrame = 0; /**< Indice du cadre actuel. */
	};
} // namespace lve
