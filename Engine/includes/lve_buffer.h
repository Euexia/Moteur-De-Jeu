#pragma once

#include <vulkan/vulkan.hpp>
#include "lve_device.h"

namespace lve
{
	class LveBuffer
	{
		public:
			LveBuffer(
				LveDevice&              _device,
				vk::DeviceSize          _instanceSize,
				uint32_t                _instanceCount,
				vk::BufferUsageFlags    _usageFlags,
				vk::MemoryPropertyFlags _memoryPropertyFlags,
				vk::DeviceSize          _minOffsetAlignment = 1); /**< Constructeur de LveBuffer. */

			~LveBuffer(); /**< Destructeur de LveBuffer. */

			LveBuffer(const LveBuffer&)            = delete; /**< Constructeur de copie supprim�. */
			LveBuffer& operator=(const LveBuffer&) = delete; /**< Op�rateur d'affectation supprim�. */


			/**
			 * @brief Mappe le tampon en m�moire.
			 *
			 * @param _size La taille du tampon � mapper. Par d�faut, toute la taille du tampon est mapp�e.
			 * @param _offset Le d�calage � partir du d�but du tampon pour commencer le mappage. Par d�faut, le mappage commence au d�but du tampon.
			 *
			 * @return vk::Result indiquant le succ�s ou l'�chec de l'op�ration de mappage.
			 */
			vk::Result Map(vk::DeviceSize _size = VK_WHOLE_SIZE, vk::DeviceSize _offset = 0);

			/**
			 * @brief D�salloue le tampon mapp� en m�moire.
			 */
			void Unmap();

			/**
			 * @brief �crit les donn�es dans le tampon.
			 *
			 * @param _data Pointeur vers les donn�es � �crire dans le tampon.
			 * @param _size La taille des donn�es � �crire dans le tampon. Par d�faut, toute la taille des donn�es est �crite.
			 * @param _offset Le d�calage � partir du d�but du tampon o� �crire les donn�es. Par d�faut, les donn�es sont �crites � partir du d�but du tampon.
			 */
			void WriteToBuffer(const void*    _data, vk::DeviceSize _size = VK_WHOLE_SIZE,
			                   vk::DeviceSize _offset                     = 0) const;

			/**
			 * @brief Flushes the memory-mapped buffer range.
			 *
			 * @param _size La taille de la plage du tampon � flusher. Par d�faut, toute la taille du tampon est flush�e.
			 * @param _offset Le d�calage � partir du d�but du tampon pour commencer le flush. Par d�faut, le flush commence au d�but du tampon.
			 *
			 * @return vk::Result indiquant le succ�s ou l'�chec de l'op�ration de flush.
			 */
			vk::Result Flush(vk::DeviceSize _size = VK_WHOLE_SIZE, vk::DeviceSize _offset = 0) const;

			/**
			 * @brief R�cup�re les informations de tampon de descripteur.
			 *
			 * @param _size La taille des donn�es � inclure dans les informations de descripteur. Par d�faut, toute la taille des donn�es est incluse.
			 * @param _offset Le d�calage � partir du d�but du tampon pour inclure dans les informations de descripteur. Par d�faut, les informations de descripteur incluent le d�but du tampon.
			 *
			 * @return vk::DescriptorBufferInfo contenant les informations de tampon de descripteur.
			 */
			vk::DescriptorBufferInfo DescriptorInfo(vk::DeviceSize _size   = VK_WHOLE_SIZE,
			                                        vk::DeviceSize _offset = 0) const;

			/**
			 * @brief Invalidate the memory-mapped buffer range.
			 *
			 * @param _size La taille de la plage du tampon � invalider. Par d�faut, toute la taille du tampon est invalid�e.
			 * @param _offset Le d�calage � partir du d�but du tampon pour commencer l'invalidation. Par d�faut, l'invalidation commence au d�but du tampon.
			 *
			 * @return vk::Result indiquant le succ�s ou l'�chec de l'op�ration d'invalidation.
			 */
			vk::Result Invalidate(vk::DeviceSize _size = VK_WHOLE_SIZE, vk::DeviceSize _offset = 0) const;

			/**
			 * @brief �crit les donn�es dans l'index sp�cifi� du tampon.
			 *
			 * @param _data Pointeur vers les donn�es � �crire dans le tampon.
			 * @param _index L'index dans le tampon o� �crire les donn�es.
			 */
			void WriteToIndex(void* _data, int _index);

