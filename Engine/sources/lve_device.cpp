#include "lve_device.h"

// std headers
#include <cstring>
#include <iostream>
#include <set>
#include <unordered_set>

#include "VulkanHooks.h"

namespace lve
{
	// local callback functions
	/**
	 * @brief Fonction de rappel de d�bogage pour la gestion des messages de validation Vulkan.
	 *
	 * Cette fonction est appel�e par les couches de validation Vulkan pour g�rer les messages de d�bogage.
	 * Elle affiche les messages de d�bogage dans la console d'erreur standard.
	 *
	 * @param messageSeverity La s�v�rit� du message de d�bogage.
	 * @param messageType Le type de message de d�bogage.
	 * @param pCallbackData Les donn�es de rappel du messager de d�bogage.
	 * @param pUserData Des donn�es utilisateur optionnelles.
	 * @return VK_FALSE indiquant que le traitement du message est termin�, et aucune action suppl�mentaire n'est n�cessaire.
	 */
	static VKAPI_ATTR vk::Bool32 VKAPI_CALL debugCallback(
		vk::DebugUtilsMessageSeverityFlagBitsEXT      messageSeverity,
		vk::DebugUtilsMessageTypeFlagsEXT             messageType,
		const vk::DebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void*                                         pUserData)
	{
		// Affiche le message de d�bogage dans la console d'erreur standard
		std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

		// Indique que le traitement du message est termin�, et qu'aucune action suppl�mentaire n'est n�cessaire
		return VK_FALSE;
	}


	/**
	 * @brief Cr�e un gestionnaire de messager de d�bogage Vulkan en utilisant l'extension VK_EXT_debug_utils.
	 *
	 * Cette fonction cr�e un gestionnaire de messager de d�bogage Vulkan en utilisant l'extension VK_EXT_debug_utils.
	 * Elle utilise la fonction vkCreateDebugUtilsMessengerEXT, si elle est disponible.
	 *
	 * @param instance L'instance Vulkan.
	 * @param pCreateInfo Les informations de cr�ation du gestionnaire de messager de d�bogage.
	 * @param pAllocator L'allocation de rappels, permettant la personnalisation de la gestion de la m�moire.
	 * @param pDebugMessenger Un pointeur vers l'objet de gestionnaire de messager de d�bogage � cr�er.
	 * @return VK_SUCCESS si le gestionnaire de messager de d�bogage a �t� cr�� avec succ�s, VK_ERROR_EXTENSION_NOT_PRESENT si l'extension n'est pas prise en charge.
	 */
	vk::Result CreateDebugUtilsMessengerEXT(vk::Instance                                instance,
	                                        const vk::DebugUtilsMessengerCreateInfoEXT* pCreateInfo,
	                                        const vk::AllocationCallbacks*              pAllocator,
	                                        vk::DebugUtilsMessengerEXT*                 pDebugMessenger)
	{
		const PFN_vkVoidFunction function = instance.getProcAddr("vkCreateDebugUtilsMessengerEXT");
		const auto messenger_ext = reinterpret_cast<PFN_vkGetInstanceProcAddr>(function);
		const vk::DispatchLoaderDynamic dispatch(instance, messenger_ext);
		const vk::Result result = instance.createDebugUtilsMessengerEXT(pCreateInfo, pAllocator, pDebugMessenger,
		                                                                dispatch);
		return result;
	}


	/**
	 * @brief D�truit un gestionnaire de messager de d�bogage Vulkan cr�� avec l'extension VK_EXT_debug_utils.
	 *
	 * Cette fonction d�truit un gestionnaire de messager de d�bogage Vulkan cr�� avec l'extension VK_EXT_debug_utils.
	 * Elle utilise la fonction vkDestroyDebugUtilsMessengerEXT, si elle est disponible.
	 *
	 * @param _instance L'instance Vulkan.
	 * @param _debugMessenger L'objet de gestionnaire de messager de d�bogage � d�truire.
	 * @param _pAllocator L'allocation de rappels, permettant la personnalisation de la gestion de la m�moire.
	 */
	void DestroyDebugUtilsMessengerEXT(
		const vk::Instance               _instance,
		const vk::DebugUtilsMessengerEXT _debugMessenger,
		const vk::AllocationCallbacks*   _pAllocator)
	{
		// R�cup�re le pointeur de fonction pour vkDestroyDebugUtilsMessengerEXT

		// V�rifie si la fonction est disponible
		if (const auto func = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(
			_instance.getProcAddr("vkDestroyDebugUtilsMessengerEXT")); func != nullptr)
			// Appelle la fonction vkDestroyDebugUtilsMessengerEXT pour d�truire le gestionnaire de messager de d�bogage
			func(_instance, static_cast<VkDebugUtilsMessengerEXT>(_debugMessenger),
			     reinterpret_cast<const VkAllocationCallbacks*>(_pAllocator));
	}


	// class member functions

	// Constructeur de la classe LveDevice
	LveDevice::LveDevice(LveWindow& _window) : window{_window}
	{
		CreateInstance();      // Cr�e une instance Vulkan
		SetupDebugMessenger(); // Configure le messager de d�bogage Vulkan
		CreateSurface();       // Cr�e la surface de la fen�tre Vulkan
		PickPhysicalDevice();  // S�lectionne le p�riph�rique physique adapt�
		CreateLogicalDevice(); // Cr�e le p�riph�rique logique Vulkan
		CreateCommandPool();   // Cr�e le pool de commandes Vulkan
	}

