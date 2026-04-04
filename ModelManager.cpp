#include "ModelManager.h"
#include <glm/gtc/type_ptr.hpp>
#include <stb/stb_image.h>

void Engine::Models::ModelManager::init() {
    meshManager = MeshManager::get_Instance();
    subMeshManager = SubMeshManager::get_Instance();
    meshRenderManager = MeshRenderManager::get_Instance();
    materialMgr = MaterialManager::get_Instance();
    textureMgr = TextureManager::get_Instance();
}

void Engine::Models::ModelManager::load_plane_mesh() {
    std::vector<SubMeshVertex> vertices = {
        // Front face (z = +0.5)
        { { -0.5f, 0.0f,  0.5f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f } },
        { { -0.5f, 0.0f, -0.5f }, { 0.f, 1.f, 0.f }, { 0.f, 1.f } },
        { {  0.5f, 0.0f, -0.5f }, { 0.f, 1.f, 0.f }, { 1.f, 1.f } },
        { {  0.5f, 0.0f,  0.5f }, { 0.f, 1.f, 0.f }, { 1.f, 0.f } }
    };
    std::vector<uint32_t> indices = {
        0, 1, 2,    0, 2, 3
    };
    SubMesh submesh;
    submesh.vertexCount = vertices.size();
    submesh.indexCount = indices.size();
    SubMesh& newSubMesh = SubMeshManager::get_Instance()->add(std::move(submesh));
    Mesh mesh;
    mesh.subMeshes.emplace_back(newSubMesh.id);
    MeshManager::get_Instance()->add_mesh(std::move(mesh));
    MeshRenderManager::get_Instance()->load_submesh(indices, vertices, submesh);
}

void Engine::Models::ModelManager::load_cube_mesh() {
    std::vector<SubMeshVertex> vertices = {
        // Front (+Z)
        { { -0.5f, -0.5f,  0.5f }, { 0.f, 0.f, 1.f }, { 0.f, 0.f } },
        { {  0.5f, -0.5f,  0.5f }, { 0.f, 0.f, 1.f }, { 1.f, 0.f } },
        { {  0.5f,  0.5f,  0.5f }, { 0.f, 0.f, 1.f }, { 1.f, 1.f } },
        { { -0.5f,  0.5f,  0.5f }, { 0.f, 0.f, 1.f }, { 0.f, 1.f } },
        // Back (−Z)
        { {  0.5f, -0.5f, -0.5f }, { 0.f, 0.f,-1.f }, { 0.f, 0.f } },
        { { -0.5f, -0.5f, -0.5f }, { 0.f, 0.f,-1.f }, { 1.f, 0.f } },
        { { -0.5f,  0.5f, -0.5f }, { 0.f, 0.f,-1.f }, { 1.f, 1.f } },
        { {  0.5f,  0.5f, -0.5f }, { 0.f, 0.f,-1.f }, { 0.f, 1.f } },
        // Left (−X)
        { { -0.5f, -0.5f, -0.5f }, { -1.f, 0.f, 0.f }, { 0.f, 0.f } },
        { { -0.5f, -0.5f,  0.5f }, { -1.f, 0.f, 0.f }, { 1.f, 0.f } },
        { { -0.5f,  0.5f,  0.5f }, { -1.f, 0.f, 0.f }, { 1.f, 1.f } },
        { { -0.5f,  0.5f, -0.5f }, { -1.f, 0.f, 0.f }, { 0.f, 1.f } },
        // Right (+X)
        { {  0.5f, -0.5f,  0.5f }, { 1.f, 0.f, 0.f }, { 0.f, 0.f } },
        { {  0.5f, -0.5f, -0.5f }, { 1.f, 0.f, 0.f }, { 1.f, 0.f } },
        { {  0.5f,  0.5f, -0.5f }, { 1.f, 0.f, 0.f }, { 1.f, 1.f } },
        { {  0.5f,  0.5f,  0.5f }, { 1.f, 0.f, 0.f }, { 0.f, 1.f } },
        // Bottom (−Y)
        { { -0.5f, -0.5f, -0.5f }, { 0.f,-1.f, 0.f }, { 0.f, 0.f } },
        { {  0.5f, -0.5f, -0.5f }, { 0.f,-1.f, 0.f }, { 1.f, 0.f } },
        { {  0.5f, -0.5f,  0.5f }, { 0.f,-1.f, 0.f }, { 1.f, 1.f } },
        { { -0.5f, -0.5f,  0.5f }, { 0.f,-1.f, 0.f }, { 0.f, 1.f } },
        // Top (+Y)
        { { -0.5f,  0.5f,  0.5f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f } },
        { {  0.5f,  0.5f,  0.5f }, { 0.f, 1.f, 0.f }, { 1.f, 0.f } },
        { {  0.5f,  0.5f, -0.5f }, { 0.f, 1.f, 0.f }, { 1.f, 1.f } },
        { { -0.5f,  0.5f, -0.5f }, { 0.f, 1.f, 0.f }, { 0.f, 1.f } }
    };
    std::vector<uint32_t> indices = {
        // Front
        0, 1, 2,   0, 2, 3,
        // Back
        4, 5, 6,   4, 6, 7,
        // Left
        8, 9, 10,  8, 10, 11,
        // Right
        12, 13, 14, 12, 14, 15,
        // Bottom
        16, 17, 18, 16, 18, 19,
        // Top
        20, 21, 22, 20, 22, 23
    };
    SubMesh submesh;
    submesh.vertexCount = vertices.size();
    submesh.indexCount = indices.size();
    SubMesh& newSubMesh = SubMeshManager::get_Instance()->add(std::move(submesh));
    Mesh mesh;
    mesh.subMeshes.emplace_back(newSubMesh.id);
    MeshManager::get_Instance()->add_mesh(std::move(mesh));
    MeshRenderManager::get_Instance()->load_submesh(indices, vertices, submesh);
}

