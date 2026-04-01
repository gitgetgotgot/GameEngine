#pragma once
#include "MeshRenderManager.h"
#include "MeshManager.h"
#include "TextureManager.h"
#include <TinyGLTF/tiny_gltf.h>

namespace Engine {
	namespace Models {
		constexpr char baseDirectory[] = "Resources/Models/";
		constexpr uint32_t defaultTextureID = 0;
		constexpr uint32_t defaultMaterialID = 0;

		struct ModelNode {
			uint32_t parent_mesh;
			uint32_t mesh_id;
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
			void create_model_object(uint32_t model_id);
		private:
			Mesh& create_mesh_from_model(
				const tinygltf::Model& model,
				const tinygltf::Mesh& mesh,
				std::vector<uint32_t>& meshes_indices,
				std::vector<SubMeshVertex>& meshes_vertices,
				std::vector<SubMesh>& submeshes
			);
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
}