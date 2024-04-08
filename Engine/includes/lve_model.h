#pragma once

#include <vulkan/vulkan.hpp>
#include "lve_buffer.h"
#include "lve_device.h"

//libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm.hpp>

//std
#include <memory>
#include <string>
#include <vector>

namespace lve
{
	class LveModel
	{
		public:
			/**
			 * @brief Structure repr�sentant un vertex dans l'espace 3D.
			 */
			struct Vertex
			{
				glm::vec3 position{}; /**< Position du vertex dans l'espace. */
				glm::vec3 color{};    /**< Couleur du vertex. */
				glm::vec3 normal{};   /**< Normale du vertex. */
				glm::vec2 uv{};       /**< Coordonn�es de texture du vertex. */

				/**
				 * @brief Obtient les descriptions de liaison des entr�es de vertex.
				 *
				 * @return Un vecteur contenant les descriptions de liaison des entr�es de vertex.
				 */
				static std::vector<vk::VertexInputBindingDescription> GetBindingDescriptions();

				/**
				 * @brief Obtient les descriptions d'attribut des entr�es de vertex.
				 *
				 * @return Un vecteur contenant les descriptions d'attribut des entr�es de vertex.
				 */
				static std::vector<vk::VertexInputAttributeDescription> GetAttributeDescriptions();

				/**
				 * @brief Op�rateur de comparaison d'�galit�.
				 *
				 * @param _other L'autre vertex � comparer.
				 * @return true si les deux vertices sont �gaux, sinon false.
				 */
				bool operator==(const Vertex& _other) const
				{
					return position == _other.position && color == _other.color && normal == _other.normal && uv ==
					       _other.uv;
				}
			};


			/**
			 * @brief Structure responsable de la construction d'un mod�le 3D.
			 */
			struct Builder
			{
				std::vector<Vertex>   vertices{}; /**< Vecteur contenant les vertices du mod�le. */
				std::vector<uint32_t> indices{};  /**< Vecteur contenant les indices des vertices. */

				/**
				 * @brief Charge un mod�le � partir d'un fichier au format OBJ.
				 *
				 * Cette fonction charge un mod�le � partir d'un fichier au format OBJ.
				 * Elle utilise la biblioth�que tinyobjloader pour lire le fichier et extraire les attributs des vertices tels que la position, la couleur, la normale et les coordonn�es de texture.
				 * Les vertices sont stock�s dans le vecteur 'vertices' de l'objet LveModel::Builder, tandis que les indices des vertices sont stock�s dans le vecteur 'indices'.
				 * Les vertices sont compar�s pour d�tecter les doublons et les indices sont ajust�s en cons�quence pour assurer l'unicit� des vertices.
				 *
				 * @param _filepath Le chemin du fichier du mod�le.
				 * @throws std::runtime_error Si une erreur survient lors du chargement du mod�le.
				 */
				void LoadModel(const std::string& _filepath);
			};


			/**
			 * @brief Constructeur de la classe LveModel.
			 *
			 * Construit un mod�le 3D � partir des donn�es fournies par le builder.
			 *
			 * @param _lveDevice R�f�rence vers l'objet LveDevice utilis� pour la cr�ation du mod�le.
			 * @param _builder R�f�rence vers l'objet Builder contenant les donn�es du mod�le.
			 */
			LveModel(LveDevice& _lveDevice, const Builder& _builder);

			/**
			 * @brief Destructeur de la classe LveModel.
			 */
			~LveModel();

			/**
			 * @brief Constructeur de copie supprim�.
			 *
			 * Le constructeur de copie de la classe LveModel est supprim� pour emp�cher la duplication d'instances de mod�le.
			 */
			LveModel(const LveModel&) = delete;

			/**
			 * @brief Op�rateur d'affectation supprim�.
			 *
			 * L'op�rateur d'affectation de la classe LveModel est supprim� pour emp�cher l'affectation d'une instance de mod�le � une autre.
			 */
			LveModel operator=(const LveModel&) = delete;