void Engine::Models::ModelManager::load_pyramid_mesh() {
    std::vector<SubMeshVertex> vertices = {
        // Base (square, z = -0.5, CW order)
        { { -0.5f, -0.5f, -0.5f }, { 0.f, 0.f, -1.f }, { 0.f, 0.f } }, // BL
        { {  0.5f, -0.5f, -0.5f }, { 0.f, 0.f, -1.f }, { 1.f, 0.f } }, // BR
        { {  0.5f,  0.5f, -0.5f }, { 0.f, 0.f, -1.f }, { 1.f, 1.f } }, // TR
        { { -0.5f,  0.5f, -0.5f }, { 0.f, 0.f, -1.f }, { 0.f, 1.f } }, // TL
        // Front face (apex + bottom edge)
        { { -0.5f, -0.5f, -0.5f }, { 0.f, -0.5f, 0.5f }, { 0.f, 0.f } }, // BL
        { {  0.5f, -0.5f, -0.5f }, { 0.f, -0.5f, 0.5f }, { 1.f, 0.f } }, // BR
        { {  0.0f,  0.0f,  0.5f }, { 0.f, -0.5f, 0.5f }, { 0.5f, 1.f } }, // Apex
        // Right face
        { {  0.5f, -0.5f, -0.5f }, { 0.5f, 0.f, 0.5f }, { 0.f, 0.f } }, // BR
        { {  0.5f,  0.5f, -0.5f }, { 0.5f, 0.f, 0.5f }, { 1.f, 0.f } }, // TR
        { {  0.0f,  0.0f,  0.5f }, { 0.5f, 0.f, 0.5f }, { 0.5f, 1.f } }, // Apex
        // Back face
        { {  0.5f,  0.5f, -0.5f }, { 0.f, 0.5f, 0.5f }, { 0.f, 0.f } }, // TR
        { { -0.5f,  0.5f, -0.5f }, { 0.f, 0.5f, 0.5f }, { 1.f, 0.f } }, // TL
        { {  0.0f,  0.0f,  0.5f }, { 0.f, 0.5f, 0.5f }, { 0.5f, 1.f } }, // Apex
        // Left face
        { { -0.5f,  0.5f, -0.5f }, { -0.5f, 0.f, 0.5f }, { 0.f, 0.f } }, // TL
        { { -0.5f, -0.5f, -0.5f }, { -0.5f, 0.f, 0.5f }, { 1.f, 0.f } }, // BL
        { {  0.0f,  0.0f,  0.5f }, { -0.5f, 0.f, 0.5f }, { 0.5f, 1.f } } // Apex
    };
    std::vector<uint32_t> indices = {
        0, 1, 2,    0, 2, 3,
        4, 5, 6,
        7, 8, 9,
        10, 11, 12,
        13, 14, 15
    };
    SubMesh submesh;
    submesh.vertexCount = vertices.size();
    submesh.indexCount = indices.size();
    SubMesh& newSubMesh = SubMeshManager::get_Instance()->add(std::move(submesh));
    Mesh mesh;
    mesh.subMeshes.emplace_back(newSubMesh.id);
    MeshManager::get_Instance()->add_mesh(std::move(mesh));
    MeshRenderManager::get_Instance()->load_submesh(indices, vertices, submesh);
}

