#pragma once

#include "LveEngine/lve_device.h"

#include <vulkan/vulkan.hpp>

// std
#include <memory>
#include <unordered_map>
#include <vector>

namespace lve
{
	/**
	 * @brief Classe repr�sentant un agencement de jeu de descripteurs Vulkan.
	 */
	class LveDescriptorSetLayout
	{
		public:
			class Builder
			{
				public:
					/**
					 * @brief Constructeur de la classe Builder.
					 *
					 * @param _lveDevice Une r�f�rence vers l'objet LveDevice utilis� pour la cr�ation de la mise en page de l'ensemble de descripteurs.
					 */
					explicit Builder(LveDevice& _lveDevice) : lveDevice{_lveDevice}
					{
					}

					/**
					 * @brief Ajoute un binding � la mise en page de l'ensemble de descripteurs.
					 *
					 * @param _binding L'index du binding.
					 * @param _descriptorType Le type de descripteur.
					 * @param _stageFlags Les indicateurs d'�tape de shader.
					 * @param _count Le nombre d'�l�ments dans le binding.
					 * @return Une r�f�rence vers l'objet Builder actuel.
					 */
					Builder& AddBinding(uint32_t             _binding, vk::DescriptorType _descriptorType,
					                    vk::ShaderStageFlags _stageFlags, uint32_t        _count = 1);

					/**
					 * @brief Construit et retourne un ensemble de descripteurs avec la mise en page sp�cifi�e.
					 *
					 * @return Un pointeur unique vers l'objet LveDescriptorSetLayout nouvellement cr��.
					 */
					std::unique_ptr<LveDescriptorSetLayout> Build() const;

				private:
					LveDevice& lveDevice;
					/**< Une r�f�rence vers l'objet LveDevice utilis� pour la cr�ation de la mise en page de l'ensemble de descripteurs. */
					std::unordered_map<uint32_t, vk::DescriptorSetLayoutBinding> bindings{};
					/**< Les bindings ajout�s � la mise en page de l'ensemble de descripteurs. */
			};


			/**
			 * @brief Constructeur de la classe LveDescriptorSetLayout.
			 *
			 * @param _lveDevice Une r�f�rence vers l'objet LveDevice utilis� pour la cr�ation de l'ensemble de descripteurs.
			 * @param _bindings Les bindings de la mise en page de l'ensemble de descripteurs.
			 */
			LveDescriptorSetLayout(LveDevice&                                                   _lveDevice,
			                       std::unordered_map<uint32_t, vk::DescriptorSetLayoutBinding> _bindings);

			/**
			 * @brief Destructeur de la classe LveDescriptorSetLayout.
			 */
			~LveDescriptorSetLayout();

			/**
			 * @brief Constructeur de copie supprim�.
			 */
			LveDescriptorSetLayout(const LveDescriptorSetLayout&) = delete;

			/**
			 * @brief Op�rateur d'affectation supprim�.
			 */
			LveDescriptorSetLayout& operator=(const LveDescriptorSetLayout&) = delete;


			/**
			 * @brief Obtient la mise en page de l'ensemble de descripteurs.
			 *
			 * @return La mise en page de l'ensemble de descripteurs.
			 */
			vk::DescriptorSetLayout GetDescriptorSetLayout() const { return descriptorSetLayout; }

		private:
			LveDevice& lveDevice; /**< R�f�rence � l'objet LveDevice associ�. */
			vk::DescriptorSetLayout descriptorSetLayout; /**< Mise en page de l'ensemble de descripteurs Vulkan. */
			std::unordered_map<uint32_t, vk::DescriptorSetLayoutBinding> bindings;
			/**< Map des bindings de l'ensemble de descripteurs. */

			friend class LveDescriptorWriter; /**< D�claration d'amiti� avec la classe LveDescriptorWriter. */
	};

