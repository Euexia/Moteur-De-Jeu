#pragma once

#include "lve_window.h"

#include <vulkan/vulkan.hpp> // Inclure les en-t�tes Vulkan C++

// std lib headers
#include <string>
#include <vector>

namespace lve {

    // Structure pour stocker les d�tails de support de la cha�ne d'�change
    struct SwapChainSupportDetails {
        vk::SurfaceCapabilitiesKHR capabilities; // Capacit�s de la surface Vulkan
        std::vector<vk::SurfaceFormatKHR> formats; // Formats de surface Vulkan pris en charge
        std::vector<vk::PresentModeKHR> presentModes; // Modes de pr�sentation Vulkan pris en charge
    };

    // Structure pour stocker les indices des files d'attente requises
    struct QueueFamilyIndices {
        uint32_t graphicsFamily; // Indice de la famille de files d'attente graphiques
        uint32_t presentFamily; // Indice de la famille de files d'attente de pr�sentation
        bool graphicsFamilyHasValue = false; // Indique si l'indice de la famille de files d'attente graphiques est d�fini
        bool presentFamilyHasValue = false; // Indique si l'indice de la famille de files d'attente de pr�sentation est d�fini

        // V�rifie si les indices des files d'attente requis sont complets
        bool isComplete() { return graphicsFamilyHasValue && presentFamilyHasValue; }
    };

    class LveDevice {
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
        * @param window Une r�f�rence � la fen�tre Vulkan utilis�e pour initialiser le p�riph�rique.
        */
        LveDevice(LveWindow& window);

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
        LveDevice &operator=(const LveDevice&) = delete;

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
        vk::CommandPool getCommandPool() { return commandPool; }

        /**
         * @brief R�cup�re l'objet de p�riph�rique Vulkan.
         *
         * @return L'objet de p�riph�rique Vulkan.
         */
        vk::Device device() { return device_; }

        /**
         * @brief R�cup�re la surface Vulkan associ�e au p�riph�rique.
         *
         * @return La surface Vulkan associ�e au p�riph�rique.
         */
        vk::SurfaceKHR surface() { return surface_; }

        /**
         * @brief R�cup�re la file de commandes graphiques du p�riph�rique.
         *
         * @return La file de commandes graphiques du p�riph�rique.
         */
        vk::Queue graphicsQueue() { return graphicsQueue_; }

        /**
         * @brief R�cup�re la file de commandes de pr�sentation du p�riph�rique.
         *
         * @return La file de commandes de pr�sentation du p�riph�rique.
         */
        vk::Queue presentQueue() { return presentQueue_; }


        /**
         * @brief R�cup�re les d�tails de prise en charge de la cha�ne de balisage pour le p�riph�rique.
         *
         * @return Les d�tails de prise en charge de la cha�ne de balisage pour le p�riph�rique.
         */
        SwapChainSupportDetails getSwapChainSupport() { return querySwapChainSupport(physicalDevice); }

        vk::PhysicalDevice getPhysicalDevice() { return physicalDevice; }

        /**
         * @brief Trouve un type de m�moire appropri� pour une utilisation sp�cifi�e.
         *
         * @param typeFilter Le type de filtre de m�moire.
         * @param properties Les propri�t�s de la m�moire.
         * @return Le type de m�moire appropri�.
         */
        uint32_t findMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags properties);

        /**
         * @brief Trouve les familles de files de commandes physiques disponibles sur le p�riph�rique.
         *
         * @return Les familles de files de commandes physiques disponibles sur le p�riph�rique.
         */
        QueueFamilyIndices findPhysicalQueueFamilies() { return findQueueFamilies(physicalDevice); }

        /**
         * @brief Trouve un format d'image support� par le p�riph�rique.
         *
         * @param candidates Les formats d'image candidats.
         * @param tiling L'inclinaison de l'image.
         * @param features Les fonctionnalit�s du format de l'image.
         * @return Le format d'image support� par le p�riph�rique.
         */
        vk::Format findSupportedFormat(
            const std::vector<vk::Format>& candidates, vk::ImageTiling tiling, vk::FormatFeatureFlags features);


        // Buffer Helper Functions
        /**
         * @brief Cr�e un tampon Vulkan avec les param�tres sp�cifi�s.
         *
         * @param size La taille du tampon.
         * @param usage Les indicateurs d'utilisation du tampon.
         * @param properties Les propri�t�s de la m�moire du tampon.
         * @param buffer R�f�rence � l'objet tampon Vulkan cr��.
         * @param bufferMemory R�f�rence � l'objet m�moire du tampon Vulkan cr��.
         */
        void createBuffer(
            vk::DeviceSize size,
            vk::BufferUsageFlags usage,
            vk::MemoryPropertyFlags properties,
            vk::Buffer& buffer,
            vk::DeviceMemory& bufferMemory);

