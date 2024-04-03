#pragma once
#include "Module.h"

#include "lve_window.h"
#include "lve_device.h"
#include "lve_renderer.h"
#include "lve_game_object.h"
#include "lve_descriptors.h"

#include "lve_camera.h"   
#include "keyboard_movement_controller.h"   
#include "lve_buffer.h"   
#include "Systems/simple_render_system.h"   
#include "Systems/point_light_system.h"

//std
#include <memory>
#include <vector>
#include <iostream>
#include <string>
#include <map>
#include <chrono>


class WindowManager final : public Module
{
public:
	static constexpr int WIDTH = 800; ///< Largeur de la fen�tre par d�faut.
	static constexpr int HEIGHT = 600; ///< Hauteur de la fen�tre par d�faut.

	void Init() override;
	void Start() override;
	void FixedUpdate() override;
	void Update() override;
	void PreRender() override;
	void Render() override;
	void RenderGui() override;
	void PostRender() override;
	void Release() override;

	lve::LveWindow* GetWindow() { return &lveWindow; }

private:
	void LoadGameObjects();

	lve::LveWindow lveWindow{ WIDTH, HEIGHT, "Hello Vulkan!" }; /// Fen�tre de l'application.
	lve::LveDevice lveDevice{ lveWindow };
	lve::LveRenderer lveRenderer{ lveWindow, lveDevice };

	lve::LveDescriptorPool::Builder builder{ lveDevice };

	std::unique_ptr<lve::LveDescriptorSetLayout, std::default_delete<lve::LveDescriptorSetLayout>> *globalSetLayout;

	lve::SimpleRenderSystem* simpleRenderSystem;
	lve::PointLightSystem* pointLightSystem;
	lve::LveCamera* camera;

	std::chrono::steady_clock::time_point currentTime;
	lve::KeyboardMovementController cameraController{};
	lve::LveGameObject* viewerObject;

	std::vector<vk::DescriptorSet> globalDescriptorSets;
	std::vector<std::unique_ptr<lve::LveBuffer>> uboBuffers;

	// note : order of declarations matters
	std::unique_ptr<lve::LveDescriptorPool> globalPool{};
	lve::LveGameObject::Map* gameObjects;
};