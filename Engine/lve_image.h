#pragma once

#include "lve_buffer.h"
#include "lve_device.h"

#include <cassert>
#include <iostream>
#include <stdexcept>
#include <unordered_map>

namespace lve
{
	class LveImage
	{
		public:
			/**
			 * @brief Constructeur de la classe LveImage.
			 *
			 * @param _lveDevice R�f�rence vers l'objet LveDevice associ� � cette image.
			 */
			explicit LveImage(LveDevice& _lveDevice);

			/** @brief Destructeur de la classe LveImage. */
			~LveImage();

			/**
			 * @brief Op�rateur de copie supprim� pour la classe LveImage.
			 *
			 * @return L'objet LveImage r�sultant de l'op�ration.
			 */
			LveImage(const LveImage&) = delete;

			/**
			 * @brief Op�rateur d'affectation supprim� pour la classe LveImage.
			 *
			 * @return L'objet LveImage r�sultant de l'op�ration.
			 */
			LveImage operator=(const LveImage&) = delete;

		private:
			/**
			 * @brief Cr�e une image de texture.
			 */
			void CreateTextureImage();

			/**
			 * @brief Cr�e une image Vulkan avec les param�tres sp�cifi�s.
			 *
			 * @param _width La largeur de l'image.
			 * @param _height La hauteur de l'image.
			 * @param _format Le format de l'image.
			 * @param _tiling Le mode de pavage de l'image.
			 * @param _usage Les indicateurs d'utilisation de l'image.
			 * @param _properties Les propri�t�s de m�moire de l'image.
			 * @param _image R�f�rence o� stocker l'image Vulkan cr��e.
			 * @param _imageMemory R�f�rence o� stocker la m�moire allou�e pour l'image.
			 */
			void CreateImage(uint32_t                _width,
			                 uint32_t                _height,
			                 vk::Format              _format,
			                 vk::ImageTiling         _tiling,
			                 vk::ImageUsageFlags     _usage,
			                 vk::MemoryPropertyFlags _properties,
			                 vk::Image&              _image,
			                 vk::DeviceMemory&       _imageMemory) const;

			/** R�f�rence vers l'objet LveDevice associ�. */
			LveDevice& lveDevice;

			/** Image de texture Vulkan cr��e. */
			vk::Image textureImage;

			/** M�moire allou�e pour l'image de texture. */
			vk::DeviceMemory textureImageMemory;
	};
}
