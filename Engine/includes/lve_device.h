#pragma once

#include "lve_window.h"

#include <vulkan/vulkan.hpp> // Inclure les en-t�tes Vulkan C++

// std lib headers
#include <string>
#include <vector>

namespace lve
{
	// Structure pour stocker les d�tails de support de la cha�ne d'�change
	struct SwapChainSupportDetails
	{
		vk::SurfaceCapabilitiesKHR        capabilities; // Capacit�s de la surface Vulkan
		std::vector<vk::SurfaceFormatKHR> formats;      // Formats de surface Vulkan pris en charge
		std::vector<vk::PresentModeKHR>   presentModes; // Modes de pr�sentation Vulkan pris en charge
	};

	// Structure pour stocker les indices des files d'attente requises
	struct QueueFamilyIndices
	{
		uint32_t graphicsFamily; // Indice de la famille de files d'attente graphiques
		uint32_t presentFamily;  // Indice de la famille de files d'attente de pr�sentation
		bool     graphicsFamilyHasValue = false;
		// Indique si l'indice de la famille de files d'attente graphiques est d�fini
		bool presentFamilyHasValue = false;
		// Indique si l'indice de la famille de files d'attente de pr�sentation est d�fini

		// V�rifie si les indices des files d'attente requis sont complets
		bool IsComplete() const { return graphicsFamilyHasValue && presentFamilyHasValue; }
	};

	class LveDevice
	{
		public:
			#ifdef NDEBUG
        const bool enableValidationLayers = false;
			#else
			const bool enableValidationLayers = true;
			#endif

			/**
			* @brief Constructeur de la classe LveDevice.
			*
			* Initialise un p�riph�rique Vulkan en prenant une r�f�rence � une fen�tre Vulkan LveWindow en tant que param�tre.
			*
			* @param _window Une r�f�rence � la fen�tre Vulkan utilis�e pour initialiser le p�riph�rique.
			*/
			explicit LveDevice(LveWindow& _window);

			/**
			 * @brief Destructeur de la classe LveDevice.
			 *
			 * Nettoie les ressources associ�es au p�riph�rique Vulkan lorsqu'il est d�truit.
			 */
			~LveDevice();

			// Not copyable or movable
			/**
			* @brief Constructeur de copie supprim�.
			*
			* Emp�che la cr�ation d'une nouvelle instance de LveDevice en copiant une instance existante.
			*/
			LveDevice(const LveDevice&) = delete;

			/**
			* @brief Op�rateur d'affectation de copie supprim�.
			*
			* Emp�che la copie des membres d'une instance de LveDevice vers une autre instance existante.
			*/
			LveDevice& operator=(const LveDevice&) = delete;

			/**
			* @brief Constructeur de d�placement supprim�.
			*
			* Emp�che la cr�ation d'une nouvelle instance de LveDevice en d�pla�ant une instance existante.
			*/
			LveDevice(LveDevice&&) = delete;

			/**
			* @brief Op�rateur d'affectation de d�placement supprim�.
			*
			* Emp�che le d�placement des membres d'une instance de LveDevice vers une autre instance existante.
			*/
			LveDevice& operator=(LveDevice&&) = delete;

			/**
			* @brief R�cup�re le pool de commandes Vulkan associ� au p�riph�rique.
			*
			* @return Le pool de commandes Vulkan associ� au p�riph�rique.
			*/
			vk::CommandPool GetCommandPool() const { return commandPool; }

			/**
			 * @brief R�cup�re l'objet de p�riph�rique Vulkan.
			 *
			 * @return L'objet de p�riph�rique Vulkan.
			 */
			vk::Device Device() const { return device_; }

			// MAURAD
			vk::Instance       GetInstance() const { return instance; }
			vk::PhysicalDevice GetPhysicalDevice() const { return physicalDevice; }

			/**
			 * @brief R�cup�re la surface Vulkan associ�e au p�riph�rique.
			 *
			 * @return La surface Vulkan associ�e au p�riph�rique.
			 */
			vk::SurfaceKHR Surface() const { return surface_; }

			/**
			 * @brief R�cup�re la file de commandes graphiques du p�riph�rique.
			 *
			 * @return La file de commandes graphiques du p�riph�rique.
			 */
			vk::Queue GraphicsQueue() const { return graphicsQueue_; }

