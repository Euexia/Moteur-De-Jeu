#pragma once
#include "Script.h"
#include "GameObject/GameObject.h"
#include <string>

	class GameObjectScript : public Script
	{
	public:

		GameObjectScript();

		~GameObjectScript() = default;

	protected:

		GameObject object;
	};


