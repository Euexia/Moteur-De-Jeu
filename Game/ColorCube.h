#pragma once
#include "../Librairies/glm/vec3.hpp"
#include "GameObject/Components/Transform.h"
#include "Scene/SceneManager.h"
#include "Script/GameObjectScript.h"

class ColorCube : public GameObjectScript
{
	void Start() override
	{
		redCube = GetSceneManager()->GetMainScene()->CreateCubeGameObject(1);
	}

	void Update() override
	{
		if(time <= 0.f)
		{
			glm::vec3 newPosition = redCube->GetTransform()->GetPosition() + glm::vec3{ 1.f } * GetTimeModule()->GetDeltaTime();
			redCube->GetTransform()->SetPosition(newPosition);
		}
		else
		{
			glm::vec3 newPosition = redCube->GetTransform()->GetPosition() - glm::vec3{ 1.f } * GetTimeModule()->GetDeltaTime();
			redCube->GetTransform()->SetPosition(newPosition);
		}
		time -= GetTimeModule()->GetDeltaTime();
		if(time <= -10.f)
		{
			time = 10.f;
		}
	}


	float time = 10;
	GameObject* redCube;
};