void Engine::Models::ModelManager::load_cylinder_mesh() {
    int N = 20;
    float M_PI = 3.14159265359;
    std::vector<SubMeshVertex> vertices;
    std::vector<uint32_t> indices;
    // Верхнее основание
    vertices.push_back({ {0.f, 0.5f, 0.f}, {0.f, 1.f, 0.f}, {0.5f, 0.5f} }); // центр
    for (int i = 0; i < N; i++) {
        float angle = 2.0f * M_PI * i / N;
        float x = 0.5f * cos(angle);
        float z = 0.5f * sin(angle);
        vertices.push_back({ {x, 0.5f, z}, {0.f, 1.f, 0.f}, { (x + 0.5f), (z + 0.5f) } });
    }
    // Нижнее основание
    vertices.push_back({ {0.f, -0.5f, 0.f}, {0.f, -1.f, 0.f}, {0.5f, 0.5f} }); // центр
    for (int i = 0; i < N; i++) {
        float angle = 2.0f * M_PI * i / N;
        float x = 0.5f * cos(angle);
        float z = 0.5f * sin(angle);
        vertices.push_back({ {x, -0.5f, z}, {0.f, -1.f, 0.f}, { (x + 0.5f), (z + 0.5f) } });
    }

    for (int i = 0; i < N; i++) {
        indices.push_back(0);
        indices.push_back(1 + i);
        indices.push_back(1 + (i + 1) % N);
    }
    for (int i = 0; i < N; i++) {
        indices.push_back(N + 1);
        indices.push_back(N + 2 + (i + 1) % N);
        indices.push_back(N + 2 + i);
    }
    for (int i = 0; i < N; i++) {
        int top1 = 1 + i;
        int top2 = 1 + (i + 1) % N;
        int bot1 = N + 2 + i;
        int bot2 = N + 2 + (i + 1) % N;

        // первый треугольник
        indices.push_back(top1);
        indices.push_back(bot1);
        indices.push_back(top2);

        // второй треугольник
        indices.push_back(top2);
        indices.push_back(bot1);
        indices.push_back(bot2);
    }
    SubMesh submesh;
    submesh.vertexCount = vertices.size();
    submesh.indexCount = indices.size();
    SubMesh& newSubMesh = SubMeshManager::get_Instance()->add(std::move(submesh));
    Mesh mesh;
    mesh.subMeshes.emplace_back(newSubMesh.id);
    MeshManager::get_Instance()->add_mesh(std::move(mesh));
    MeshRenderManager::get_Instance()->load_submesh(indices, vertices, submesh);
}

