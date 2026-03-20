#include "Transform.h"

Engine::Component::Transform::Transform(float x, float y, float z) {
	position.x = x; position.y = y; position.z = z;
	update_modelMatrix();
}

void Engine::Component::Transform::translateLocal(float x, float y, float z) {
	position.x += x; position.y += y; position.z += z;
	mark_dirty();
}

void Engine::Component::Transform::translateLocal(glm::vec3& vec) {
	position.x += vec.x; position.y += vec.y; position.z += vec.z;
	mark_dirty();
}

void Engine::Component::Transform::translateDirectionally(float x, float y, float z) {
	float yawRad = glm::radians(rotation.y);
	float pitchRad = glm::radians(rotation.x);

	glm::vec3 forward;
	forward.x = cos(pitchRad) * sin(yawRad);
	forward.y = -sin(pitchRad);
	forward.z = cos(pitchRad) * cos(yawRad);

	position += forward * z;

	glm::vec3 right = glm::normalize(glm::cross(glm::vec3(0.f, 1.f, 0.f), forward));
	position += right * x;

	glm::vec3 up = glm::normalize(glm::cross(right, forward));
	position += up * y;
	mark_dirty();
}

void Engine::Component::Transform::translateDirectionally(glm::vec3& tr_vec) {

	mark_dirty();
}

void Engine::Component::Transform::rotate(bool x, bool y, bool z, float angle_in_degrees) {
	if (x) rotation.x += angle_in_degrees;
	if (y) rotation.y += angle_in_degrees;
	if (z) rotation.z += angle_in_degrees;
	mark_dirty();
}

void Engine::Component::Transform::set_rotation(float x, float y, float z) {
	rotation.x = x; rotation.y = y; rotation.z = z;
	mark_dirty();
}

void Engine::Component::Transform::set_rotation(glm::vec3& rotate_vec) {
	rotation.x = rotate_vec.x; rotation.y = rotate_vec.y; rotation.z = rotate_vec.z;
	mark_dirty();
}

void Engine::Component::Transform::set_scale(float x, float y, float z) {
	scale.x = x; scale.y = y; scale.z = z;
	mark_dirty();
}

void Engine::Component::Transform::set_scale(glm::vec3& scale_vec) {
	scale.x = scale_vec.x; scale.y = scale_vec.y; scale.z = scale_vec.z;
	mark_dirty();
}

void Engine::Component::Transform::add_child(component_ptr<Transform>& child_tr, component_ptr<Transform>& parent_tr) {
	this->children.emplace_back(child_tr);
	child_tr->parent = parent_tr;
}

void Engine::Component::Transform::update_modelMatrix() {
	localMatrix = glm::translate(glm::mat4(1.0f), position);
	localMatrix = glm::rotate(localMatrix, glm::radians(rotation.z), glm::vec3(0, 0, 1));
	localMatrix = glm::rotate(localMatrix, glm::radians(rotation.y), glm::vec3(0, 1, 0));
	localMatrix = glm::rotate(localMatrix, glm::radians(rotation.x), glm::vec3(1, 0, 0));
	localMatrix = glm::scale(localMatrix, scale);
	if (parent) {
		worldMatrix = parent->worldMatrix * localMatrix;
	}
	else {
		worldMatrix = localMatrix;
	}
	isDirty = false;
	//update children transforms
	update(worldMatrix);
}

void Engine::Component::Transform::update(const glm::mat4& parentWorldMatrix) {
	for (auto& child : children) {
		if (child) {
			Transform& child_tr = *child;
			child_tr.worldMatrix = parentWorldMatrix * child_tr.localMatrix;
			child_tr.update(child_tr.worldMatrix);
		}
		else {

		}
	}
}

void Engine::Component::Transform::mark_dirty() {
	if (!isDirty) {
		isDirty = true;
		TransformUpdater::dirtyTransforms.emplace_back(obj_id);
	}
}
