#pragma once
#include "MeshRenderManager.h"
#include "MeshManager.h"
#include "TextureManager.h"
#include "ObjectManager.h"
#include <TinyGLTF/tiny_gltf.h>

namespace Engine::Models {
	constexpr char baseDirectory[] = "Resources/Models/";
	constexpr uint32_t defaultTextureID = 0;
	constexpr uint32_t defaultMaterialID = 0;

	enum NodeType : uint8_t {
		isEmpty, isMesh
	};

	struct ModelNode {
		NodeType type;
		uint32_t mesh_id;
		std::vector<uint32_t> meshes;
		glm::vec3 localPos{ 0.f, 0.f, 0.f };
		glm::vec3 localScale{ 1.f, 1.f, 1.f };
		glm::vec3 localRotation{ 0.f, 0.f, 0.f };
	};

	struct Model {
		//ids of each mesh
		std::vector<ModelNode> mesh_nodes;
	};

	class ModelManager {
	public:
		static ModelManager* get_Instance() {
			static ModelManager modelLoader;
			return &modelLoader;
		}
		void init();
		//basic meshes
		void load_plane_mesh();
		void load_cube_mesh();
		void load_pyramid_mesh();
		void load_cylinder_mesh();
		//loads meshes of a 3d model, creates a Model with hierarchy if contains 2 or more meshes
		void load_model(std::string& path);
		void delete_model(uint32_t id);
		Model* get_model(uint32_t id);

		//creates a model object and returns root object_ptr
		Engine::Object::object_ptr create_model_object(uint32_t model_id);
	private:
		ModelManager() {}
		~ModelManager() {}
		sparse_set<Model> models;
		SubMeshManager* subMeshManager = nullptr;
		MeshManager* meshManager = nullptr;
		MaterialManager* materialMgr = nullptr;
		TextureManager* textureMgr = nullptr;
		MeshRenderManager* meshRenderManager = nullptr;
	};
}