void Engine::Models::ModelManager::load_model(std::string& path) {
    //LOAD MODEL
    tinygltf::Model model;
    tinygltf::TinyGLTF parser;
    std::string err, warn;
    bool success = false;

    if (path.ends_with(".glb"))
        success = parser.LoadBinaryFromFile(&model, &err, &warn, path); //.glb
    else
        success = parser.LoadASCIIFromFile(&model, &err, &warn, path);  //.gltf

    if (!warn.empty())
        std::cout << "[ModelLoader] Warning: " << warn << std::endl;

    if (!err.empty())
        std::cerr << "[ModelLoader] Error: " << err << std::endl;

    if (!success) return;

    std::unordered_map<int, uint32_t> materials_cache;
    std::unordered_map<int, uint32_t> textures_cache;
    std::vector<uint32_t> submeshes_indices;
    std::vector<SubMeshVertex> submeshes_vertices;
    std::vector<SubMesh> submeshes;
    Model meshModel;
    int meshes_count = 0;
    stbi_set_flip_vertically_on_load(false);

    auto create_mesh_from_model = [&](tinygltf::Mesh& mesh) {
        Mesh& newMesh = meshManager->add_mesh(Mesh{});
        for (const auto& primitive : mesh.primitives) {
            //INDICES
            const tinygltf::Accessor& indexAccessor = model.accessors[primitive.indices];
            const tinygltf::BufferView& indexView = model.bufferViews[indexAccessor.bufferView];
            const tinygltf::Buffer& indexBuffer = model.buffers[indexView.buffer];

            const void* indexData = &indexBuffer.data[indexView.byteOffset + indexAccessor.byteOffset];
            size_t indexCount = indexAccessor.count;

            std::vector<uint32_t> indices(indexCount);

            if (indexAccessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT) {
                const uint16_t* src = (const uint16_t*)indexData;
                for (size_t i = 0; i < indexCount; i++)
                    indices[i] = src[i];
            }
            else if (indexAccessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT) {
                const uint32_t* src = (const uint32_t*)indexData;
                for (size_t i = 0; i < indexCount; i++)
                    indices[i] = src[i];
            }

            //VERTICES
            std::vector<SubMeshVertex> vertices;

            const auto& posAccessor = model.accessors[primitive.attributes.at("POSITION")];
            const auto& posView = model.bufferViews[posAccessor.bufferView];
            const auto& posBuffer = model.buffers[posView.buffer];

            const float* posData = (const float*)&posBuffer.data[posView.byteOffset + posAccessor.byteOffset];

            const float* normalData = nullptr;
            const float* uvData = nullptr;

            if (primitive.attributes.count("NORMAL")) {
                const auto& acc = model.accessors[primitive.attributes.at("NORMAL")];
                const auto& view = model.bufferViews[acc.bufferView];
                const auto& buf = model.buffers[view.buffer];
                normalData = (const float*)&buf.data[view.byteOffset + acc.byteOffset];
            }

            if (primitive.attributes.count("TEXCOORD_0")) {
                const auto& acc = model.accessors[primitive.attributes.at("TEXCOORD_0")];
                const auto& view = model.bufferViews[acc.bufferView];
                const auto& buf = model.buffers[view.buffer];
                uvData = (const float*)&buf.data[view.byteOffset + acc.byteOffset];
            }

            vertices.resize(posAccessor.count);

            for (size_t i = 0; i < posAccessor.count; i++) {
                vertices[i].pos[0] = posData[i * 3 + 0];
                vertices[i].pos[1] = posData[i * 3 + 1];
                vertices[i].pos[2] = posData[i * 3 + 2];
                if (normalData) {
                    vertices[i].normal[0] = normalData[i * 3 + 0];
                    vertices[i].normal[1] = normalData[i * 3 + 1];
                    vertices[i].normal[2] = normalData[i * 3 + 2];
                }
                if (uvData) {
                    vertices[i].UV[0] = uvData[i * 2 + 0];
                    vertices[i].UV[1] = uvData[i * 2 + 1];
                }
            }
            //SUBMESH DATA SETUP
            SubMesh& newSubmesh = subMeshManager->add(SubMesh(indices.size(), vertices.size()));

            submeshes_indices.insert(submeshes_indices.end(), indices.begin(), indices.end());
            submeshes_vertices.insert(submeshes_vertices.end(), vertices.begin(), vertices.end());
            submeshes.emplace_back(newSubmesh);
            newMesh.subMeshes.emplace_back(newSubmesh.id);

            //MATERIAL AND TEXTURES
            int materialID = primitive.material;
            if (materialID < 0) newSubmesh.material_id = 0; //apply default material, if this submesh doesn't have one
            else {
                if (materials_cache.contains(materialID)) {
                    //APPLY MATERIAL IF EXISTS ALREADY
                    newSubmesh.material_id = materials_cache[materialID];
                }
                else {
                    //CREATE NEW MATERIAL
                    Material& newMaterial = materialMgr->add_material();
                    materials_cache.emplace(materialID, newMaterial.id);
                    newSubmesh.material_id = newMaterial.id;
                    //IF NEW MATERIAL IS CREATED, THEN CHECK AND CREATE MISSING TEXTURES
                    const tinygltf::Material& mat = model.materials[materialID];
                    int baseColorTextureID = mat.pbrMetallicRoughness.baseColorTexture.index;

                    auto process_texture = [&](int textureID) {
                        //IF MATERIAL HAS NO SUCH TEXTURE, THEN RETURN DEFAULT
                        if (textureID < 0 || textureID >= model.textures.size())
                            return defaultTextureID;

                        //IF TEXTURE FROM THIS MATERIAL ALREADY EXISTS, THEN RETURN ITS ID
                        if (textures_cache.contains(textureID))
                            return textures_cache[textureID];

                        //IF TEXTURE HAS NO SOURCE, THEN RETURN DEFAULT
                        const tinygltf::Texture& tex = model.textures[textureID];
                        if (tex.source < 0 || tex.source >= model.images.size())
                            return defaultTextureID;

                        //CREATE NEW TEXTURE
                        const tinygltf::Image& img = model.images[tex.source];
                        Texture* newTexture = nullptr;

                        if (img.uri.empty()) { //.GLB
                            const tinygltf::BufferView& view = model.bufferViews[img.bufferView];
                            const tinygltf::Buffer& buffer = model.buffers[view.buffer];

                            const unsigned char* data = buffer.data.data() + view.byteOffset;
                            size_t size = view.byteLength;

                            int width, height, channels;
                            unsigned char* decoded = stbi_load_from_memory(data, size, &width, &height, &channels, STBI_rgb_alpha);

                            newTexture = textureMgr->create_texture_from_data(decoded, width, height, 4, false); //4 CHANNELS BECAUSE desired channels == STBI_rgb_alpha
                            stbi_image_free(decoded);
                        }
                        else { //.GLFT
                            std::string path = baseDirectory + img.uri;
                            std::cout << "Loading texture from: " << path << std::endl;
                            newTexture = textureMgr->create_texture_from_file(path.c_str(), false);
                        }
                        textures_cache.emplace(textureID, newTexture->texture_id);
                        return newTexture->texture_id;
                    };

                    //add base color (albedo) texture of this material if not exists yet
                    newMaterial.albedoID = process_texture(baseColorTextureID);
                }
            }
        }
        return newMesh.id;
    };

    //LOAD MESHES AND SUBMESHES DATA
    for (auto& node : model.nodes) {
        if (node.mesh >= 0) {
            uint32_t newMeshID = create_mesh_from_model(model.meshes[node.mesh]);
            meshModel.mesh_nodes.emplace_back(ModelNode{});
            ModelNode& modelNode = meshModel.mesh_nodes.back();
            modelNode.mesh_id = newMeshID;
            if (node.translation.size() == 3) {
                modelNode.localPos = glm::make_vec3(node.translation.data());
            }
            if (node.scale.size() == 3) {
                modelNode.localScale = glm::make_vec3(node.scale.data());
            }
            //rotation
            meshes_count++;
            node.name;
        }
    }
    //LOAD DATA TO MESHRENDERER
    meshRenderManager->load_submeshes(submeshes_indices, submeshes_vertices, submeshes);
    //ADD A MODEL, IF IT HAS 2 OR MORE MESHES
    if (meshModel.mesh_nodes.size() > 1)
        models.add(std::move(meshModel), 0);

    //SHOW DEBUG INFO
    std::cout << "Total vertices :" << submeshes_vertices.size() << std::endl;
    std::cout << "Total meshes :" << meshes_count << std::endl;

    std::cout << "Materials list:" << std::endl;
    for (auto it : materials_cache) {
        Material& mat = MaterialManager::get_Instance()->get_material(it.second);
        std::cout << "--Material [id = " << mat.id << "]: albedoID = " << mat.albedoID << std::endl;
        Texture* tex = TextureManager::get_Instance()->get(mat.albedoID);
        std::cout << "----Texture[id = " << tex->texture_id << "]" << std::endl;
    }
    std::cout << "\n---Checking added submeshes materials and textures---" << std::endl;
    for (auto& submesh : submeshes) {
        SubMesh& sm = subMeshManager->get(submesh.id);
        std::cout << "----Submesh[id = " << sm.id << "]: Material id = " << sm.material_id << std::endl;
        Material& mat = MaterialManager::get_Instance()->get_material(sm.material_id);
        std::cout << "------Submesh Material [id = " << mat.id << "]: albedoID = " << mat.albedoID << std::endl;
    }

    for (auto& scene : model.scenes) {
        std::cout << "-Scene: " << scene.name << std::endl;
    }
    for (auto& node : model.nodes) {
        if (node.mesh >= 0) {
            std::cout << "--Node: " << node.name << ", Mesh: " << node.mesh << std::endl;
            for (auto& child_id : node.children) {
                tinygltf::Node& child = model.nodes[child_id];
                std::cout << "----Child Node: " << child.name << ", Mesh: " << child.mesh << std::endl;
            }
        }
    }
    for (auto& node : model.nodes) {
        if (!node.children.empty()) {
            std::cout << "Node: " << node.name << ", Mesh: " << node.mesh << std::endl;
            for (auto& child_id : node.children) {
                tinygltf::Node& child = model.nodes[child_id];
                std::cout << "----Child Node: " << child.name << ", Mesh: " << child.mesh << std::endl;
            }
        }
    }
}