			/**
			 * @brief R�cup�re la file de commandes de pr�sentation du p�riph�rique.
			 *
			 * @return La file de commandes de pr�sentation du p�riph�rique.
			 */
			vk::Queue PresentQueue() const { return presentQueue_; }


			/**
			 * @brief R�cup�re les d�tails de prise en charge de la cha�ne de balisage pour le p�riph�rique.
			 *
			 * @return Les d�tails de prise en charge de la cha�ne de balisage pour le p�riph�rique.
			 */
			SwapChainSupportDetails GetSwapChainSupport() { return QuerySwapChainSupport(physicalDevice); }

			/**
			 * @brief Trouve un type de m�moire appropri� pour une utilisation sp�cifi�e.
			 *
			 * @param _typeFilter Le type de filtre de m�moire.
			 * @param _properties Les propri�t�s de la m�moire.
			 * @return Le type de m�moire appropri�.
			 */
			uint32_t FindMemoryType(uint32_t _typeFilter, vk::MemoryPropertyFlags _properties) const;

			/**
			 * @brief Trouve les familles de files de commandes physiques disponibles sur le p�riph�rique.
			 *
			 * @return Les familles de files de commandes physiques disponibles sur le p�riph�rique.
			 */
			QueueFamilyIndices FindPhysicalQueueFamilies() { return FindQueueFamilies(physicalDevice); }

			/**
			 * @brief Trouve un format d'image support� par le p�riph�rique.
			 *
			 * @param _candidates Les formats d'image candidats.
			 * @param _tiling L'inclinaison de l'image.
			 * @param _features Les fonctionnalit�s du format de l'image.
			 * @return Le format d'image support� par le p�riph�rique.
			 */
			vk::Format FindSupportedFormat(
				const std::vector<vk::Format>& _candidates, vk::ImageTiling _tiling,
				vk::FormatFeatureFlags         _features) const;


			// Buffer Helper Functions
			/**
			 * @brief Cr�e un tampon Vulkan avec les param�tres sp�cifi�s.
			 *
			 * @param _size La taille du tampon.
			 * @param _usage Les indicateurs d'utilisation du tampon.
			 * @param _properties Les propri�t�s de la m�moire du tampon.
			 * @param _buffer R�f�rence � l'objet tampon Vulkan cr��.
			 * @param _bufferMemory R�f�rence � l'objet m�moire du tampon Vulkan cr��.
			 */
			void CreateBuffer(
				vk::DeviceSize          _size,
				vk::BufferUsageFlags    _usage,
				vk::MemoryPropertyFlags _properties,
				vk::Buffer&             _buffer,
				vk::DeviceMemory&       _bufferMemory) const;

			/**
			 * @brief D�marre une s�quence de commandes Vulkan temporaires.
			 *
			 * Cette fonction alloue un tampon de commandes Vulkan et le d�marre pour une utilisation unique.
			 *
			 * @return Le tampon de commandes Vulkan allou�.
			 */
			vk::CommandBuffer BeginSingleTimeCommands() const;

			/**
			 * @brief Termine une s�quence de commandes Vulkan temporaires.
			 *
			 * Cette fonction termine l'ex�cution d'une s�quence de commandes Vulkan temporaires et lib�re les ressources associ�es.
			 *
			 * @param _commandBuffer Le tampon de commandes Vulkan � terminer.
			 */
			void EndSingleTimeCommands(vk::CommandBuffer _commandBuffer) const;

			/**
			 * @brief Copie les donn�es d'un tampon source vers un tampon de destination.
			 *
			 * @param _srcBuffer Le tampon source � partir duquel copier les donn�es.
			 * @param _dstBuffer Le tampon de destination o� copier les donn�es.
			 * @param _size La taille des donn�es � copier.
			 */
			void CopyBuffer(vk::Buffer _srcBuffer, vk::Buffer _dstBuffer, vk::DeviceSize _size) const;

			/**
			 * @brief Copie les donn�es d'un tampon vers une image Vulkan.
			 *
			 * Cette fonction copie les donn�es d'un tampon Vulkan vers une image Vulkan en sp�cifiant la largeur, la hauteur et le nombre de couches de l'image.
			 *
			 * @param _buffer Le tampon contenant les donn�es � copier.
			 * @param _image L'image Vulkan de destination.
			 * @param _width La largeur de l'image.
			 * @param _height La hauteur de l'image.
			 * @param _layerCount Le nombre de couches de l'image.
			 */
			void CopyBufferToImage(
				vk::Buffer _buffer, vk::Image _image, uint32_t _width, uint32_t _height, uint32_t _layerCount) const;