	// Destructeur de la classe LveDevice
	LveDevice::~LveDevice()
	{
		// D�truit le pool de commandes Vulkan
		device_.destroyCommandPool(commandPool, nullptr);

		// D�truit le p�riph�rique logique Vulkan
		device_.destroy(nullptr);

		// Si les couches de validation sont activ�es, d�truit le messager de d�bogage Vulkan
		if (enableValidationLayers) DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);

		// D�truit la surface Vulkan associ�e � la fen�tre
		instance.destroySurfaceKHR(surface_, nullptr);

		// D�truit l'instance Vulkan
		instance.destroy(nullptr);
	}


	/**
	 * @brief Cr�e une instance Vulkan pour l'application.
	 *
	 * Cette fonction configure et cr�e une instance Vulkan, qui est la premi�re �tape dans l'utilisation de l'API Vulkan.
	 * Elle configure les informations d'application, les extensions requises, les couches de validation, et cr�e l'instance Vulkan correspondante.
	 *
	 * @throws std::runtime_error si la cr�ation de l'instance �choue ou si les couches de validation sont activ�es mais non disponibles.
	 */
	vk::Instance LveDevice::CreateInstance()
	{
		// V�rifie si les couches de validation sont activ�es et si elles sont support�es
		if (enableValidationLayers && !CheckValidationLayerSupport())
			throw std::runtime_error(
				"validation layers requested, but not available!");

		// Configuration des informations d'application pour l'instance Vulkan
		constexpr vk::ApplicationInfo app_info(
			"Vulkanity App",
			VK_MAKE_VERSION(1, 0, 0),
			"Vulkanity",
			VK_MAKE_VERSION(1, 0, 0),
			VK_API_VERSION_1_0
		);

		// Obtention des extensions Vulkan requises pour l'application
		const auto extensions = GetRequiredExtensions();

		// Configuration de la cr�ation de l'instance Vulkan
		vk::InstanceCreateInfo create_info(
			{},
			&app_info, // Initialisation directe avec l'adresse de appInfo
			0, nullptr, // Aucune couche de validation
			static_cast<uint32_t>(extensions.size()), extensions.data() // Extensions activ�es
		);

		// Configuration du messager de d�bogage Vulkan si les couches de validation sont activ�es
		vk::DebugUtilsMessengerCreateInfoEXT debug_create_info;
		if (enableValidationLayers)
		{
			create_info.enabledLayerCount   = static_cast<uint32_t>(validationLayers.size());
			create_info.ppEnabledLayerNames = validationLayers.data();

			PopulateDebugMessengerCreateInfo(debug_create_info);
			create_info.pNext = &debug_create_info;
		}
		else
		{
			create_info.enabledLayerCount = 0;
			create_info.pNext             = nullptr;
		}
		instance = createInstance(create_info);
		// V�rification des extensions GLFW requises pour l'instance Vulkan
		HasGlfwRequiredInstanceExtensions();

		return instance;
	}

	/**
	 * @brief S�lectionne le p�riph�rique physique adapt� pour l'application.
	 *
	 * Cette fonction r�cup�re la liste des p�riph�riques physiques Vulkan disponibles sur le syst�me,
	 * puis elle it�re � travers chaque p�riph�rique pour trouver celui qui convient le mieux � l'application.
	 * Le p�riph�rique s�lectionn� est stock� dans la variable physicalDevice.
	 *
	 * @throws Une exception si aucun p�riph�rique physique appropri� n'est trouv�.
	 */
	void LveDevice::PickPhysicalDevice()
	{
		// R�cup�re la liste des p�riph�riques physiques Vulkan disponibles sur le syst�me
		std::vector<vk::PhysicalDevice> devices = instance.enumeratePhysicalDevices();

		// V�rifie si des p�riph�riques Vulkan ont �t� trouv�s
		if (devices.empty()) throw std::runtime_error("failed to find GPUs with Vulkan support!");

		// Affiche le nombre de p�riph�riques Vulkan trouv�s
		std::cout << "Device count: " << devices.size() << std::endl;

		// Parcourt chaque p�riph�rique pour trouver celui qui convient le mieux � l'application
		for (const auto& device : devices)
		{
			if (IsDeviceSuitable(device))
			{
				physicalDevice = device;
				break;
			}
		}

		// V�rifie si un p�riph�rique adapt� a �t� trouv�
		if (!physicalDevice) throw std::runtime_error("failed to find a suitable GPU!");
		// R�cup�re les propri�t�s du p�riph�rique s�lectionn� et les affiche
		vk::PhysicalDeviceProperties properties = physicalDevice.getProperties();
		std::cout << "Physical device: " << properties.deviceName << std::endl;
	}


	/**
	* @brief Cr�e un p�riph�rique logique Vulkan.
	*
	* Cette fonction cr�e un p�riph�rique logique Vulkan � partir du p�riph�rique physique s�lectionn�.
	* Elle configure les files de commandes pour les op�rations graphiques et de pr�sentation, ainsi que les fonctionnalit�s du p�riph�rique.
	*
	* @throws Une exception si la cr�ation du p�riph�rique logique �choue.
	*/
	void LveDevice::CreateLogicalDevice()
	{
		// Recherche des familles de files de commandes pour le p�riph�rique physique s�lectionn�
		QueueFamilyIndices indices = FindQueueFamilies(physicalDevice);

		// Configuration des files de commandes pour les op�rations graphiques et de pr�sentation
		std::vector<vk::DeviceQueueCreateInfo> queue_create_infos;
		std::set<uint32_t>                     unique_queue_families = {indices.graphicsFamily, indices.presentFamily};
		float                                  queue_priority        = 1.0f;
		for (uint32_t queue_family : unique_queue_families)
		{
			vk::DeviceQueueCreateInfo queue_create_info(
				{},
				queue_family,
				1,
				&queue_priority
			);
			queue_create_infos.push_back(queue_create_info);
		}

		// Configuration des fonctionnalit�s du p�riph�rique
		vk::PhysicalDeviceFeatures device_features;
		device_features.samplerAnisotropy = VK_TRUE;

		// Configuration de la cr�ation du p�riph�rique logique
		vk::DeviceCreateInfo create_info;
		create_info.queueCreateInfoCount    = static_cast<uint32_t>(queue_create_infos.size());
		create_info.pQueueCreateInfos       = queue_create_infos.data();
		create_info.pEnabledFeatures        = &device_features;
		create_info.enabledExtensionCount   = static_cast<uint32_t>(deviceExtensions.size());
		create_info.ppEnabledExtensionNames = deviceExtensions.data();

		// Configuration des couches de validation si elles sont activ�es
		if (enableValidationLayers)
		{
			create_info.enabledLayerCount   = static_cast<uint32_t>(validationLayers.size());
			create_info.ppEnabledLayerNames = validationLayers.data();
		}

		// Cr�ation du p�riph�rique logique Vulkan
		device_ = physicalDevice.createDevice(create_info);

		// Obtention des files de commandes graphiques et de pr�sentation du p�riph�rique logique
		graphicsQueue_ = device_.getQueue(indices.graphicsFamily, 0);
		presentQueue_  = device_.getQueue(indices.presentFamily, 0);
	}


	/**
	 * @brief Cr�e un pool de commandes Vulkan.
	 *
	 * Cette fonction cr�e un pool de commandes Vulkan pour le p�riph�rique logique actuel.
	 * Le pool de commandes est associ� � la famille de files de commandes graphiques trouv�e pour ce p�riph�rique.
	 * Les tampons de commandes cr��s � partir de ce pool auront les drapeaux VK_COMMAND_POOL_CREATE_TRANSIENT_BIT
	 * et VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT, ce qui permet des op�rations transitoires et de r�initialiser
	 * les tampons de commandes.
	 *
	 * @throw std::runtime_error si la cr�ation du pool de commandes �choue.
	 */
	void LveDevice::CreateCommandPool()
	{
		// Recherche les indices des familles de files de commandes support�es par le p�riph�rique physique actuel.
		const QueueFamilyIndices queue_family_indices = FindPhysicalQueueFamilies();

		// Initialise la structure d'informations du pool de commandes Vulkan.
		const vk::CommandPoolCreateInfo pool_info(
			vk::CommandPoolCreateFlags(
				vk::CommandPoolCreateFlagBits::eTransient | vk::CommandPoolCreateFlagBits::eResetCommandBuffer),
			queue_family_indices.graphicsFamily
		);

		// Cr�e le pool de commandes Vulkan en utilisant les informations fournies.
		try
		{
			commandPool = device_.createCommandPool(pool_info);
		}
		catch (const vk::SystemError& e)
		{
			// Lance une exception si la cr�ation du pool de commandes �choue.
			throw std::runtime_error("failed to create command pool!");
		}
	}


	/**
	 * @brief Cr�e une surface Vulkan associ�e � la fen�tre de l'application.
	 *
	 * Cette fonction utilise la fen�tre de l'application pour cr�er une surface Vulkan, qui est n�cessaire
	 * pour interagir avec la fen�tre et effectuer le rendu graphique.
	 *
	 * @throw std::runtime_error si la cr�ation de la surface �choue.
	 */
	void LveDevice::CreateSurface() { window.CreateWindowSurface(instance, &surface_); }

	/**
	 * @brief V�rifie si le p�riph�rique physique Vulkan sp�cifi� convient aux besoins de l'application.
	 *
	 * Cette fonction examine les propri�t�s et les capacit�s du p�riph�rique pour d�terminer s'il convient � l'application.
	 *
	 * @param _device
	 * @param device Le p�riph�rique physique Vulkan � �valuer.
	 * @return true si le p�riph�rique convient, sinon false.
	 */
	bool LveDevice::IsDeviceSuitable(vk::PhysicalDevice _device)
	{
		// Recherche des indices des familles de files de commandes support�es par le p�riph�rique physique.
		QueueFamilyIndices indices = FindQueueFamilies(_device);

		// V�rification de la pris000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000e en charge des extensions de p�riph�rique n�cessaires.
		bool extensions_supported = CheckDeviceExtensionSupport(_device);

		// V�rification de l'ad�quation de la cha�ne d'�change.
		bool swapChainAdequate = false;
		if (extensions_supported)
		{
			// Interrogation du p�riph�rique pour obtenir les d�tails de support de la cha�ne d'�change.
			SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(_device);
			swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
		}

		// R�cup�ration des fonctionnalit�s support�es par le p�riph�rique.
		vk::PhysicalDeviceFeatures supportedFeatures = _device.getFeatures();

		// V�rification des fonctionnalit�s requises.
		return indices.IsComplete() && extensions_supported && swapChainAdequate &&
		       supportedFeatures.samplerAnisotropy;
	}

	/**
	 * @brief Remplit une structure de donn�es pour la cr�ation d'un gestionnaire de d�bogage Vulkan.
	 *
	 * Cette fonction initialise une structure de donn�es pour la cr�ation d'un gestionnaire de d�bogage Vulkan avec les informations n�cessaires.
	 *
	 * @param createInfo La structure de donn�es � remplir pour la cr�ation du gestionnaire de d�bogage.
	 */
	void LveDevice::PopulateDebugMessengerCreateInfo(
		vk::DebugUtilsMessengerCreateInfoEXT& _createInfo)
	{
		// Sp�cifie le type de la structure.
		_createInfo.sType = vk::StructureType::eDebugUtilsMessengerCreateInfoEXT;

		// Sp�cifie les niveaux de s�v�rit� des messages � intercepter.
		_createInfo.setMessageSeverity(
			vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
			vk::DebugUtilsMessageSeverityFlagBitsEXT::eError);

		// Sp�cifie les types de messages � intercepter.
		_createInfo.setMessageType(
			vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral |
			vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation |
			vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance);

		// Sp�cifie la fonction de rappel � appeler pour chaque message de d�bogage.
		_createInfo.setPfnUserCallback(reinterpret_cast<PFN_vkDebugUtilsMessengerCallbackEXT>(debugCallback));

		// Sp�cifie des donn�es utilisateur facultatives.
		_createInfo.setPUserData(nullptr); // Optionnel
	}


	/**
	 * @brief Configure le gestionnaire de d�bogage Vulkan.
	 *
	 * Cette fonction configure le gestionnaire de d�bogage Vulkan si les couches de validation sont activ�es.
	 *
	 * Elle utilise la fonction populateDebugMessengerCreateInfo() pour remplir les informations n�cessaires pour
	 * la cr�ation du gestionnaire de d�bogage.
	 *
	 * @throw std::runtime_error si la configuration du gestionnaire de d�bogage �choue.
	 */
	vk::DebugUtilsMessengerEXT LveDevice::SetupDebugMessenger()
	{
		// V�rifie si les couches de validation sont activ�es.
		if (!enableValidationLayers) return VK_NULL_HANDLE;

		// Initialise une structure pour la cr�ation du gestionnaire de d�bogage.
		vk::DebugUtilsMessengerCreateInfoEXT create_info;
		PopulateDebugMessengerCreateInfo(create_info);

		if (const auto dispatcher = vk::DispatchLoaderDynamic(instance, vkGetInstanceProcAddr);
			instance.createDebugUtilsMessengerEXT(&create_info, nullptr, &debugMessenger, dispatcher) !=
			vk::Result::eSuccess)
			throw std::runtime_error("failed to set up debug messenger!");

		return debugMessenger;
	}


	/**
	 * @brief V�rifie la prise en charge des couches de validation Vulkan.
	 *
	 * Cette fonction v�rifie si toutes les couches de validation sp�cifi�es dans la variable `validationLayers`
	 * sont prises en charge par Vulkan sur le syst�me.
	 *
	 * @return true si toutes les couches de validation sont prises en charge, sinon false.
	 */
	bool LveDevice::CheckValidationLayerSupport() const
	{
		// R�cup�re les propri�t�s de toutes les couches de validation disponibles.
		const auto available_layers = vk::enumerateInstanceLayerProperties();

		// Parcourt toutes les couches de validation requises.
		for (const char* layer_name : validationLayers)
		{
			bool layer_found = false;

			// V�rifie si la couche de validation requise est pr�sente parmi les couches disponibles.
			for (const auto& layer_properties : available_layers)
			{
				if (strcmp(layer_name, layer_properties.layerName) == 0)
				{
					layer_found = true;
					break;
				}
			}

			// Si la couche de validation requise n'est pas trouv�e, retourne false.
			if (!layer_found) return false;
		}

		// Si toutes les couches de validation requises sont trouv�es, retourne true.
		return true;
	}


	/**
	 * @brief R�cup�re les extensions Vulkan requises pour l'instance Vulkan.
	 *
	 * Cette fonction r�cup�re les extensions Vulkan requises pour l'instance Vulkan en utilisant la biblioth�que GLFW.
	 * Elle inclut les extensions requises par GLFW et, si les couches de validation sont activ�es, l'extension de
	 * gestionnaire de d�bogage Vulkan.
	 *
	 * @return Un vecteur de pointeurs vers les noms des extensions Vulkan requises.
	 */
	std::vector<const char*> LveDevice::GetRequiredExtensions() const
	{
		// R�cup�re le nombre d'extensions requises par GLFW.
		uint32_t     glfw_extension_count = 0;
		const char** glfw_extensions      = glfwGetRequiredInstanceExtensions(&glfw_extension_count);

		// Convertit les noms des extensions GLFW en un vecteur de pointeurs vers les noms des extensions Vulkan.
		std::vector<const char*> extensions(glfw_extensions, glfw_extensions + glfw_extension_count);

		// Ajoute l'extension de gestionnaire de d�bogage Vulkan si les couches de validation sont activ�es.
		if (enableValidationLayers) extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

		// Retourne le vecteur contenant les noms des extensions Vulkan requises.
		return extensions;
	}


	/**
	  * @brief V�rifie la disponibilit� des extensions Vulkan requises par GLFW.
	  *
	  * Cette fonction v�rifie si toutes les extensions Vulkan requises par GLFW sont disponibles sur le syst�me.
	  * Elle r�cup�re d'abord toutes les extensions Vulkan disponibles, puis compare avec les extensions requises
	  * obtenues � partir de la fonction getRequiredExtensions(). Si une extension requise est manquante, une exception
	  * est lev�e.
	  *
	  * @throw std::runtime_error si une extension requise par GLFW est manquante.
	  */
	void LveDevice::HasGlfwRequiredInstanceExtensions() const
	{
		// R�cup�re le nombre d'extensions Vulkan disponibles.
		const auto available_extensions = vk::enumerateInstanceExtensionProperties();

		// Affiche les extensions Vulkan disponibles et les stocke dans un ensemble pour une recherche rapide.
		std::cout << "available extensions:" << std::endl;
		std::unordered_set<std::string> available;
		for (const auto& extension : available_extensions)
		{
			std::cout << "\t" << extension.extensionName << std::endl;
			available.insert(extension.extensionName);
		}

		// Affiche les extensions Vulkan requises par GLFW et v�rifie leur disponibilit�.
		std::cout << "required extensions:" << std::endl;
		for (const auto required_extensions = GetRequiredExtensions(); const auto& required : required_extensions)
		{
			std::cout << "\t" << required << std::endl;
			if (!available.contains(required))
				// Lance une exception si une extension requise est manquante.
				throw std::runtime_error("Missing required glfw extension");
		}
	}


	/**
	 * @brief V�rifie la prise en charge des extensions de p�riph�rique Vulkan par le p�riph�rique physique sp�cifi�.
	 *
	 * Cette fonction v�rifie si toutes les extensions de p�riph�rique Vulkan requises par l'application sont prises en charge
	 * par le p�riph�rique physique Vulkan sp�cifi�.
	 *
	 * @param _device
	 * @param device Le p�riph�rique physique Vulkan � v�rifier.
	 * @return true si toutes les extensions de p�riph�rique requises sont prises en charge, sinon false.
	 */
	bool LveDevice::CheckDeviceExtensionSupport(const vk::PhysicalDevice _device) const
	{
		// R�cup�re les propri�t�s de toutes les extensions de p�riph�rique Vulkan disponibles pour le p�riph�rique sp�cifi�.
		const auto available_extensions = _device.enumerateDeviceExtensionProperties(nullptr);

		// Cr�e un ensemble contenant toutes les extensions de p�riph�rique requises par l'application.
		std::set<std::string> required_extensions(deviceExtensions.begin(), deviceExtensions.end());

		// Parcourt toutes les extensions de p�riph�rique disponibles.
		for (const auto& extension : available_extensions)
		{
			// Supprime les extensions disponibles de l'ensemble des extensions requises.
			required_extensions.erase(extension.extensionName);
		}

		// Si l'ensemble des extensions requises est vide, cela signifie que toutes les extensions requises sont prises en charge.
		return required_extensions.empty();
	}


	/**
	 * @brief Recherche et retourne les indices des files d'attente de p�riph�riques pour le p�riph�rique physique sp�cifi�.
	 *
	 * Cette fonction recherche les indices des files d'attente de p�riph�riques pour les op�rations de rendu graphique et de pr�sentation
	 * pour le p�riph�rique physique Vulkan sp�cifi�.
	 *
	 * @param _device Le p�riph�rique physique Vulkan pour lequel les files d'attente doivent �tre recherch�es.
	 * @return Une structure QueueFamilyIndices contenant les indices des files d'attente pour les op�rations de rendu graphique et de pr�sentation.
	 */
	QueueFamilyIndices LveDevice::FindQueueFamilies(const vk::PhysicalDevice _device) const
	{
		// Initialise la structure QueueFamilyIndices pour stocker les indices des files d'attente.
		QueueFamilyIndices indices;

		// R�cup�re le nombre de familles de files d'attente de p�riph�riques disponibles pour le p�riph�rique sp�cifi�.
		const auto queue_families = _device.getQueueFamilyProperties();

		// Parcourt toutes les familles de files d'attente pour trouver celles qui prennent en charge les op�rations de rendu graphique et de pr�sentation.
		int i = 0;
		for (const auto& queue_family : queue_families)
		{
			// V�rifie si la famille de files d'attente prend en charge les op�rations de rendu graphique.
			if (queue_family.queueCount > 0 && queue_family.queueFlags & vk::QueueFlagBits::eGraphics)
			{
				indices.graphicsFamily         = i;
				indices.graphicsFamilyHasValue = true;
			}
			// V�rifie si la famille de files d'attente prend en charge la pr�sentation sur la surface associ�e.
			if (const bool presentSupport = _device.getSurfaceSupportKHR(i, surface_);
				queue_family.queueCount > 0 && presentSupport)
			{
				indices.presentFamily         = i;
				indices.presentFamilyHasValue = true;
			}
			// Si les indices requis ont �t� trouv�s, arr�te la recherche.
			if (indices.IsComplete()) break;

			i++;
		}

		// Retourne les indices des files d'attente trouv�es.
		return indices;
	}


	/**
	 * @brief Interroge et retourne les d�tails de prise en charge de la cha�ne d'�change de swap (SwapChain) pour le p�riph�rique physique sp�cifi�.
	 *
	 * Cette fonction interroge le p�riph�rique physique Vulkan sp�cifi� pour obtenir les d�tails de prise en charge de la cha�ne d'�change de swap, tels que
	 * les capacit�s de surface, les formats de surface pris en charge et les modes de pr�sentation pris en charge.
	 *
	 * @param _device Le p�riph�rique physique Vulkan pour lequel les d�tails de prise en charge de la cha�ne d'�change de swap doivent �tre interrog�s.
	 * @return Une structure SwapChainSupportDetails contenant les d�tails de prise en charge de la cha�ne d'�change de swap.
	 */
	SwapChainSupportDetails LveDevice::QuerySwapChainSupport(const vk::PhysicalDevice _device) const
	{
		// Initialise la structure SwapChainSupportDetails pour stocker les d�tails de prise en charge de la cha�ne d'�change de swap.
		SwapChainSupportDetails details;

		// Interroge le p�riph�rique physique Vulkan pour obtenir les capacit�s de la surface associ�e.
		details.capabilities = _device.getSurfaceCapabilitiesKHR(surface_);

		// R�cup�re les formats de surface pris en charge par le p�riph�rique.
		details.formats = _device.getSurfaceFormatsKHR(surface_);

		// R�cup�re les modes de pr�sentation pris en charge par le p�riph�rique.
		details.presentModes = _device.getSurfacePresentModesKHR(surface_);

		// Retourne les d�tails de prise en charge de la cha�ne d'�change de swap.
		return details;
	}


	/**
	 * @brief Recherche un format d'image support� parmi une liste de formats candidats.
	 *
	 * Cette fonction recherche un format d'image support� parmi une liste de formats candidats,
	 * en tenant compte du mode de tiling sp�cifi� (lin�aire ou optimal) et des fonctionnalit�s requises.
	 *
	 * @param _candidates La liste des formats candidats � v�rifier.
	 * @param _tiling Le mode de tiling de l'image (VK_IMAGE_TILING_LINEAR ou VK_IMAGE_TILING_OPTIMAL).
	 * @param _features Les fonctionnalit�s requises pour le format.
	 * @return Le format d'image support� trouv�.
	 * @throws std::runtime_error si aucun format d'image support� n'est trouv�.
	 */
	vk::Format LveDevice::FindSupportedFormat(
		const std::vector<vk::Format>& _candidates, const vk::ImageTiling _tiling,
		const vk::FormatFeatureFlags   _features) const
	{
		// Parcourt chaque format d'image candidat
		for (const vk::Format format : _candidates)
		{
			vk::FormatProperties props;
			// Obtient les propri�t�s du format
			props = physicalDevice.getFormatProperties(format);

			// V�rifie si le format est adapt� au mode de tiling sp�cifi� (lin�aire ou optimal)
			if (_tiling == vk::ImageTiling::eLinear && (props.linearTilingFeatures & _features) == _features)
				// Si le format est adapt� au mode lin�aire et prend en charge toutes les fonctionnalit�s requises, le retourne
				return format;
			if (_tiling == vk::ImageTiling::eOptimal && (props.optimalTilingFeatures & _features) == _features)
				// Si le format est adapt� au mode optimal et prend en charge toutes les fonctionnalit�s requises, le retourne
				return format;
		}
		// Si aucun format d'image trouv� ne r�pond aux crit�res sp�cifi�s, lance une exception
		throw std::runtime_error("failed to find supported format!");
	}


	/**
	 * @brief Recherche un type de m�moire adapt� pour les allocations de m�moire graphique.
	 *
	 * Cette fonction recherche un type de m�moire adapt� pour les allocations de m�moire graphique,
	 * en tenant compte du filtre de type sp�cifi� et des propri�t�s de m�moire requises.
	 *
	 * @param _typeFilter Le filtre de type de m�moire sp�cifi�.
	 * @param _properties Les propri�t�s de m�moire requises.
	 * @return L'index du type de m�moire adapt� trouv�.
	 * @throws std::runtime_error si aucun type de m�moire adapt� n'est trouv�.
	 */
	uint32_t LveDevice::FindMemoryType(const uint32_t _typeFilter, const vk::MemoryPropertyFlags _properties) const
	{
		const vk::PhysicalDeviceMemoryProperties mem_properties = physicalDevice.getMemoryProperties();

		// Parcourt chaque type de m�moire
		for (uint32_t i = 0; i < mem_properties.memoryTypeCount; i++)
		{
			// V�rifie si le type de m�moire est compatible avec le filtre sp�cifi� et poss�de toutes les propri�t�s requises
			if ((_typeFilter & (1 << i)) &&
			    (mem_properties.memoryTypes[i].propertyFlags & _properties) == _properties)
				// Si le type de m�moire convient, retourne son index
				return i;
		}
		// Si aucun type de m�moire adapt� n'est trouv�, lance une exception
		throw std::runtime_error("failed to find suitable memory type!");
	}


	/**
	 * @brief Cr�e un tampon Vulkan avec la taille, l'utilisation et les propri�t�s sp�cifi�es.
	 *
	 * Cette fonction cr�e un tampon Vulkan avec la taille, l'utilisation et les propri�t�s sp�cifi�es,
	 * et associe la m�moire appropri�e au tampon.
	 *
	 * @param _size La taille du tampon en octets.
	 * @param _usage Les indicateurs d'utilisation du tampon.
	 * @param _properties Les propri�t�s de m�moire du tampon.
	 * @param _buffer Une r�f�rence � l'objet tampon � cr�er.
	 * @param _bufferMemory Une r�f�rence � la m�moire allou�e pour le tampon.
	 * @throws std::runtime_error en cas d'�chec de la cr�ation ou de l'allocation de m�moire pour le tampon.
	 */
	void LveDevice::CreateBuffer(
		const vk::DeviceSize          _size,
		const vk::BufferUsageFlags    _usage,
		const vk::MemoryPropertyFlags _properties,
		vk::Buffer&                   _buffer,
		vk::DeviceMemory&             _bufferMemory) const
	{
		// D�finit les informations du tampon
		const vk::BufferCreateInfo buffer_info(
			{},
			_size,
			_usage,
			vk::SharingMode::eExclusive
		);

		// Cr�e le tampon
		_buffer = device_.createBuffer(buffer_info);

		// Obtient les exigences de m�moire pour le tampon
		const vk::MemoryRequirements mem_requirements = device_.getBufferMemoryRequirements(_buffer);

		// Alloue la m�moire pour le tampon
		const vk::MemoryAllocateInfo alloc_info(
			mem_requirements.size,
			FindMemoryType(mem_requirements.memoryTypeBits, _properties)
		);

		_bufferMemory = device_.allocateMemory(alloc_info);

		// Associe la m�moire allou�e au tampon
		device_.bindBufferMemory(_buffer, _bufferMemory, 0);
	}


	/**
	 * @brief D�marre une s�rie de commandes Vulkan pour une utilisation unique.
	 *
	 * Cette fonction alloue et d�marre un tampon de commandes Vulkan pour une utilisation unique.
	 * Le tampon de commandes retourn� peut �tre utilis� pour ex�cuter des commandes Vulkan
	 * qui doivent �tre ex�cut�es une seule fois.
	 *
	 * @return Le tampon de commandes Vulkan allou� et d�marr�.
	 * @throws std::runtime_error en cas d'�chec de l'allocation du tampon de commandes.
	 */
	vk::CommandBuffer LveDevice::BeginSingleTimeCommands() const
	{
		// Alloue un tampon de commandes
		const vk::CommandBufferAllocateInfo alloc_info(
			commandPool,
			vk::CommandBufferLevel::ePrimary,
			1
		);

		const vk::CommandBuffer command_buffer = device_.allocateCommandBuffers(alloc_info)[0];

		// D�marre l'enregistrement des commandes dans le tampon
		constexpr vk::CommandBufferBeginInfo begin_info(
			vk::CommandBufferUsageFlagBits::eOneTimeSubmit
		);

		command_buffer.begin(begin_info);

		return command_buffer;
	}


	/**
	 * @brief Termine une s�rie de commandes Vulkan pour une utilisation unique.
	 *
	 * Cette fonction termine l'enregistrement des commandes dans le tampon de commandes sp�cifi�,
	 * soumet les commandes � la file de commandes graphiques pour ex�cution, attend la fin de l'ex�cution
	 * des commandes, puis lib�re le tampon de commandes.
	 *
	 * @param _commandBuffer Le tampon de commandes Vulkan � terminer et � soumettre.
	 * @throws std::runtime_error en cas d'�chec de soumission des commandes ou d'attente de la fin de l'ex�cution.
	 */
	void LveDevice::EndSingleTimeCommands(const vk::CommandBuffer _commandBuffer) const
	{
		// Termine l'enregistrement des commandes dans le tampon de commandes
		_commandBuffer.end();

		// Soumet les commandes � la file de commandes graphiques pour ex�cution
		vk::SubmitInfo submit_info;
		submit_info.commandBufferCount = 1;
		submit_info.pCommandBuffers    = &_commandBuffer;

		graphicsQueue_.submit(submit_info, nullptr);

		// Attend la fin de l'ex�cution des commandes
		graphicsQueue_.waitIdle();

		// Lib�re le tampon de commandes
		device_.freeCommandBuffers(commandPool, 1, &_commandBuffer);
	}


	/**
	 * @brief Copie les donn�es d'un tampon source vers un tampon de destination.
	 *
	 * Cette fonction utilise un tampon de commandes temporaire pour copier les donn�es
	 * du tampon source vers le tampon de destination. Elle garantit que l'op�ration de
	 * copie se produit de mani�re synchrone, c'est-�-dire que le tampon de commandes temporaire
	 * est soumis � la file de commandes graphiques, attend que toutes les op�rations en cours
	 * sur cette file soient termin�es, puis est lib�r�.
	 *
	 * @param _srcBuffer Le tampon source � partir duquel copier les donn�es.
	 * @param _dstBuffer Le tampon de destination vers lequel copier les donn�es.
	 * @param _size La taille des donn�es � copier, en octets.
	 * @throws std::runtime_error en cas d'�chec de l'allocation ou de l'enregistrement des commandes.
	 */
	void LveDevice::CopyBuffer(const vk::Buffer     _srcBuffer, const vk::Buffer _dstBuffer,
	                           const vk::DeviceSize _size) const
	{
		// D�marre l'enregistrement des commandes dans un tampon de commandes temporaire
		const vk::CommandBuffer command_buffer = BeginSingleTimeCommands();

		// D�finit la r�gion de copie
		vk::BufferCopy copyRegion{};
		copyRegion.srcOffset = 0; // Facultatif
		copyRegion.dstOffset = 0; // Facultatif
		copyRegion.size      = _size;

		// Effectue la copie des donn�es entre les tampons
		command_buffer.copyBuffer(_srcBuffer, _dstBuffer, 1, &copyRegion);

		// Termine l'enregistrement des commandes et soumet le tampon de commandes � la file de commandes graphiques
		EndSingleTimeCommands(command_buffer);
	}


	/**
	 * @brief Copie les donn�es d'un tampon vers une image Vulkan.
	 *
	 * Cette fonction utilise un tampon de commandes temporaire pour copier les donn�es
	 * du tampon vers l'image Vulkan. Elle garantit que l'op�ration de copie se produit de mani�re
	 * synchrone, c'est-�-dire que le tampon de commandes temporaire est soumis � la file de commandes
	 * graphiques, attend que toutes les op�rations en cours sur cette file soient termin�es, puis est lib�r�.
	 *
	 * @param _buffer Le tampon contenant les donn�es � copier vers l'image.
	 * @param _image L'image Vulkan de destination vers laquelle copier les donn�es.
	 * @param _width La largeur de l'image en pixels.
	 * @param _height La hauteur de l'image en pixels.
	 * @param _layerCount Le nombre de couches de l'image.
	 * @throws std::runtime_error en cas d'�chec de l'allocation ou de l'enregistrement des commandes.
	 */
	void LveDevice::CopyBufferToImage(
		const vk::Buffer _buffer, const vk::Image _image, const uint32_t _width, const uint32_t _height,
		const uint32_t   _layerCount) const
	{
		// D�marre l'enregistrement des commandes dans un tampon de commandes temporaire
		const vk::CommandBuffer command_buffer = BeginSingleTimeCommands();

		// Configure la r�gion de copie
		vk::BufferImageCopy region{};
		region.bufferOffset      = 0;
		region.bufferRowLength   = 0;
		region.bufferImageHeight = 0;

		region.imageSubresource.aspectMask     = vk::ImageAspectFlagBits::eColor;
		region.imageSubresource.mipLevel       = 0;
		region.imageSubresource.baseArrayLayer = 0;
		region.imageSubresource.layerCount     = _layerCount;

		region.imageOffset = vk::Offset3D{0, 0, 0};
		region.imageExtent = vk::Extent3D{_width, _height, 1};

		// Copie les donn�es du tampon vers l'image Vulkan
		command_buffer.copyBufferToImage(
			_buffer,
			_image,
			vk::ImageLayout::eTransferDstOptimal,
			1,
			&region);

		// Termine l'enregistrement des commandes et soumet le tampon de commandes � la file de commandes graphiques
		EndSingleTimeCommands(command_buffer);
	}


	/**
	 * @brief Cr�e une image Vulkan avec les informations sp�cifi�es.
	 *
	 * Cette fonction cr�e une image Vulkan en utilisant les informations fournies dans
	 * l'objet VkImageCreateInfo. Elle alloue �galement la m�moire n�cessaire pour cette
	 * image et associe cette m�moire � l'image cr��e.
	 *
	 * @param _imageInfo Les informations n�cessaires � la cr�ation de l'image.
	 * @param _properties Les propri�t�s de la m�moire de l'image.
	 * @param _image R�f�rence o� l'objet image cr�� sera stock�.
	 * @param _imageMemory R�f�rence o� la m�moire de l'image sera stock�e.
	 * @throws std::runtime_error en cas d'�chec de la cr�ation de l'image ou de l'allocation de m�moire.
	 */
	void LveDevice::CreateImageWithInfo(
		const vk::ImageCreateInfo&    _imageInfo,
		const vk::MemoryPropertyFlags _properties,
		vk::Image&                    _image,
		vk::DeviceMemory&             _imageMemory) const
	{
		// Cr�e l'image Vulkan
		try
		{
			_image = device_.createImage(_imageInfo);
		}
		catch (const vk::SystemError& e)
		{
			throw std::runtime_error("failed to create image!");
		}

		// R�cup�re les exigences de m�moire de l'image
		const vk::MemoryRequirements mem_requirements = device_.getImageMemoryRequirements(_image);

		// Alloue la m�moire pour l'image
		vk::MemoryAllocateInfo alloc_info{};
		alloc_info.setAllocationSize(mem_requirements.size);
		alloc_info.setMemoryTypeIndex(FindMemoryType(mem_requirements.memoryTypeBits, _properties));

		try
		{
			_imageMemory = device_.allocateMemory(alloc_info);
		}
		catch (const vk::SystemError& e)
		{
			// Lib�re l'image si l'allocation de m�moire �choue
			device_.destroyImage(_image);
			throw std::runtime_error("failed to allocate image memory!");
		}

		// Associe la m�moire allou�e � l'image
		try
		{
			device_.bindImageMemory(_image, _imageMemory, 0);
		}
		catch (const vk::SystemError& e)
		{
			// Lib�re l'image et la m�moire si la liaison �choue
			device_.destroyImage(_image);
			device_.freeMemory(_imageMemory);
			throw std::runtime_error("failed to bind image memory!");
		}
	}
} // namespace lve