void Engine::Models::ModelManager::delete_model(uint32_t id) {
    models.remove(id);
}

Engine::Models::Model* Engine::Models::ModelManager::get_model(uint32_t id) {
    return models.get(id);
}

Engine::Object::object_ptr Engine::Models::ModelManager::create_model_object(uint32_t model_id) {
    Model* model = models.get(model_id);
    if (!model) return Object::object_ptr(0);

    Object::ObjectManager* objectManager = Object::ObjectManager::get_Instance();
    Engine::Object::object_ptr root = objectManager->InstantiateObject();
    auto root_transform = root->add_component<Component::Transform>();

    for (auto& node : model->mesh_nodes) {
        auto mesh_object = objectManager->InstantiateObject();
        auto mesh_transform = mesh_object->add_component<Component::Transform>();
        auto mesh_comp = mesh_object->add_component<Component::MeshComponent>();
        mesh_comp->mesh_id = node.mesh_id;
        mesh_transform->translateLocal(node.localPos);
        mesh_transform->set_scale(node.localScale);
        mesh_transform->set_rotation(node.localRotation);
        root_transform->add_child(mesh_transform);
    }

    return root;
}

Mesh& Engine::Models::ModelManager::create_mesh_from_model(
    const tinygltf::Model& model,
    const tinygltf::Mesh& mesh,
    std::vector<uint32_t>& meshes_indices,
    std::vector<SubMeshVertex>& meshes_vertices,
    std::vector<SubMesh>& submeshes
) {
    Mesh& newMesh = meshManager->add_mesh(Mesh{});
    for (const auto& primitive : mesh.primitives) {
        //indices
        const tinygltf::Accessor& indexAccessor = model.accessors[primitive.indices];
        const tinygltf::BufferView& indexView = model.bufferViews[indexAccessor.bufferView];
        const tinygltf::Buffer& indexBuffer = model.buffers[indexView.buffer];

        const void* indexData = &indexBuffer.data[indexView.byteOffset + indexAccessor.byteOffset];
        size_t indexCount = indexAccessor.count;

        std::vector<uint32_t> indices(indexCount);

        if (indexAccessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT)
        {
            const uint16_t* src = (const uint16_t*)indexData;
            for (size_t i = 0; i < indexCount; i++)
                indices[i] = src[i];
        }
        else if (indexAccessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT)
        {
            const uint32_t* src = (const uint32_t*)indexData;
            for (size_t i = 0; i < indexCount; i++)
                indices[i] = src[i];
        }

        //vertices
        std::vector<SubMeshVertex> vertices;

        const auto& posAccessor = model.accessors[primitive.attributes.at("POSITION")];
        const auto& posView = model.bufferViews[posAccessor.bufferView];
        const auto& posBuffer = model.buffers[posView.buffer];

        const float* posData = (const float*)&posBuffer.data[posView.byteOffset + posAccessor.byteOffset];

        const float* normalData = nullptr;
        const float* uvData = nullptr;

        if (primitive.attributes.count("NORMAL"))
        {
            const auto& acc = model.accessors[primitive.attributes.at("NORMAL")];
            const auto& view = model.bufferViews[acc.bufferView];
            const auto& buf = model.buffers[view.buffer];
            normalData = (const float*)&buf.data[view.byteOffset + acc.byteOffset];
        }

        if (primitive.attributes.count("TEXCOORD_0"))
        {
            const auto& acc = model.accessors[primitive.attributes.at("TEXCOORD_0")];
            const auto& view = model.bufferViews[acc.bufferView];
            const auto& buf = model.buffers[view.buffer];
            uvData = (const float*)&buf.data[view.byteOffset + acc.byteOffset];
        }

        vertices.resize(posAccessor.count);

        for (size_t i = 0; i < posAccessor.count; i++)
        {
            vertices[i].pos[0] = posData[i * 3 + 0];
            vertices[i].pos[1] = posData[i * 3 + 1];
            vertices[i].pos[2] = posData[i * 3 + 2];

            if (normalData)
            {
                vertices[i].normal[0] = normalData[i * 3 + 0];
                vertices[i].normal[1] = normalData[i * 3 + 1];
                vertices[i].normal[2] = normalData[i * 3 + 2];
            }

            if (uvData)
            {
                vertices[i].UV[0] = uvData[i * 2 + 0];
                vertices[i].UV[1] = uvData[i * 2 + 1];
            }
        }
        //submesh data setup
        SubMesh submesh;
        submesh.vertexCount = vertices.size();
        submesh.indexCount = indices.size();
        SubMesh& newSubmesh = subMeshManager->add(std::move(submesh));
        
        meshes_indices.insert(meshes_indices.end(), indices.begin(), indices.end());
        meshes_vertices.insert(meshes_vertices.end(), vertices.begin(), vertices.end());
        submeshes.emplace_back(newSubmesh);
        newMesh.subMeshes.emplace_back(newSubmesh.id);

        //material and textures
        int materialID = primitive.material;
        if (materialID < 0) newSubmesh.material_id = 0; //default
        else {
            
        }
    }
    return newMesh;
}