#include "GameObject/GameObjectSerializer.h"
#include <iostream>

using json = nlohmann::json;

void GameObjectSerializer::SerializeComponent(const Component* component, json& componentJson)
{
    // Ajoutez les donn�es sp�cifiques de chaque composant � l'objet JSON du composant
    componentJson["name"] = component->GetName();
    componentJson["isActive"] = component->GetActive();
    componentJson["isVisible"] = component->GetVisible();
    // Vous pouvez ajouter d'autres donn�es sp�cifiques de chaque composant ici
}

void GameObjectSerializer::SerializeChildren(const std::vector<GameObject*>& children, json& parentJson)
{
    for (const GameObject* child : children)
    {
        json childJson; // Cr�e un objet JSON pour chaque enfant

        // Ajoute les donn�es du child GameObject � l'objet JSON de l'enfant
        childJson["name"] = child->GetName();
        childJson["position"] = { child->GetPosition().x, child->GetPosition().y, child->GetPosition().z };
        childJson["scale"] = { child->GetScale().x, child->GetScale().y, child->GetScale().z };

        const auto& rotation = child->GetRotation();
        childJson["rotation"] = { rotation.x, rotation.y, rotation.z };

        childJson["isActive"] = child->GetActive();
        childJson["isVisible"] = child->GetVisible();
        childJson["layer"] = static_cast<int>(child->GetLayer());
        //childJson["depth"] = child->depth; 

        // S�rialise les composants de l'enfant s'il y en a
        if (!child->GetComponents().empty())
        {
            json componentsJson;
            for (const Component* component : child->GetComponents())
            {
                json componentJson;
                SerializeComponent(component, componentJson);
                componentsJson.push_back(componentJson);
            }
            childJson["components"] = componentsJson;
        }

        // R�cursion pour s�rialiser les enfants de cet enfant
        if (!child->children.empty())
        {
            SerializeChildren(child->children, childJson["children"]);
        }

        // Ajoute l'objet JSON de l'enfant � l'objet JSON du parent
        parentJson.push_back(childJson);
    }
}

void GameObjectSerializer::SerializeGameObject(const GameObject& gameObject, const std::string& filename)
{
    json serializedData; // Cr�e un objet JSON vide pour le GameObject principal

    // Ajoute les donn�es du GameObject � l'objet JSON principal
    serializedData["name"] = gameObject.GetName();
    serializedData["position"] = { gameObject.GetPosition().x, gameObject.GetPosition().y, gameObject.GetPosition().z };
    serializedData["scale"] = { gameObject.GetScale().x, gameObject.GetScale().y, gameObject.GetScale().z };

    const auto& rotation = gameObject.GetRotation();
    serializedData["rotation"] = { rotation.x, rotation.y, rotation.z };

    serializedData["isActive"] = gameObject.GetActive();
    serializedData["isVisible"] = gameObject.GetVisible();
    serializedData["layer"] = static_cast<int>(gameObject.GetLayer());
    //serializedData["depth"] = gameObject.depth;

    // S�rialise les composants du GameObject s'il y en a
    if (!gameObject.GetComponents().empty())
    {
        json componentsJson;
        for (const Component* component : gameObject.GetComponents())
        {
            json componentJson;
            SerializeComponent(component, componentJson);
            componentsJson.push_back(componentJson);
        }
        serializedData["components"] = componentsJson;
    }

    // S�rialise les enfants du GameObject s'il y en a
    if (!gameObject.children.empty())
    {
        SerializeChildren(gameObject.children, serializedData["children"]);
    }

    // Convertit l'objet JSON en cha�ne JSON format�e
    std::string jsonString = serializedData.dump(4);

    // �crit la cha�ne JSON dans un fichier
    std::ofstream file(filename);
    file << jsonString;
    file.close();

    std::cout << "GameObject sauvegard� avec succ�s dans " << filename << std::endl;
}
