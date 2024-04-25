#pragma once
#include <fstream>

/**
 * @brief Classe utilitaire pour la gestion des fichiers.
 */
class FileManager
{
	public:
		/**
		 * @brief V�rifie si un fichier existe.
		 *
		 * @param _filePath Chemin du fichier � v�rifier.
		 * @return true si le fichier existe, sinon false.
		 */
		static bool FileExists(const std::string& _filePath)
		{
			const std::ifstream file(_filePath);
			return file.good();
		}
};

