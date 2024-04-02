#include "Camera/CameraManager.h"

#include <iostream>


#include "Camera/BaseCamera.h"

namespace lve
{
	CameraManager::CameraManager()
	{
	}

	void CameraManager::Initialize()
	{
		BaseCamera* camera = CurrentCamera();
		/*CHECK_NE(camera, nullptr);*/

		camera->Initialize();
		/*camera->OnPossess();*/

		/*g_InputManager->BindActionCallback(&m_ActionCallback, 12);

		m_bInitialized = true;*/
	}

	void CameraManager::Destroy()
	{
		while (!cameraStack.empty())
		{
			cameraStack.pop();
		}

		for (BaseCamera* camera : cameras)
		{
			camera->Destroy();
			delete camera;
		}
		cameras.clear();

		/*g_InputManager->UnbindActionCallback(&m_ActionCallback);

		m_bInitialized = false;*/
	}

	void CameraManager::Update()
	{
	}

	void CameraManager::OnPostSceneChange()
	{
		for (BaseCamera* cam : cameras)
		{
			/*cam->OnPostSceneChange();*/
		}
	}

	BaseCamera* CameraManager::CurrentCamera() const
	{
		return cameraStack.top();
	}

	void CameraManager::AddCamera(BaseCamera* _camera, bool _cameraSwitchTo)
	{
		/*CHECK_NE(camera, nullptr);*/

		uint32_t cameraIndex = GetCameraIndex(_camera);
		if (cameraIndex == -1) // Only add camera if it hasn't been added before
		{
			cameras.push_back(_camera);

			if (_cameraSwitchTo)
			{
				SetCamera(_camera, false);
			}
		}
	}

	BaseCamera* CameraManager::SetCamera(BaseCamera* _camera, bool _bAlignWithPrevious)
	{
		if (!cameraStack.empty())
		{
			BaseCamera* activeCamera = CurrentCamera();
			/*activeCamera->OnDepossess();*/
			activeCamera->Destroy();
		}

		while (cameraStack.empty())
		{
			cameraStack.pop();
		}

		return PushCamera(_camera, _bAlignWithPrevious, true);
	}

	BaseCamera* CameraManager::CycleCamera(uint32_t _deltaIndex, bool _bAlignWithPrevious)
	{
		/*CHECK_EQ(glm::abs(_deltaIndex), 1);*/

		const uint32_t numCameras = (uint32_t)cameras.size();

		const uint32_t desiredIndex = GetCameraIndex(cameraStack.top()) + _deltaIndex;
		uint32_t newIndex;
		uint32_t offset = 0;
		do
		{
			newIndex = desiredIndex + offset;
			offset += _deltaIndex;
			if (newIndex < 0)
			{
				newIndex += numCameras;
			}
			else if (newIndex >= numCameras)
			{
				newIndex -= numCameras;
			}
		} while (!cameras[newIndex]->bDEBUGCyclable);

		return SetCamera(cameras[newIndex], _bAlignWithPrevious);
	}

	BaseCamera* CameraManager::SetCameraByName(const std::string& _name, bool _bAlignWithPrevious)
	{
		BaseCamera* cam = GetCameraByName(_name);
		if (cam == nullptr)
		{
			std::string errorStr = "Attempted to set camera with invalid name: " + _name + "\n";
			std::cout << ("%s", errorStr.c_str()) << std::endl;
			return nullptr;
		}
		return SetCamera(cam, _bAlignWithPrevious);
	}

	BaseCamera* CameraManager::PushCamera(BaseCamera* _camera, bool _bAlignWithPrevious, bool _bInitialize)
	{
		if (_camera == nullptr)
		{
			std::cout << ("Attempted to push null camera\n") << std::endl;
			return nullptr;
		}

		/*g_InputManager->ClearAllInputs();*/

		BaseCamera* pActiveCam = nullptr;
		if (!cameraStack.empty())
		{
			pActiveCam = cameraStack.top();

			/*pActiveCam->OnDepossess();*/
			pActiveCam->Destroy();
		}

		cameraStack.push(_camera);

		if (_bAlignWithPrevious && pActiveCam != nullptr)
		{
			AlignCameras(pActiveCam, _camera);
		}

		if (_bInitialize && initialized)
		{
			_camera->Initialize();
			/*camera->OnPossess();*/
		}

		return _camera;
	}

	BaseCamera* CameraManager::PushCameraByName(const std::string& _name, bool _bAlignWithPrevious, bool _bInitialize)
	{
		BaseCamera* cam = GetCameraByName(_name);
		if (cam == nullptr)
		{
			std::string errorStr = "Attempted to push camera with invalid name: " + _name + "\n";
			std::cout << ("%s", errorStr.c_str()) << std::endl;
			return nullptr;
		}
		return PushCamera(cam, _bAlignWithPrevious, _bInitialize);
	}

