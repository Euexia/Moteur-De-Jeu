#pragma once
#include "Managers/TextureManager.h"

class RenderManager
{
public:
	RenderManager()
	{
		TextureManager::get();
	}

	~RenderManager() {}

	void Awake() {}

	void Create() {}

	void Update() {}

	void start() {}

	void stop() {}
};