			/**
			 * @brief Cr�e une image Vulkan avec les informations fournies.
			 *
			 * Cette fonction cr�e une image Vulkan en utilisant les informations sp�cifi�es dans la structure VkImageCreateInfo.
			 *
			 * @param _imageInfo Les informations de cr�ation de l'image.
			 * @param _properties Les propri�t�s de la m�moire de l'image.
			 * @param _image R�f�rence � l'objet image Vulkan cr��.
			 * @param _imageMemory R�f�rence � l'objet m�moire de l'image Vulkan cr��.
			 */
			void CreateImageWithInfo(
				const vk::ImageCreateInfo& _imageInfo,
				vk::MemoryPropertyFlags    _properties,
				vk::Image&                 _image,
				vk::DeviceMemory&          _imageMemory) const;

			/**
			 * @brief Propri�t�s du p�riph�rique physique Vulkan associ�.
			 *
			 * Cette structure contient les propri�t�s du p�riph�rique physique Vulkan associ�.
			 */
			vk::PhysicalDeviceProperties properties;

			/**
		  * @brief Cr�e une instance Vulkan.
		  *
		  * Cette fonction initialise une instance Vulkan, qui repr�sente la connexion entre l'application et l'API Vulkan.
		  */
			vk::Instance CreateInstance();

			/**
			* @brief Configure le gestionnaire de d�bogage.
			*
			* Cette fonction configure un gestionnaire de d�bogage Vulkan pour recevoir les messages de validation et de d�bogage de l'API Vulkan.
			*/
			vk::DebugUtilsMessengerEXT SetupDebugMessenger();

		private:
			/**
			 * @brief Cr�e une surface Vulkan.
			 *
			 * Cette fonction cr�e une surface Vulkan � partir de la fen�tre Vulkan associ�e.
			 */
			void CreateSurface();

			/**
			 * @brief S�lectionne le p�riph�rique physique Vulkan appropri�.
			 *
			 * Cette fonction s�lectionne le p�riph�rique physique Vulkan appropri� parmi ceux disponibles sur le syst�me.
			 */
			void PickPhysicalDevice();

			/**
			 * @brief Cr�e le p�riph�rique logique Vulkan.
			 *
			 * Cette fonction cr�e le p�riph�rique logique Vulkan qui sera utilis� pour interagir avec le mat�riel graphique.
			 */
			void CreateLogicalDevice();

			/**
			 * @brief Cr�e le pool de commandes Vulkan.
			 *
			 * Cette fonction cr�e un pool de commandes Vulkan, qui est utilis� pour allouer des tampons de commandes pour le p�riph�rique logique.
			 */
			void CreateCommandPool();


			// helper functions
			/**
			 * @brief V�rifie si le p�riph�rique physique Vulkan sp�cifi� convient aux besoins de l'application.
			 *
			 * Cette fonction examine les propri�t�s et les capacit�s du p�riph�rique pour d�terminer s'il convient � l'application.
			 *
			 * @param _device Le p�riph�rique physique Vulkan � �valuer.
			 * @return true si le p�riph�rique convient, sinon false.
			 */
			bool IsDeviceSuitable(vk::PhysicalDevice _device);

			/**
			 * @brief R�cup�re les extensions Vulkan requises par l'application.
			 *
			 * Cette fonction retourne un vecteur contenant les noms des extensions Vulkan n�cessaires � l'application.
			 *
			 * @return Un vecteur de pointeurs de cha�nes C repr�sentant les noms des extensions requises.
			 */
			std::vector<const char*> GetRequiredExtensions() const;

			/**
			 * @brief V�rifie la prise en charge des couches de validation Vulkan.
			 *
			 * Cette fonction v�rifie si les couches de validation Vulkan sp�cifi�es sont prises en charge par l'instance Vulkan.
			 *
			 * @return true si les couches de validation sont prises en charge, sinon false.
			 */
			bool CheckValidationLayerSupport() const;

