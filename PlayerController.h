#pragma once
#include "ObjectManager.h"
#include "Transform.h"
#include "SpriteRenderer.h"
#include "MeshRenderer.h"

namespace EC = Engine::Component;
namespace EUI = Engine::UI;

class PlayerController : public Engine::Object::ScriptBase {
public:
	DECLARE_ON_CONSTRUCT_FLAG
	void OnConstruct() override;
	DECLARE_UPDATE_FLAG
	void Update() override;
private:
	EC::component_ptr<EC::Transform> transform;
	EC::component_ptr<EC::SpriteRenderer> sr;
	EC::component_ptr<EC::MeshRenderer> mr;
	bool canControl;
	float cameraRotateSpeed = 30.0f;
};
