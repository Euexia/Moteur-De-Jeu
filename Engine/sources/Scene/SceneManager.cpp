#include "Scene/SceneManager.h"
#include <fstream>
#include <sstream>

#include "CoreEngine.h"
#include "ModuleManager.h"
#include "Modules/WindowModule.h"


/**
 * @brief Cr�e une nouvelle sc�ne.
 * @param _name Nom de la nouvelle sc�ne.
 * @param _isActive Indique si la nouvelle sc�ne est active.
 */
void SceneManager::CreateScene(std::string _name, bool _isActive)
{
	listScenes.insert(std::make_pair(_name, _isActive));
	scenes.push_back(std::make_unique<BaseScene>(_name));
}

/**
 * @brief R�cup�re le nombre de sc�nes.
 * @return Le nombre de sc�nes.
 */
int SceneManager::SceneCount() const
{
	int count = 0;
	if (!listScenes.empty()) count = listScenes.size();
	return count;
}

/**
 * @brief R�cup�re le nom de la sc�ne active.
 * @return Le nom de la sc�ne active, ou "None" si aucune sc�ne n'est active.
 */
std::string SceneManager::GetActiveScene() const
{
	for (const auto scene : listScenes)
	{
		std::string name = scene.first;
		if (bool is_active = scene.second) return name;
	}
	return "None";
}

/**
 * @brief R�cup�re le nom de la premi�re sc�ne dans la liste des sc�nes.
 * @return Le nom de la premi�re sc�ne, ou "None" si la liste des sc�nes est vide.
 */
std::string SceneManager::GetListScenes() const
{
	for (const auto scene : listScenes)
	{
		std::string name = scene.first;
		return name;
	}
	return "None";
}

/**
 * @brief R�cup�re la sc�ne � un index donn� dans la liste des sc�nes.
 * @param _index Index de la sc�ne � r�cup�rer.
 * @return Une paire contenant le nom de la sc�ne et son �tat d'activit�, ou une paire "None" si l'index est invalide.
 */
std::pair<std::string, bool> SceneManager::GetSceneAt(const int _index)
{
	if (!listScenes.empty())
	{
		auto find_name = listScenes.begin();
		std::advance(find_name, _index);
		return *find_name;
	}
	return {"None", false};
}


/**
 * @brief Supprime une sc�ne de la liste des sc�nes.
 * @param _sceneName Nom de la sc�ne � supprimer.
 */
void SceneManager::DestroyScene(const std::string& _sceneName)
{
	const auto it = listScenes.find(_sceneName);
	if (it != listScenes.end()) listScenes.erase(it);
}

/**
 * @brief D�finit la sc�ne courante par son index dans la liste des sc�nes.
 * @param _sceneIndex Index de la sc�ne � d�finir comme sc�ne courante.
 */
void SceneManager::SetCurrentScene(const int _sceneIndex)
{
	if (_sceneIndex >= 0 && _sceneIndex < static_cast<int>(scenes.size())) {
		currentSceneIndex = _sceneIndex;
		mainScene = scenes[currentSceneIndex].get();
	}
}

/**
 * @brief Active la sc�ne suivante dans la liste des sc�nes.
 * Si la liste des sc�nes n'est pas vide, l'index de la sc�ne courante est mis � jour pour passer � la sc�ne suivante circulairement.
 */
void SceneManager::SetNextSceneActive()
{
	if (!scenes.empty()) currentSceneIndex = (currentSceneIndex + 1) % scenes.size();
}

/**
 * @brief Active la sc�ne pr�c�dente dans la liste des sc�nes.
 * Si la liste des sc�nes n'est pas vide, l'index de la sc�ne courante est mis � jour pour passer � la sc�ne pr�c�dente circulairement.
 */
void SceneManager::SetPreviousSceneActive()
{
	if (!scenes.empty()) currentSceneIndex = (currentSceneIndex - 1 + scenes.size()) % scenes.size();
}


/**
 * @brief R�cup�re la sc�ne courante.
 * @return Un pointeur vers la sc�ne courante si l'index de la sc�ne courante est valide, sinon nullptr.
 */
BaseScene* SceneManager::GetCurrentScene() const
{
	return (currentSceneIndex >= 0 && currentSceneIndex < static_cast<int>(scenes.size()))
		       ? scenes[currentSceneIndex].get()
		       : nullptr;
}

/**
 * @brief V�rifie si un fichier de sc�ne existe dans la liste des sc�nes.
 * @param fileName Nom du fichier de sc�ne � rechercher.
 * @return true si le fichier de sc�ne existe, sinon false.
 */
bool SceneManager::SceneFileExists(const std::string& fileName) const
{
	for (const auto& scene : scenes)
	{
		if (scene->GetFileName() == fileName) return true;
	}

	return false;
}


//--------------------------A MODIFIER TRES SIMPLIFIE------------------------------------
GameObject* SceneManager::CreateGameObjectFromSceneData()
{
	const auto game_object = new GameObject();

	return game_object;
}

/**
 * @brief Charge une sc�ne � partir d'un fichier.
 * @param _fileName Nom du fichier de sc�ne � charger.
 * @return true si la sc�ne a �t� charg�e avec succ�s, sinon false.
 */