        /**
         * @brief D�marre une s�quence de commandes Vulkan temporaires.
         *
         * Cette fonction alloue un tampon de commandes Vulkan et le d�marre pour une utilisation unique.
         *
         * @return Le tampon de commandes Vulkan allou�.
         */
        vk::CommandBuffer beginSingleTimeCommands();

        /**
         * @brief Termine une s�quence de commandes Vulkan temporaires.
         *
         * Cette fonction termine l'ex�cution d'une s�quence de commandes Vulkan temporaires et lib�re les ressources associ�es.
         *
         * @param commandBuffer Le tampon de commandes Vulkan � terminer.
         */
        void endSingleTimeCommands(vk::CommandBuffer commandBuffer);

        /**
         * @brief Copie les donn�es d'un tampon source vers un tampon de destination.
         *
         * @param srcBuffer Le tampon source � partir duquel copier les donn�es.
         * @param dstBuffer Le tampon de destination o� copier les donn�es.
         * @param size La taille des donn�es � copier.
         */
        void copyBuffer(vk::Buffer srcBuffer, vk::Buffer dstBuffer, vk::DeviceSize size);

        /**
         * @brief Copie les donn�es d'un tampon vers une image Vulkan.
         *
         * Cette fonction copie les donn�es d'un tampon Vulkan vers une image Vulkan en sp�cifiant la largeur, la hauteur et le nombre de couches de l'image.
         *
         * @param buffer Le tampon contenant les donn�es � copier.
         * @param image L'image Vulkan de destination.
         * @param width La largeur de l'image.
         * @param height La hauteur de l'image.
         * @param layerCount Le nombre de couches de l'image.
         */
        void copyBufferToImage(
            vk::Buffer buffer, vk::Image image, uint32_t width, uint32_t height, uint32_t layerCount);

        /**
         * @brief Cr�e une image Vulkan avec les informations fournies.
         *
         * Cette fonction cr�e une image Vulkan en utilisant les informations sp�cifi�es dans la structure VkImageCreateInfo.
         *
         * @param imageInfo Les informations de cr�ation de l'image.
         * @param properties Les propri�t�s de la m�moire de l'image.
         * @param image R�f�rence � l'objet image Vulkan cr��.
         * @param imageMemory R�f�rence � l'objet m�moire de l'image Vulkan cr��.
         */
        void createImageWithInfo(
            const vk::ImageCreateInfo& imageInfo,
            vk::MemoryPropertyFlags properties,
            vk::Image& image,
            vk::DeviceMemory& imageMemory);

        /**
         * @brief Propri�t�s du p�riph�rique physique Vulkan associ�.
         *
         * Cette structure contient les propri�t�s du p�riph�rique physique Vulkan associ�.
         */
        vk::PhysicalDeviceProperties properties;


    private:

        /**
         * @brief Cr�e une instance Vulkan.
         *
         * Cette fonction initialise une instance Vulkan, qui repr�sente la connexion entre l'application et l'API Vulkan.
         */
        void createInstance();

        /**
         * @brief Configure le gestionnaire de d�bogage.
         *
         * Cette fonction configure un gestionnaire de d�bogage Vulkan pour recevoir les messages de validation et de d�bogage de l'API Vulkan.
         */
        void setupDebugMessenger();

        /**
         * @brief Cr�e une surface Vulkan.
         *
         * Cette fonction cr�e une surface Vulkan � partir de la fen�tre Vulkan associ�e.
         */
        void createSurface();

        /**
         * @brief S�lectionne le p�riph�rique physique Vulkan appropri�.
         *
         * Cette fonction s�lectionne le p�riph�rique physique Vulkan appropri� parmi ceux disponibles sur le syst�me.
         */
        void pickPhysicalDevice();

        /**
         * @brief Cr�e le p�riph�rique logique Vulkan.
         *
         * Cette fonction cr�e le p�riph�rique logique Vulkan qui sera utilis� pour interagir avec le mat�riel graphique.
         */
        void createLogicalDevice();

        /**
         * @brief Cr�e le pool de commandes Vulkan.
         *
         * Cette fonction cr�e un pool de commandes Vulkan, qui est utilis� pour allouer des tampons de commandes pour le p�riph�rique logique.
         */
        void createCommandPool();


        // helper functions
        /**
         * @brief V�rifie si le p�riph�rique physique Vulkan sp�cifi� convient aux besoins de l'application.
         *
         * Cette fonction examine les propri�t�s et les capacit�s du p�riph�rique pour d�terminer s'il convient � l'application.
         *
         * @param device Le p�riph�rique physique Vulkan � �valuer.
         * @return true si le p�riph�rique convient, sinon false.
         */
        bool isDeviceSuitable(vk::PhysicalDevice device);

