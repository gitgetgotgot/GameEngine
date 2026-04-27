#include <Editor/Editor.h>

void Engine::Editor::Editor::init(std::unique_ptr<Engine::Graphics::RendererInterface>& renderer) {
	project_Mgr = ProjectManager::get_Instance();
	scene_Mgr = SceneManager::get_Instance();
	object_Mgr = Object::ObjectManager::get_Instance();
}

void Engine::Editor::Editor::update() {

}

void Engine::Editor::Editor::render(std::unique_ptr<Engine::Graphics::RendererInterface>& renderer) {

}

void Engine::Editor::Editor::SaveCurrentProject() {
	project_Mgr->SaveProject();
}

void Engine::Editor::Editor::SaveCurrentScene() {
	scene_Mgr->SaveScene();
}

void Engine::Editor::Editor::BuildScene() {
	sparse_set<ObjectInfo>& scene_objects = scene_Mgr->get_current_scene()->objects;
	uint32_t objects_count = scene_objects.size();
	ObjectInfo* obj_ptr = scene_objects.data();

	uint32_t components_count;
	for (uint32_t i = 0; i < objects_count; i++) {
		ObjectInfo& obj_info = *obj_ptr++;
		auto object = object_Mgr->InstantiateObject(obj_info.id);

		components_count = obj_info.components.size();
		for (uint32_t c = 0; c < components_count; c++) {
			std::unique_ptr<ComponentInfo>& comp_info = obj_info.components[c];
			switch (comp_info->type) {
			case ComponentType::isTransform: {
				TransformInfo* tr = static_cast<TransformInfo*>(comp_info.get());
				
				break;
			}
			case ComponentType::isSpriteComp: {

				break;
			}
			case ComponentType::isMeshComp: {

				break;
			}
			case ComponentType::isCamera: {

				break;
			}
			}
		}
	}
}

void Engine::Editor::Editor::CreateEmptyObject() {
	auto obj_ptr = object_Mgr->InstantiateObject();
	scene_Mgr->add_object(obj_ptr->get_id());
}

void Engine::Editor::Editor::DeleteObject(uint32_t obj_id) {
	object_Mgr->DestroyObject(obj_id);
	scene_Mgr->remove_object(obj_id);
}
