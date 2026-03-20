#include "ObjectPointer.h"
#include "ObjectManager.h"

Engine::Object::Object* Engine::Object::object_ptr::operator->() {
	return Engine::Object::ObjectManager::get_Instance()->get_object(obj_id);
}

Engine::Object::Object& Engine::Object::object_ptr::operator*() {
	return *Engine::Object::ObjectManager::get_Instance()->get_object(obj_id);
}

Engine::Object::object_ptr& Engine::Object::object_ptr::operator=(Engine::Object::Object* obj_ptr) {
	this->obj_id = obj_ptr->get_id();
	return *this;
}

Engine::Object::object_ptr::operator bool() const {
	return Engine::Object::ObjectManager::get_Instance()->get_object(obj_id);
}