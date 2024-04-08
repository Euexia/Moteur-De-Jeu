#include "GameObject/Component/CharacterController.h"

CharacterController::CharacterController(GameObject* gameobject) : Component(gameobject) {
	
}

glm::vec3 CharacterController::setCenter(float _x, float _y, float _z)
{
	// déplace le center du collider
	glm::vec3 center(_x, _y, _z);
	return center;
}

void CharacterController::setSkinWidth(float _width)
{
	glm::vec3 newScale = gameobject->GetScale();
	gameobject->SetScale((newScale + _width));
}

void CharacterController::setRadius()
{
	// augmente le radius du collider
}

void CharacterController::setHeight()
{
	// augmente la taille en Y du collider
}