	class LveDescriptorPool
	{
		public:
			class Builder
			{
				public:
					/**
		 * @brief Constructeur de la classe Builder.
		 *
		 * Initialise un nouvel objet Builder avec une r�f�rence � l'objet LveDevice sp�cifi�.
		 *
		 * @param _lveDevice R�f�rence � l'objet LveDevice associ� � ce builder.
		 */
					explicit Builder(LveDevice& _lveDevice) : lveDevice{_lveDevice}
					{
					}

					/**
					 * @brief Ajoute une taille de pool de descripteurs au builder.
					 *
					 * Cette fonction ajoute une taille de pool de descripteurs au builder.
					 * Elle sp�cifie le type de descripteur et le nombre de descripteurs � allouer dans le pool.
					 *
					 * @param _descriptorType Le type de descripteur � ajouter.
					 * @param _count Le nombre de descripteurs � allouer dans le pool pour le type sp�cifi�.
					 * @return Une r�f�rence � cet objet Builder pour permettre les appels de m�thode en cha�ne.
					 */
					Builder& AddPoolSize(vk::DescriptorType _descriptorType, uint32_t _count);

					/**
					 * @brief D�finit les indicateurs de cr�ation du pool de descripteurs.
					 *
					 * Cette fonction d�finit les indicateurs de cr�ation du pool de descripteurs.
					 * Elle sp�cifie les options de cr�ation pour le pool de descripteurs.
					 *
					 * @param _flags Les indicateurs de cr�ation du pool de descripteurs.
					 * @return Une r�f�rence � cet objet Builder pour permettre les appels de m�thode en cha�ne.
					 */
					Builder& SetPoolFlags(vk::DescriptorPoolCreateFlags _flags);

					/**
					 * @brief D�finit le nombre maximal d'ensembles de descripteurs pouvant �tre allou�s � partir du pool.
					 *
					 * Cette fonction d�finit le nombre maximal d'ensembles de descripteurs pouvant �tre allou�s � partir du pool.
					 * Elle sp�cifie le nombre maximal d'ensembles de descripteurs qui peuvent �tre allou�s � partir du pool de descripteurs.
					 *
					 * @param _count Le nombre maximal d'ensembles de descripteurs pouvant �tre allou�s � partir du pool.
					 * @return Une r�f�rence � cet objet Builder pour permettre les appels de m�thode en cha�ne.
					 */
					Builder& SetMaxSets(uint32_t _count);

					/**
					 * @brief Construit et retourne un objet LveDescriptorPool.
					 *
					 * Cette fonction construit et retourne un objet LveDescriptorPool avec les configurations sp�cifi�es pr�c�demment.
					 *
					 * @return Un unique_ptr pointant vers l'objet LveDescriptorPool nouvellement cr��.
					 */
					[[nodiscard]] std::unique_ptr<LveDescriptorPool> Build() const;

				private:
					LveDevice& lveDevice; /**< R�f�rence � l'objet LveDevice associ� � ce pool de descripteurs. */
					uint32_t   maxSets;
					/**< Nombre maximal d'ensembles de descripteurs pouvant �tre allou�s � partir du pool. */
					vk::DescriptorPoolCreateFlags poolFlags; /**< Indicateurs de cr�ation du pool de descripteurs. */
					std::vector<vk::DescriptorPoolSize> poolSizes;
					/**< Vecteur contenant les tailles de pool de descripteurs. */
			};

			/**
			 * @brief Constructeur de LveDescriptorPool.
			 *
			 * @param _lveDevice R�f�rence � l'objet LveDevice associ� � ce pool de descripteurs.
			 * @param _maxSets Nombre maximal d'ensembles de descripteurs pouvant �tre allou�s � partir du pool.
			 * @param _poolFlags Indicateurs de cr�ation du pool de descripteurs.
			 * @param _poolSizes Vecteur contenant les tailles de pool de descripteurs.
			 */
			LveDescriptorPool(
				LveDevice&                                 _lveDevice,
				uint32_t                                   _maxSets,
				vk::DescriptorPoolCreateFlags              _poolFlags,
				const std::vector<vk::DescriptorPoolSize>& _poolSizes);

			/**
			 * @brief Destructeur de LveDescriptorPool.
			 */
			~LveDescriptorPool();