        /**
         * @brief R�cup�re les extensions Vulkan requises par l'application.
         *
         * Cette fonction retourne un vecteur contenant les noms des extensions Vulkan n�cessaires � l'application.
         *
         * @return Un vecteur de pointeurs de cha�nes C repr�sentant les noms des extensions requises.
         */
        std::vector<const char*> getRequiredExtensions();

        /**
         * @brief V�rifie la prise en charge des couches de validation Vulkan.
         *
         * Cette fonction v�rifie si les couches de validation Vulkan sp�cifi�es sont prises en charge par l'instance Vulkan.
         *
         * @return true si les couches de validation sont prises en charge, sinon false.
         */
        bool checkValidationLayerSupport();

        /**
         * @brief Recherche les familles de files de commandes support�es par le p�riph�rique physique Vulkan sp�cifi�.
         *
         * Cette fonction identifie les indices des familles de files de commandes graphiques et de pr�sentation support�es par le p�riph�rique.
         *
         * @param device Le p�riph�rique physique Vulkan � �valuer.
         * @return Une structure contenant les indices des familles de files de commandes support�es.
         */
        QueueFamilyIndices findQueueFamilies(vk::PhysicalDevice device);

        /**
         * @brief Remplit une structure de donn�es pour la cr�ation d'un gestionnaire de d�bogage Vulkan.
         *
         * Cette fonction initialise une structure de donn�es pour la cr�ation d'un gestionnaire de d�bogage Vulkan avec les informations n�cessaires.
         *
         * @param createInfo La structure de donn�es � remplir pour la cr�ation du gestionnaire de d�bogage.
         */
        void populateDebugMessengerCreateInfo(vk::DebugUtilsMessengerCreateInfoEXT& createInfo);

        /**
         * @brief V�rifie si l'instance Vulkan poss�de les extensions requises par GLFW.
         *
         * Cette fonction v�rifie si l'instance Vulkan poss�de les extensions requises par GLFW pour cr�er une surface Vulkan.
         */
        void hasGflwRequiredInstanceExtensions();

        /**
         * @brief V�rifie si le p�riph�rique physique Vulkan sp�cifi� prend en charge les extensions de p�riph�rique n�cessaires.
         *
         * Cette fonction v�rifie si le p�riph�rique physique Vulkan sp�cifi� prend en charge les extensions de p�riph�rique requises par l'application.
         *
         * @param device Le p�riph�rique physique Vulkan � �valuer.
         * @return true si les extensions de p�riph�rique sont prises en charge, sinon false.
         */
        bool checkDeviceExtensionSupport(vk::PhysicalDevice device);

        /**
         * @brief Interroge le p�riph�rique physique Vulkan sp�cifi� pour obtenir les d�tails de support de la cha�ne d'�change.
         *
         * Cette fonction interroge le p�riph�rique physique Vulkan sp�cifi� pour obtenir les d�tails de support de la cha�ne d'�change, tels que les capacit�s, les formats de surface support�s et les modes de pr�sentation support�s.
         *
         * @param device Le p�riph�rique physique Vulkan � interroger.
         * @return Une structure contenant les d�tails de support de la cha�ne d'�change.
         */
        SwapChainSupportDetails querySwapChainSupport(vk::PhysicalDevice device);


        vk::Instance instance; /**< L'instance Vulkan utilis�e par l'application. */

        vk::DebugUtilsMessengerEXT debugMessenger; /**< Le gestionnaire de d�bogage Vulkan pour la gestion des messages de validation et de d�bogage. */

        vk::PhysicalDevice physicalDevice = VK_NULL_HANDLE; /**< Le p�riph�rique physique Vulkan utilis� par l'application, initialis� � VK_NULL_HANDLE par d�faut. */

        LveWindow& window; /**< Une r�f�rence � la fen�tre Vulkan utilis�e par l'application. */

        vk::CommandPool commandPool; /**< Le pool de commandes Vulkan utilis� pour allouer les tampons de commandes. */

        vk::Device device_; /**< Le p�riph�rique logique Vulkan utilis� par l'application. */

        vk::SurfaceKHR surface_; /**< La surface Vulkan associ�e � la fen�tre Vulkan utilis�e par l'application. */

        vk::Queue graphicsQueue_; /**< La file de commandes pour les op�rations graphiques sur le p�riph�rique logique. */

        vk::Queue presentQueue_; /**< La file de commandes pour les op�rations de pr�sentation sur le p�riph�rique logique. */

        const std::vector<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation" }; /**< Les couches de validation Vulkan activ�es par d�faut. */

        const std::vector<const char*> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME }; /**< Les extensions de p�riph�rique Vulkan utilis�es par l'application, avec l'extension de la cha�ne d'�change Vulkan activ�e par d�faut. */

    };

}  // namespace lve