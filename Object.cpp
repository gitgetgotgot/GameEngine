#include "Object.h"
#include "ObjectManager.h"

Engine::Object::Object* Engine::Object::object_ptr::operator->() {
	return object_manager_ptr->get_object(obj_id);
}

Engine::Object::Object& Engine::Object::object_ptr::operator*() {
	return *object_manager_ptr->get_object(obj_id);
}

Engine::Object::object_ptr& Engine::Object::object_ptr::operator=(Object* obj_ptr) {
	this->obj_id = obj_ptr->get_id();
	return *this;
}

Engine::Object::object_ptr& Engine::Object::object_ptr::operator=(object_ptr obj_ptr) {
	this->obj_id = obj_ptr.obj_id;
	return *this;
}

Engine::Object::object_ptr::operator bool() const {
	return object_manager_ptr->get_object(obj_id);
}

bool Engine::Object::Object::add_child_object(object_ptr& child_object) {
	auto tr_parent = this->get_component<Component::Transform>();
	auto tr_child = child_object->get_component<Component::Transform>();
	if (tr_parent && tr_child) {
		tr_parent->add_child(tr_child);
		return true;
	}
	return false;
}

bool Engine::Object::Object::add_child_UI_object(object_ptr& child_UI_object) {
	auto tr_parent = this->get_ui_component<UI::UI_Transform>();
	auto tr_child = child_UI_object->get_ui_component<UI::UI_Transform>();
	if (tr_parent && tr_child) {
		tr_parent->add_child(tr_child);
		return true;
	}
	return false;
}

void Engine::Object::Object::Destroy() {
	Engine::Systems::DestroyedObjects::destroyed_objects.emplace_back(id);
	auto transform = get_component<Component::Transform>();
	if (transform) {
		std::vector<Component::Component_Ptr<Component::Transform>>& children = transform->get_children();
		for (auto& c : children)
			Engine::Systems::DestroyedObjects::destroyed_objects.emplace_back(c.get_object_id());
	}
}