			/**
			 * @brief Recherche les familles de files de commandes support�es par le p�riph�rique physique Vulkan sp�cifi�.
			 *
			 * Cette fonction identifie les indices des familles de files de commandes graphiques et de pr�sentation support�es par le p�riph�rique.
			 *
			 * @param _device Le p�riph�rique physique Vulkan � �valuer.
			 * @return Une structure contenant les indices des familles de files de commandes support�es.
			 */
			QueueFamilyIndices FindQueueFamilies(vk::PhysicalDevice _device) const;

			/**
			 * @brief Remplit une structure de donn�es pour la cr�ation d'un gestionnaire de d�bogage Vulkan.
			 *
			 * Cette fonction initialise une structure de donn�es pour la cr�ation d'un gestionnaire de d�bogage Vulkan avec les informations n�cessaires.
			 *
			 * @param _createInfo La structure de donn�es � remplir pour la cr�ation du gestionnaire de d�bogage.
			 */
			void PopulateDebugMessengerCreateInfo(vk::DebugUtilsMessengerCreateInfoEXT& _createInfo);

			/**
			 * @brief V�rifie si l'instance Vulkan poss�de les extensions requises par GLFW.
			 *
			 * Cette fonction v�rifie si l'instance Vulkan poss�de les extensions requises par GLFW pour cr�er une surface Vulkan.
			 */
			void HasGlfwRequiredInstanceExtensions() const;

			/**
			 * @brief V�rifie si le p�riph�rique physique Vulkan sp�cifi� prend en charge les extensions de p�riph�rique n�cessaires.
			 *
			 * Cette fonction v�rifie si le p�riph�rique physique Vulkan sp�cifi� prend en charge les extensions de p�riph�rique requises par l'application.
			 *
			 * @param _device Le p�riph�rique physique Vulkan � �valuer.
			 * @return true si les extensions de p�riph�rique sont prises en charge, sinon false.
			 */
			bool CheckDeviceExtensionSupport(vk::PhysicalDevice _device) const;

			/**
			 * @brief Interroge le p�riph�rique physique Vulkan sp�cifi� pour obtenir les d�tails de support de la cha�ne d'�change.
			 *
			 * Cette fonction interroge le p�riph�rique physique Vulkan sp�cifi� pour obtenir les d�tails de support de la cha�ne d'�change, tels que les capacit�s, les formats de surface support�s et les modes de pr�sentation support�s.
			 *
			 * @param _device Le p�riph�rique physique Vulkan � interroger.
			 * @return Une structure contenant les d�tails de support de la cha�ne d'�change.
			 */
			SwapChainSupportDetails QuerySwapChainSupport(vk::PhysicalDevice _device) const;


			vk::Instance instance; /**< L'instance Vulkan utilis�e par l'application. */

			vk::DebugUtilsMessengerEXT debugMessenger;
			/**< Le gestionnaire de d�bogage Vulkan pour la gestion des messages de validation et de d�bogage. */

			vk::PhysicalDevice physicalDevice = VK_NULL_HANDLE;
			/**< Le p�riph�rique physique Vulkan utilis� par l'application, initialis� � VK_NULL_HANDLE par d�faut. */

			LveWindow& window; /**< Une r�f�rence � la fen�tre Vulkan utilis�e par l'application. */

			vk::CommandPool commandPool;
			/**< Le pool de commandes Vulkan utilis� pour allouer les tampons de commandes. */

			vk::Device device_; /**< Le p�riph�rique logique Vulkan utilis� par l'application. */

			vk::SurfaceKHR surface_; /**< La surface Vulkan associ�e � la fen�tre Vulkan utilis�e par l'application. */

			vk::Queue graphicsQueue_;
			/**< La file de commandes pour les op�rations graphiques sur le p�riph�rique logique. */

			vk::Queue presentQueue_;
			/**< La file de commandes pour les op�rations de pr�sentation sur le p�riph�rique logique. */

			const std::vector<const char*> validationLayers = {"VK_LAYER_KHRONOS_validation"};
			/**< Les couches de validation Vulkan activ�es par d�faut. */

			const std::vector<const char*> deviceExtensions = {vk::KHRSwapchainExtensionName};
			/**< Les extensions de p�riph�rique Vulkan utilis�es par l'application, avec l'extension de la cha�ne d'�change Vulkan activ�e par d�faut. */
	};
} // namespace lve
