#pragma once
#include "ObjectManager.h"
#include "Transform.h"
#include "SpriteComponent.h"
#include "MeshComponent.h"
#include "Image.h"

namespace EC = Engine::Component;
namespace EUI = Engine::UI;

class PlayerController : public Engine::Object::ScriptBase {
public:
	DECLARE_ON_CONSTRUCT_FLAG
	void OnConstruct() override;
	DECLARE_UPDATE_FLAG
	void Update() override;

	void set_test_image(EUI::UI_Component_Ptr<EUI::Image>& img_ptr);
private:
	EC::Component_Ptr<EC::Transform> transform;
	EC::Component_Ptr<EC::SpriteComponent> sc;
	EC::Component_Ptr<EC::MeshComponent> mc;

	EUI::UI_Component_Ptr<EUI::Image> image;
	bool canControl = false;
	float cameraRotateSpeed = 30.0f;
};