			/**
			 * @brief Cr�e un mod�le � partir d'un fichier.
			 *
			 * Cette fonction statique cr�e un mod�le � partir d'un fichier en utilisant un objet LveDevice et un chemin de fichier donn�s.
			 * Elle charge d'abord le mod�le � l'aide du constructeur Builder, puis affiche le nombre de vertices charg�s.
			 * Enfin, elle cr�e un mod�le unique � partir de l'objet LveDevice et du constructeur Builder, puis le retourne.
			 *
			 * @param _device L'objet LveDevice utilis� pour cr�er le mod�le.
			 * @param _filepath Le chemin du fichier � partir duquel charger le mod�le.
			 * @return Un pointeur unique vers le mod�le cr��.
			 */
			static std::unique_ptr<LveModel> CreateModelFromFile(LveDevice& _device, const std::string& _filepath);

			/**
			 * @brief Lie les tampons de vertex et d'index au tampon de commande sp�cifi�.
			 *
			 * Cette fonction lie les tampons de vertex et d'index au tampon de commande sp�cifi� pour le rendu ult�rieur.
			 * Si un tampon d'index est pr�sent, il est �galement li� au tampon de commande avec le type d'index sp�cifi�.
			 *
			 * @param _commandBuffer Le tampon de commande auquel lier les tampons de vertex et d'index.
			 */
			void Bind(vk::CommandBuffer _commandBuffer) const;

			/**
			 * @brief Dessine les primitives g�om�triques � l'aide du tampon de commande sp�cifi�.
			 *
			 * Cette fonction dessine les primitives g�om�triques � l'aide du tampon de commande sp�cifi�.
			 * Si un tampon d'index est pr�sent, la fonction utilise vkCmdDrawIndexed pour dessiner les primitives index�es. Sinon, elle utilise vkCmdDraw pour dessiner les primitives non index�es.
			 *
			 * @param _commandBuffer Le tampon de commande utilis� pour dessiner les primitives g�om�triques.
			 */
			void Draw(vk::CommandBuffer _commandBuffer) const;

		private:
			/**
			 * @brief Cr�e le tampon de vertex.
			 *
			 * Cette fonction cr�e le tampon de vertex � partir des donn�es de vertex fournies.
			 * Elle calcule la taille du tampon en fonction du nombre de vertices et de leur taille individuelle, puis cr�e un tampon de transfert de m�moire visible par l'h�te pour copier les donn�es de vertex.
			 * Les donn�es de vertex sont ensuite copi�es dans le tampon de transfert, puis transf�r�es vers un tampon de m�moire local du p�riph�rique pour une utilisation efficace pendant le rendu.
			 *
			 * @param _vertices Les donn�es de vertex � utiliser pour cr�er le tampon.
			 */
			void CreateVertexBuffer(const std::vector<Vertex>& _vertices);


			/**
			 * @brief Cr�e le tampon d'index.
			 *
			 * Cette fonction cr�e le tampon d'index � partir des indices fournis.
			 * Elle d�termine d'abord si des indices sont pr�sents. Si aucun indice n'est pr�sent, la fonction se termine.
			 * Sinon, elle calcule la taille du tampon en fonction du nombre d'indices et de leur taille individuelle, puis cr�e un tampon de transfert de m�moire visible par l'h�te pour copier les donn�es d'indices.
			 * Les donn�es d'indices sont ensuite copi�es dans le tampon de transfert, puis transf�r�es vers un tampon de m�moire local du p�riph�rique pour une utilisation efficace pendant le rendu.
			 *
			 * @param _indices Les indices � utiliser pour cr�er le tampon.
			 */
			void CreateIndexBuffer(const std::vector<uint32_t>& _indices);

			LveDevice& lveDevice; /**< R�f�rence vers l'objet LveDevice utilis� pour la cr�ation du mod�le. */

			std::unique_ptr<LveBuffer> vertexBuffer; /**< Pointeur unique vers le tampon de vertex du mod�le. */
			uint32_t                   vertexCount;  /**< Nombre de vertices dans le tampon de vertex. */

			bool                       hasIndexBuffer = false; /**< Indique si le mod�le poss�de un tampon d'indices. */
			std::unique_ptr<LveBuffer> indexBuffer; /**< Pointeur unique vers le tampon d'indices du mod�le. */
			uint32_t                   indexCount; /**< Nombre d'indices dans le tampon d'indices. */
	};
} //lve