			/**
			 * @brief Flushes the memory-mapped buffer range for the specified index.
			 *
			 * @param _index L'index du tampon pour lequel effectuer le flush.
			 *
			 * @return vk::Result indiquant le succ�s ou l'�chec de l'op�ration de flush.
			 */
			vk::Result FlushIndex(int _index);

			/**
			 * @brief R�cup�re les informations de tampon de descripteur pour l'index sp�cifi�.
			 *
			 * @param _index L'index du tampon pour lequel r�cup�rer les informations de descripteur.
			 *
			 * @return vk::DescriptorBufferInfo contenant les informations de tampon de descripteur pour l'index sp�cifi�.
			 */
			vk::DescriptorBufferInfo DescriptorInfoForIndex(int _index);

			/**
			 * @brief Invalide la plage du tampon pour l'index sp�cifi�.
			 *
			 * @param _index L'index du tampon pour lequel effectuer l'invalidation.
			 *
			 * @return vk::Result indiquant le succ�s ou l'�chec de l'op�ration d'invalidation.
			 */
			vk::Result InvalidateIndex(int _index);

			/**
			 * @brief R�cup�re le tampon Vulkan associ� � l'objet LveBuffer.
			 *
			 * @return vk::Buffer contenant le tampon Vulkan.
			 */
			vk::Buffer GetBuffer() const { return buffer; }

			/**
			 * @brief R�cup�re le pointeur vers la m�moire mapp�e du tampon.
			 *
			 * @return Pointeur vers la m�moire mapp�e du tampon.
			 */
			void* GetMappedMemory() const { return mapped; }

			/**
			 * @brief R�cup�re le nombre d'instances dans le tampon.
			 *
			 * @return Le nombre d'instances dans le tampon.
			 */
			uint32_t GetInstanceCount() const { return instanceCount; }

			/**
			 * @brief R�cup�re la taille de chaque instance dans le tampon.
			 *
			 * @return La taille de chaque instance dans le tampon.
			 */
			vk::DeviceSize GetInstanceSize() const { return instanceSize; }

			/**
			 * @brief R�cup�re la taille d'alignement du tampon.
			 *
			 * @return La taille d'alignement du tampon.
			 */
			vk::DeviceSize GetAlignmentSize() const { return instanceSize; }

			/**
			 * @brief R�cup�re les indicateurs d'utilisation du tampon.
			 *
			 * @return vk::BufferUsageFlags contenant les indicateurs d'utilisation du tampon.
			 */
			vk::BufferUsageFlags GetUsageFlags() const { return usageFlags; }

			/**
			 * @brief R�cup�re les propri�t�s de la m�moire du tampon.
			 *
			 * @return vk::MemoryPropertyFlags contenant les propri�t�s de la m�moire du tampon.
			 */
			vk::MemoryPropertyFlags GetMemoryPropertyFlags() const { return memoryPropertyFlags; }

			/**
			 * @brief R�cup�re la taille totale du tampon.
			 *
			 * @return La taille totale du tampon.
			 */
			vk::DeviceSize GetBufferSize() const { return bufferSize; }

		private:
			/**
			 * @brief Obtient la taille d'alignement requise pour une taille d'instance sp�cifi�e.
			 *
			 * @param _instanceSize La taille de l'instance.
			 * @param _minOffsetAlignment La taille d'alignement minimale.
			 * @return La taille d'alignement requise.
			 */
			static vk::DeviceSize GetAlignment(vk::DeviceSize _instanceSize, vk::DeviceSize _minOffsetAlignment);

			/** @brief R�f�rence vers l'objet LveDevice associ� � ce tampon. */
			LveDevice& lveDevice;

			/** @brief Pointeur vers la m�moire mapp�e du tampon. */
			void* mapped = nullptr;

			/** @brief Le tampon Vulkan. */
			vk::Buffer buffer = VK_NULL_HANDLE;

			/** @brief La m�moire du p�riph�rique associ�e au tampon. */
			vk::DeviceMemory memory = VK_NULL_HANDLE;

			/** @brief La taille du tampon. */
			vk::DeviceSize bufferSize;

			/** @brief Le nombre d'instances dans le tampon. */
			uint32_t instanceCount;

			/** @brief La taille d'une instance dans le tampon. */
			vk::DeviceSize instanceSize;

			/** @brief La taille d'alignement pour le tampon. */
			vk::DeviceSize alignmentSize;

			/** @brief Les indicateurs d'utilisation du tampon. */
			vk::BufferUsageFlags usageFlags;

			/** @brief Les indicateurs de propri�t�s de m�moire du tampon. */
			vk::MemoryPropertyFlags memoryPropertyFlags;
	};
} // namespace lve
