#pragma once
#include <Project/ProjectManager.h>
#include <Objects/Base/ObjectManager.h>
#include <Rendering/Interfaces/Renderer.h>

enum EngineMode : uint8_t {
	EDITOR_MODE, EDITOR_PLAY_MODE, RUNTIME_MODE
};

namespace Engine::Editor {
	class Editor {
	public:
		static Editor* get_Instance() {
			static Editor editor;
			return &editor;
		}
		void init(std::unique_ptr<Engine::Graphics::RendererInterface>& renderer);
		void update();
		void render(std::unique_ptr<Engine::Graphics::RendererInterface>& renderer);

		void SaveCurrentProject();
		void SaveCurrentScene();

		void BuildScene();
		void CreateEmptyObject();
		void DeleteObject(uint32_t obj_id);
	private:
		Editor() {};
		~Editor() {};
		ProjectManager* project_Mgr = nullptr;
		SceneManager* scene_Mgr = nullptr;
		Object::ObjectManager* object_Mgr = nullptr;
	};
}