			LveDescriptorPool(const LveDescriptorPool&)            = delete; /**< Constructeur de copie supprim�. */
			LveDescriptorPool& operator=(const LveDescriptorPool&) = delete; /**< Op�rateur d'affectation supprim�. */


			/**
			 * @brief Alloue un ensemble de descripteurs � partir du pool.
			 *
			 * @param _descriptorSetLayout Layout de l'ensemble de descripteurs � allouer.
			 * @param _descriptor R�f�rence o� stocker l'ensemble de descripteurs allou�.
			 * @return true si l'allocation r�ussit, false sinon.
			 */
			bool AllocateDescriptor(
				vk::DescriptorSetLayout _descriptorSetLayout, vk::DescriptorSet& _descriptor) const;

			/**
			 * @brief Lib�re un ensemble de descripteurs.
			 *
			 * @param _descriptors Vecteur contenant les ensembles de descripteurs � lib�rer.
			 */
			void FreeDescriptors(const std::vector<vk::DescriptorSet>& _descriptors) const;

			/**
			 * @brief R�initialise le pool de descripteurs.
			 */
			void ResetPool() const;

			vk::DescriptorPool GetDescriptorPool() { return descriptorPool; }

		private:
			LveDevice&         lveDevice;      /**< R�f�rence vers l'appareil logique associ�. */
			vk::DescriptorPool descriptorPool; /**< Pool de descripteurs Vulkan. */
			friend class LveDescriptorWriter;
			/**< Permet � la classe LveDescriptorWriter d'acc�der aux membres priv�s. */
	};

	class LveDescriptorWriter
	{
		public:
			/**
			 * @brief Constructeur de LveDescriptorWriter.
			 * @param _setLayout R�f�rence vers l'ensemble de descripteurs.
			 * @param _pool R�f�rence vers le pool de descripteurs.
			 */
			LveDescriptorWriter(LveDescriptorSetLayout& _setLayout, LveDescriptorPool& _pool);

			/**
			 * @brief �criture d'informations de tampon dans le descripteur.
			 * @param _binding Indice de liaison du descripteur.
			 * @param _bufferInfo Pointeur vers les informations du tampon.
			 * @return Une r�f�rence vers l'objet LveDescriptorWriter.
			 */
			LveDescriptorWriter& WriteBuffer(uint32_t _binding, const vk::DescriptorBufferInfo* _bufferInfo);

			/**
			 * @brief �criture d'informations d'image dans le descripteur.
			 * @param _binding Indice de liaison du descripteur.
			 * @param _imageInfo Pointeur vers les informations de l'image.
			 * @return Une r�f�rence vers l'objet LveDescriptorWriter.
			 */
			LveDescriptorWriter& WriteImage(uint32_t _binding, const vk::DescriptorImageInfo* _imageInfo);


			/**
			  * @brief Construit un ensemble de descripteurs � partir des �critures pr�c�dentes.
			  * @param _set R�f�rence vers l'ensemble de descripteurs � construire.
			  * @return Bool�en indiquant si la construction a r�ussi.
			  */
			bool Build(vk::DescriptorSet& _set);

			/**
			 * @brief �crit � nouveau les informations de descripteur dans un ensemble de descripteurs existant.
			 * @param _set R�f�rence vers l'ensemble de descripteurs � modifier.
			 */
			void Overwrite(const vk::DescriptorSet& _set);

		private:
			/**
			 * @brief R�f�rence vers l'ensemble de descripteurs utilis� pour d�finir la mise en page des descripteurs.
			 */
			LveDescriptorSetLayout& setLayout;

			/**
			 * @brief R�f�rence vers le pool de descripteurs utilis� pour l'allocation des ensembles de descripteurs.
			 */
			LveDescriptorPool& pool;

			/**
			 * @brief Vecteur contenant les op�rations d'�criture de descripteurs � appliquer lors de la construction des ensembles de descripteurs.
			 */
			std::vector<vk::WriteDescriptorSet> writes;
	};
} // namespace lve
