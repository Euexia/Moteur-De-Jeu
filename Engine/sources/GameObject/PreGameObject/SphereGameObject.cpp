#include "GameObject/PreGameObject/SphereGameObject.h"

namespace lve
{
	GameObject* SphereGameObject::Create(LveDevice& _lveDevice, const glm::vec3 _position,
		const glm::vec3 _scale, const glm::vec3 _rotation)
	{
		const std::shared_ptr<LveModel> lve_model = LveModel::CreateModelFromFile(_lveDevice, "Models\\sphere.obj");

		GameObject* game_object = GameObject::CreatePGameObject();
		game_object->SetModel(lve_model);
		game_object->GetTransform()->SetPosition(_position);
		game_object->GetTransform()->SetScale(_scale);
		game_object->GetTransform()->SetRotation(_rotation);
		game_object->SetFileModel("Models\\sphere.obj");

		return game_object;
	}
}