	void CameraManager::PopCamera(bool _bAlignWithCurrent /* = false */)
	{
		if (cameraStack.size() <= 1)
		{
			std::cout << PrintError("CameraManager::PopCamera - Attempted to pop final camera from stack\n") << std::endl;
			return;
		}

		/*g_InputManager->ClearAllInputs();*/

		BaseCamera* currentCamera = CurrentCamera();
		/*currentCamera->OnDepossess();*/
		currentCamera->Destroy();
		BaseCamera* prevCamera = currentCamera;

		cameraStack.pop();

		currentCamera = CurrentCamera();
		/*currentCamera->OnPossess();*/
		currentCamera->Initialize();

		if (_bAlignWithCurrent)
		{
			AlignCameras(prevCamera, currentCamera);
		}
	}

	BaseCamera* CameraManager::GetCameraByName(const std::string& _name)
	{
		for (uint32_t i = 0; i < (uint32_t)cameras.size(); ++i)
		{
			if (cameras[i]->GetName().compare(_name) == 0)
			{
				return cameras[i];
			}
		}

		return nullptr;
	}

	/*void CameraManager::DrawImGuiObjects()
	{
		if (ImGui::TreeNode("Camera"))
		{
			BaseCamera* currentCamera = CurrentCamera();

			const i32 cameraCount = (i32)m_Cameras.size();

			if (cameraCount > 1) // Only show arrows if other cameras exist
			{
				bool cycleLeft = ImGui::Button("<");
				if (ImGui::IsItemHovered())
				{
					static StringBuilder actionBindingBuff;
					actionBindingBuff.Clear();
					if (g_InputManager->GetActionBindingName(Action::DBG_SWITCH_TO_PREV_CAM, actionBindingBuff))
					{
						ImGui::SetTooltip("Shortcut: %s", actionBindingBuff.ToCString());
					}
				}
				if (cycleLeft)
				{
					CycleCamera(-1, false);
				}

				ImGui::SameLine();

				std::string cameraNameStr = currentCamera->GetName();
				ImGui::Text("%12s", cameraNameStr.c_str());

				ImGui::SameLine();

				bool cycleRight = ImGui::Button(">");
				if (ImGui::IsItemHovered())
				{
					static StringBuilder actionBindingBuff;
					actionBindingBuff.Clear();
					if (g_InputManager->GetActionBindingName(Action::DBG_SWITCH_TO_NEXT_CAM, actionBindingBuff))
					{
						ImGui::SetTooltip("Shortcut: %s", actionBindingBuff.ToCString());
					}
				}
				if (cycleRight)
				{
					CycleCamera(1, false);
				}
			}

			ImGui::SliderFloat("Move speed", &currentCamera->moveSpeed, 1.0f, 750.0f);

			real turnSpeed = glm::degrees(currentCamera->mouseRotationSpeed);
			if (ImGui::SliderFloat("Turn speed", &turnSpeed, 0.01f, 0.3f))
			{
				currentCamera->mouseRotationSpeed = glm::radians(turnSpeed);
			}

			ImGui::DragFloat3("Position", &currentCamera->position.x, 0.1f);

			glm::vec2 camYawPitch;
			camYawPitch[0] = glm::degrees(currentCamera->yaw);
			camYawPitch[1] = glm::degrees(currentCamera->pitch);
			if (ImGui::DragFloat2("Yaw & Pitch", &camYawPitch.x, 0.05f))
			{
				currentCamera->yaw = glm::radians(camYawPitch[0]);
				currentCamera->pitch = glm::radians(camYawPitch[1]);
			}

			real camFOV = glm::degrees(currentCamera->FOV);
			if (ImGui::DragFloat("FOV", &camFOV, 0.05f, 10.0f, 150.0f))
			{
				currentCamera->FOV = glm::radians(camFOV);
			}

			ImGui::DragFloat("near", &currentCamera->zNear, 0.05f, 0.0f, 10.0f);

			ImGui::DragFloat("far", &currentCamera->zFar, 0.05f, 0.0f, 10000.0f);

			if (ImGui::Button("Reset orientation"))
			{
				currentCamera->ResetOrientation();
			}

			ImGui::SameLine();
			if (ImGui::Button("Reset position"))
			{
				currentCamera->ResetPosition();
			}

			CurrentCamera()->DrawImGuiObjects();

			ImGui::TreePop();
		}
	}*/

	uint32_t CameraManager::GetCameraIndex(BaseCamera* _camera)
	{
		for (uint32_t i = 0; i < cameras.size(); ++i)
		{
			if (cameras[i] == _camera)
			{
				return i;
			}
		}

		return -1;
	}

	void CameraManager::AlignCameras(BaseCamera* _from, BaseCamera* _to)
	{
		_to->position = _from->position;
		_to->pitch = _from->pitch;
		_to->yaw = _from->yaw;
		_to->FOV = _from->FOV;
	}

	/*EventReply CameraManager::OnActionEvent(Action action, ActionEvent actionEvent)
	{
		if (actionEvent == ActionEvent::ACTION_TRIGGER)
		{
			if (action == Action::DBG_SWITCH_TO_PREV_CAM)
			{
				CycleCamera(-1, false);
				return EventReply::CONSUMED;
			}
			else if (action == Action::DBG_SWITCH_TO_NEXT_CAM)
			{
				CycleCamera(1, false);
				return EventReply::CONSUMED;
			}
		}

		return EventReply::UNCONSUMED;
	}*/

} // namespace flex