#pragma once

#include "GameObject.h"
#include "nlohmann/json.hpp"
#include <fstream>


class GameObjectSerializer
{
public:
    static void SerializeGameObject(const GameObject& gameObject, const std::string& filename);

private:
    static void SerializeComponent(const Component* component, nlohmann::json& componentJson);
    static void SerializeChildren(const std::vector<GameObject*>& children, nlohmann::json& parentJson);

};
