#pragma once

#include <vulkan/vulkan.hpp>
#include "lve_device.h"
#include "lve_buffer.h"

//libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm.hpp>

//std
#include <memory>
#include <vector>
#include <string>

namespace lve {
class LveModel
{

public:

	struct Vertex {
		glm::vec3 position{};
		glm::vec3 color{};
		glm::vec3 normal{};
		glm::vec2 uv{};


		/**
		 * @brief Obtient les descriptions de liaison des entr�es de vertex.
		 *
		 * Cette fonction retourne les descriptions de liaison des entr�es de vertex pour le pipeline de rendu.
		 * Elle cr�e un vecteur de descriptions de liaison avec une seule description, o� le binding est d�fini sur 0,
		 * la stride est d�finie sur la taille d'une structure Vertex, et le taux d'entr�e est d�fini sur VK_VERTEX_INPUT_RATE_VERTEX.
		 *
		 * @return Un vecteur contenant les descriptions de liaison des entr�es de vertex.
		 */
		static std::vector<vk::VertexInputBindingDescription> GetBindingDescriptions();

		/**
		 * @brief Obtient les descriptions d'attribut des entr�es de vertex.
		 *
		 * Cette fonction retourne les descriptions d'attribut des entr�es de vertex pour le pipeline de rendu.
		 * Elle cr�e un vecteur de descriptions d'attribut et y ajoute des descriptions pour chaque attribut de la structure Vertex.
		 * Chaque description contient l'index de liaison, l'emplacement, le format de donn�es et le d�calage de l'attribut correspondant dans la structure Vertex.
		 *
		 * @return Un vecteur contenant les descriptions d'attribut des entr�es de vertex.
		 */
		static std::vector<vk::VertexInputAttributeDescription> GetAttributeDescriptions();

		bool operator==(const Vertex& _other) const {
			return position == _other.position && color == _other.color && normal == _other.normal && uv == _other.uv;
		}
	};

	struct Builder {
		std::vector<Vertex> vertices{};
		std::vector<uint32_t> indices{};

		/**
		 * @brief Charge un mod�le � partir d'un fichier.
		 *
		 * Cette fonction charge un mod�le � partir d'un fichier au format OBJ. Elle utilise la biblioth�que tinyobjloader pour lire le fichier.
		 * Les attributs des vertices tels que la position, la couleur, la normale et les coordonn�es de texture sont extraits du fichier et utilis�s pour cr�er les vertices du mod�le.
		 * Les vertices sont stock�s dans le vecteur 'vertices' de l'objet LveModel::Builder, tandis que les indices des vertices sont stock�s dans le vecteur 'indices'.
		 * Les vertices sont compar�s pour d�tecter les doublons et les indices sont ajust�s en cons�quence pour assurer l'unicit� des vertices.
		 *
		 * @param _filepath Le chemin du fichier du mod�le.
		 * @throws std::runtime_error Si une erreur survient lors du chargement du mod�le.
		 */
		void LoadModel(LveDevice& _device, const std::string& _filepath);
	};

	LveModel(LveDevice& _lveDevice, const LveModel::Builder &_builder);
	~LveModel();

	LveModel(const LveModel&) = delete;
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
	void Bind(vk::CommandBuffer _commandBuffer);

	/**
	 * @brief Dessine les primitives g�om�triques � l'aide du tampon de commande sp�cifi�.
	 *
	 * Cette fonction dessine les primitives g�om�triques � l'aide du tampon de commande sp�cifi�.
	 * Si un tampon d'index est pr�sent, la fonction utilise vkCmdDrawIndexed pour dessiner les primitives index�es. Sinon, elle utilise vkCmdDraw pour dessiner les primitives non index�es.
	 *
	 * @param _commandBuffer Le tampon de commande utilis� pour dessiner les primitives g�om�triques.
	 */
	void Draw(vk::CommandBuffer _commandBuffer);

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

	LveDevice& lveDevice;

	std::unique_ptr<LveBuffer> vertexBuffer;
	uint32_t vertexCount;

	bool hasIndexBuffer = false;
	std::unique_ptr<LveBuffer> indexBuffer;
	uint32_t indexCount;

};


} //lve