bool SceneManager::LoadSceneFromFile(const std::string& _fileName)
{
	// V�rifier si le fichier existe
	if (!SceneFileExists(_fileName))
	{
		std::cerr << "Erreur : Le fichier de sc�ne '" << _fileName << "' n'existe pas." << std::endl;
		return false;
	}

	// Ouvrir le fichier de sc�ne en lecture
	std::ifstream file(_fileName);
	if (!file.is_open())
	{
		std::cerr << "Erreur : Impossible d'ouvrir le fichier de sc�ne '" << _fileName << "'." << std::endl;
		return false;
	}

	// Lire le contenu du fichier de sc�ne
	std::stringstream buffer;
	buffer << file.rdbuf();
	std::string scene_data = buffer.str();

	// Fermer le fichier
	file.close();

	// Analyser les donn�es de la sc�ne
	std::istringstream scene_stream(scene_data);
	std::string        scene_object_name;
	while (scene_stream >> scene_object_name)
	{
		if (GameObject* game_object = CreateGameObjectFromSceneData())
			// Ajouter l'objet de sc�ne � la sc�ne courante
			GetCurrentScene()->AddRootObject(game_object);
		else std::cerr << "Erreur : Impossible de cr�er l'objet de sc�ne '" << scene_object_name << "'." << std::endl;
	}

	std::cout << "Chargement de la sc�ne depuis le fichier '" << _fileName << "' termin�." << std::endl;
	return true;
}

/**
 * @brief Sauvegarde une sc�ne dans un fichier.
 * @param _fileName Nom du fichier de sc�ne � sauvegarder.
 * @return true si la sc�ne a �t� sauvegard�e avec succ�s, sinon false.
 */
void SceneManager::RunScene(const std::string& _sceneName)
{
	if(listScenes.contains(_sceneName))
	{
		if(sceneActive)
		{
			GetCurrentScene()->Release();
			GetCurrentScene()->Finalize();
		}
		SetMainScene(_sceneName);
		GetCurrentScene()->Init();
		GetCurrentScene()->Start();
	}
}


/**
 * @brief D�finit la sc�ne principale � utiliser.
 * @param _sceneName Nom de la sc�ne � d�finir comme sc�ne principale.
 */
void SceneManager::SetMainScene(const std::string& _sceneName)
{
	if (const auto it = listScenes.find(_sceneName);
		it != listScenes.end()) currentSceneIndex = std::distance(listScenes.begin(), it);
	for (const auto& scene : scenes)
	{
		if(scene->name == _sceneName)
		{
			mainScene = scene.get();
			break;
		}
	}
}


/**
 * @brief R�cup�re une sc�ne par son nom.
 * @param _sceneName Nom de la sc�ne � r�cup�rer.
 * @return Un pointeur vers la sc�ne si elle existe, sinon nullptr.
 */
BaseScene* SceneManager::GetScene(const std::string& _sceneName)
{
	if (const auto it = listScenes.find(_sceneName); it != listScenes.end()) return scenes[std::distance(
		listScenes.begin(), it)].get();
	return nullptr;
}

/**
 * @brief Renomme une sc�ne dans la liste des sc�nes.
 * @param _oldName Ancien nom de la sc�ne � renommer.
 * @param _newName Nouveau nom de la sc�ne.
 */
void SceneManager::RenameScene(const std::string& _oldName, const std::string& _newName)
{
	if (const auto it = listScenes.find(_oldName); it != listScenes.end())
	{
		listScenes[_newName] = it->second;
		listScenes.erase(it);
	}
}

void SceneManager::Init()
{
	Module::Init();
	CreateScene("SampleScene", true);
	SetMainScene("SampleScene");

}

/**
 * @brief D�marre le gestionnaire de sc�nes.
 * Initialise le gestionnaire de fen�tres.
 */
void SceneManager::Start()
{
	Module::Start();
	RunScene("SampleScene");
	windowModule = moduleManager->GetModule<WindowModule>();
}

void SceneManager::FixedUpdate()
{
	Module::FixedUpdate();
	if (mainScene) mainScene->FixedUpdate();
}

void SceneManager::Update()
{
	Module::Update();
	if(mainScene)
	{
		if(Engine::GetInstance()->GetEngineMode() == EngineMode::Editor) mainScene->UpdateEditor();
		else if(Engine::GetInstance()->GetEngineMode() == EngineMode::Play) mainScene->Update();
	}
}

void SceneManager::PreRender()
{
	Module::PreRender();
	if (mainScene) mainScene->PreRender();
}

void SceneManager::Render()
{
	Module::Render();
	if (mainScene) mainScene->Render();
}

void SceneManager::RenderGui()
{
	Module::RenderGui();
	if (mainScene) mainScene->RenderGui();
}

void SceneManager::PostRender()
{
	Module::PostRender();
	if (mainScene) mainScene->PostRender();
}

/**
 * @brief D�truit toutes les sc�nes et vide la liste des sc�nes.
 * L'index de la sc�ne courante est r�initialis� � -1.
 */
void SceneManager::Release()
{
	if (mainScene) mainScene->Release();
	if (mainScene) mainScene->Finalize();
	if (currentSceneIndex >= 0 && currentSceneIndex < static_cast<int>(scenes.size()))
		scenes[currentSceneIndex]->
		Finalize();

	scenes.clear();
	currentSceneIndex = -1;
}

void SceneManager::Finalize()
{
	Module::Finalize();
}
