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

void Engine::Component::Transform::translateDirectionally(float dx, float dy, float dz) {
	float yawRad = glm::radians(rotation.y);
	float pitchRad = glm::radians(rotation.x);
	float cy = cos(yawRad);
	float sy = sin(yawRad);
	float cp = cos(pitchRad);
	float sp = sin(pitchRad);

	glm::vec3 forward;
	forward.x = cp * sy;
	forward.y = -sp;
	forward.z = cp * cy;

	position += forward * dz;

	glm::vec3 right = glm::normalize(glm::cross(glm::vec3(0.f, 1.f, 0.f), forward));
	position += right * dx;

	glm::vec3 up = glm::normalize(glm::cross(right, forward));
	position += up * dy;
	mark_dirty();
}

void Engine::Component::Transform::translateDirectionally(glm::vec3& tr_vec) {
	translateDirectionally(tr_vec.x, tr_vec.y, tr_vec.z);
}

void Engine::Component::Transform::translateDirectionallyX(float dx) {
	float yawRad = glm::radians(rotation.y);
	float rollRad = glm::radians(rotation.z);
	float cy = cos(yawRad);
	float sy = sin(yawRad);
	float cr = cos(rollRad);
	float sr = sin(rollRad);
	glm::vec3 right;
	right.x = cr * cy;
	right.y = sr * cy;
	right.z = -sy;
	position += right * dx;
	mark_dirty();
}

void Engine::Component::Transform::translateDirectionallyY(float dy) {
	float pitchRad = glm::radians(rotation.x);
	float rollRad = glm::radians(rotation.z);
	float cp = cos(pitchRad);
	float sp = sin(pitchRad);
	float cr = cos(rollRad);
	float sr = sin(rollRad);
	glm::vec3 up;
	up.x = -sr * cp;
	up.y = cr * cp;
	up.z = sp;
	position += up * dy;
	mark_dirty();
}

void Engine::Component::Transform::translateDirectionallyZ(float dz) {
	float yawRad = glm::radians(rotation.y);
	float pitchRad = glm::radians(rotation.x);
	glm::vec3 forward;
	forward.x = cos(pitchRad) * sin(yawRad);
	forward.y = -sin(pitchRad);
	forward.z = cos(pitchRad) * cos(yawRad);
	position += forward * dz;
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

void Engine::Component::Transform::add_child(Component_Ptr<Transform>& child_tr, Component_Ptr<Transform>& parent_tr) {
	this->children.emplace_back(child_tr);
	child_tr->parent = parent_tr;
}

void Engine::Component::Transform::update_modelMatrix() {
	localMatrix = glm::translate(glm::mat4(1.0f), position);
	glm::quat q(glm::radians(rotation));
	glm::toMat4(glm::quat(glm::radians(rotation)));
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
