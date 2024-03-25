#pragma once
#include <cassert>

class TextureManager
{
public:
	// la fonction static va �tre cr�er lors du premi�re appel de la fonction
	static RenderManager& get() {
		static RenderManager* gpSingleton = nullptr;
		if (gpSingleton == nullptr) {
			gpSingleton = new RenderManager;
		}
		assert(gpSingleton);
		return *gpSingleton;
	}
private:
	TextureManager() {}

	~TextureManager() {}

	void Awake() {}

	void Create() {}

	void Update() {}

	void start() {}

	void stop() {}
};
