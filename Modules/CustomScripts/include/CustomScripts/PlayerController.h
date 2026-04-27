#pragma once
#include <Objects/Base/ObjectManager.h>
#include <Objects/Components/SpriteComponent.h>
#include <Objects/Components/MeshComponent.h>
#include <UI/UIComponents/Image.h>
#include <UI/UIComponents/Button.h>
#include <UI/UIComponents/SDF_Text.h>

namespace EC = Engine::Component;
namespace EUI = Engine::UI;

class PlayerController : public Engine::Object::ScriptBase {
public:
	DECLARE_ON_CONSTRUCT_FLAG
	void OnConstruct() override;
	DECLARE_UPDATE_FLAG
	void Update() override;

	void set_test_UI(
		EUI::UI_Component_Ptr<EUI::Image>& img_ptr,
		EUI::UI_Component_Ptr<EUI::Button>& button_ptr,
		EUI::UI_Component_Ptr<EUI::SDF_Text>& text_ptr,
		EUI::UI_Component_Ptr<EUI::UI_Transform>& text_transform
	);
	void set_test_transform(EC::Component_Ptr<EC::Transform>& tr_ptr);
	Engine::Object::object_ptr model;
private:
	void change_image_color();
	static void wrapper(void* obj);

	EC::Component_Ptr<EC::Transform> transform;
	EC::Component_Ptr<EC::SpriteComponent> sc;
	EC::Component_Ptr<EC::MeshComponent> mc;

	EUI::UI_Component_Ptr<EUI::Image> image;
	EUI::UI_Component_Ptr<EUI::Button> button;
	EUI::UI_Component_Ptr<EUI::SDF_Text> text;
	EUI::UI_Component_Ptr<EUI::UI_Transform> text_transform;
	bool canControl = false;
	float cameraRotateSpeed = 30.0f;
	bool color_changed = false;
};
