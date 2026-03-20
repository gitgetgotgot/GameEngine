#include "MeshManager.h"

void MeshManager::init_basic_meshes() {
	meshes.add(Mesh{}, 0);
    meshes.add(Mesh{}, 1);
    meshes.add(Mesh{}, 2);
    meshes.add(Mesh{}, 3);

    //Plane
    Mesh* mesh = meshes.get(0);
    mesh->id = 0;
    mesh->vertices = {
        // Front face (z = +0.5)
        { { -0.5f, 0.0f,  0.5f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f }, 0.f },
        { { -0.5f, 0.0f, -0.5f }, { 0.f, 1.f, 0.f }, { 0.f, 1.f }, 0.f },
        { {  0.5f, 0.0f, -0.5f }, { 0.f, 1.f, 0.f }, { 1.f, 1.f }, 0.f },
        { {  0.5f, 0.0f,  0.5f }, { 0.f, 1.f, 0.f }, { 1.f, 0.f }, 0.f }
    };
    mesh->indices = {
        0, 1, 2,    0, 2, 3
    };

    //Cube
	mesh = meshes.get(1);
	mesh->id = 1;
	mesh->vertices = {
        // Front face (z = +0.5)
        { { -0.5f, -0.5f,  0.5f }, { 0.f, 0.f, 1.f }, { 0.f, 0.f }, 0.f }, // BL
        { { -0.5f,  0.5f,  0.5f }, { 0.f, 0.f, 1.f }, { 0.f, 1.f }, 0.f }, // TL
        { {  0.5f,  0.5f,  0.5f }, { 0.f, 0.f, 1.f }, { 1.f, 1.f }, 0.f }, // TR
        { {  0.5f, -0.5f,  0.5f }, { 0.f, 0.f, 1.f }, { 1.f, 0.f }, 0.f }, // BR
        // Back face (z = -0.5)
        { {  0.5f, -0.5f, -0.5f }, { 0.f, 0.f, -1.f }, { 0.f, 0.f }, 0.f }, // BR
        { {  0.5f,  0.5f, -0.5f }, { 0.f, 0.f, -1.f }, { 0.f, 1.f }, 0.f }, // TR
        { { -0.5f,  0.5f, -0.5f }, { 0.f, 0.f, -1.f }, { 1.f, 1.f }, 0.f }, // TL
        { { -0.5f, -0.5f, -0.5f }, { 0.f, 0.f, -1.f }, { 1.f, 0.f }, 0.f }, // BL
        // Left face (x = -0.5)
        { { -0.5f, -0.5f, -0.5f }, { -1.f, 0.f, 0.f }, { 0.f, 0.f }, 0.f }, // BL
        { { -0.5f,  0.5f, -0.5f }, { -1.f, 0.f, 0.f }, { 0.f, 1.f }, 0.f }, // TL
        { { -0.5f,  0.5f,  0.5f }, { -1.f, 0.f, 0.f }, { 1.f, 1.f }, 0.f }, // TR
        { { -0.5f, -0.5f,  0.5f }, { -1.f, 0.f, 0.f }, { 1.f, 0.f }, 0.f }, // BR
        // Right face (x = +0.5)
        { {  0.5f, -0.5f,  0.5f }, { 1.f, 0.f, 0.f }, { 0.f, 0.f }, 0.f }, // BL
        { {  0.5f,  0.5f,  0.5f }, { 1.f, 0.f, 0.f }, { 0.f, 1.f }, 0.f }, // TL
        { {  0.5f,  0.5f, -0.5f }, { 1.f, 0.f, 0.f }, { 1.f, 1.f }, 0.f }, // TR
        { {  0.5f, -0.5f, -0.5f }, { 1.f, 0.f, 0.f }, { 1.f, 0.f }, 0.f }, // BR
        // Top face (y = +0.5)
        { { -0.5f,  0.5f,  0.5f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f }, 0.f }, // BL
        { { -0.5f,  0.5f, -0.5f }, { 0.f, 1.f, 0.f }, { 0.f, 1.f }, 0.f }, // TL
        { {  0.5f,  0.5f, -0.5f }, { 0.f, 1.f, 0.f }, { 1.f, 1.f }, 0.f }, // TR
        { {  0.5f,  0.5f,  0.5f }, { 0.f, 1.f, 0.f }, { 1.f, 0.f }, 0.f }, // BR
        // Bottom face (y = -0.5)
        { { -0.5f, -0.5f, -0.5f }, { 0.f, -1.f, 0.f }, { 0.f, 0.f }, 0.f }, // BL
        { {  0.5f, -0.5f, -0.5f }, { 0.f, -1.f, 0.f }, { 1.f, 0.f }, 0.f }, // BR
        { {  0.5f, -0.5f,  0.5f }, { 0.f, -1.f, 0.f }, { 1.f, 1.f }, 0.f }, // TR
        { { -0.5f, -0.5f,  0.5f }, { 0.f, -1.f, 0.f }, { 0.f, 1.f }, 0.f }  // TL
    };
    mesh->indices.resize(36);
    uint32_t faces = 6;
    for (int i = 0; i < faces; i++) {
        size_t face_index = i * 6;
        mesh->indices[face_index] = mesh->indices[face_index + 3] = i * 4;
        mesh->indices[face_index + 2] = mesh->indices[face_index + 4] = mesh->indices[face_index] + 2;
        mesh->indices[face_index + 1] = mesh->indices[face_index + 2] - 1;
        mesh->indices[face_index + 5] = mesh->indices[face_index + 2] + 1;
    }

    //Pyramid
    mesh = meshes.get(2);
    mesh->id = 2;
    mesh->vertices = {
        // Base (square, z = -0.5, CW order)
        { { -0.5f, -0.5f, -0.5f }, { 0.f, 0.f, -1.f }, { 0.f, 0.f }, 0.f }, // BL
        { {  0.5f, -0.5f, -0.5f }, { 0.f, 0.f, -1.f }, { 1.f, 0.f }, 0.f }, // BR
        { {  0.5f,  0.5f, -0.5f }, { 0.f, 0.f, -1.f }, { 1.f, 1.f }, 0.f }, // TR
        { { -0.5f,  0.5f, -0.5f }, { 0.f, 0.f, -1.f }, { 0.f, 1.f }, 0.f }, // TL
        // Front face (apex + bottom edge)
        { { -0.5f, -0.5f, -0.5f }, { 0.f, -0.5f, 0.5f }, { 0.f, 0.f }, 0.f }, // BL
        { {  0.5f, -0.5f, -0.5f }, { 0.f, -0.5f, 0.5f }, { 1.f, 0.f }, 0.f }, // BR
        { {  0.0f,  0.0f,  0.5f }, { 0.f, -0.5f, 0.5f }, { 0.5f, 1.f }, 0.f }, // Apex
        // Right face
        { {  0.5f, -0.5f, -0.5f }, { 0.5f, 0.f, 0.5f }, { 0.f, 0.f }, 0.f }, // BR
        { {  0.5f,  0.5f, -0.5f }, { 0.5f, 0.f, 0.5f }, { 1.f, 0.f }, 0.f }, // TR
        { {  0.0f,  0.0f,  0.5f }, { 0.5f, 0.f, 0.5f }, { 0.5f, 1.f }, 0.f }, // Apex
        // Back face
        { {  0.5f,  0.5f, -0.5f }, { 0.f, 0.5f, 0.5f }, { 0.f, 0.f }, 0.f }, // TR
        { { -0.5f,  0.5f, -0.5f }, { 0.f, 0.5f, 0.5f }, { 1.f, 0.f }, 0.f }, // TL
        { {  0.0f,  0.0f,  0.5f }, { 0.f, 0.5f, 0.5f }, { 0.5f, 1.f }, 0.f }, // Apex
        // Left face
        { { -0.5f,  0.5f, -0.5f }, { -0.5f, 0.f, 0.5f }, { 0.f, 0.f }, 0.f }, // TL
        { { -0.5f, -0.5f, -0.5f }, { -0.5f, 0.f, 0.5f }, { 1.f, 0.f }, 0.f }, // BL
        { {  0.0f,  0.0f,  0.5f }, { -0.5f, 0.f, 0.5f }, { 0.5f, 1.f }, 0.f } // Apex
    };
    mesh->indices = {
        0, 1, 2,    0, 2, 3,
        4, 5, 6,
        7, 8, 9,
        10, 11, 12,
        13, 14, 15
    };

    //Cylinder
    mesh = meshes.get(3);
    mesh->id = 3;
    int N = 20;
    float M_PI = 3.14159265359;
    std::vector<MeshVertex>& vertices = mesh->vertices;
    std::vector<uint32_t>& indices = mesh->indices;
    // Верхнее основание
    vertices.push_back({ {0.f, 0.5f, 0.f}, {0.f, 1.f, 0.f}, {0.5f, 0.5f}, 0.f }); // центр
    for (int i = 0; i < N; i++) {
        float angle = 2.0f * M_PI * i / N;
        float x = 0.5f * cos(angle);
        float z = 0.5f * sin(angle);
        vertices.push_back({ {x, 0.5f, z}, {0.f, 1.f, 0.f}, { (x + 0.5f), (z + 0.5f) }, 0.f });
    }
    // Нижнее основание
    vertices.push_back({ {0.f, -0.5f, 0.f}, {0.f, -1.f, 0.f}, {0.5f, 0.5f}, 0.f }); // центр
    for (int i = 0; i < N; i++) {
        float angle = 2.0f * M_PI * i / N;
        float x = 0.5f * cos(angle);
        float z = 0.5f * sin(angle);
        vertices.push_back({ {x, -0.5f, z}, {0.f, -1.f, 0.f}, { (x + 0.5f), (z + 0.5f) }, 0.f });
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
}

void MeshManager::load_mesh() {
	//
}

void MeshManager::load_meshes() {
	//
}

Mesh* MeshManager::get_mesh(uint32_t id) {
	return meshes.get(id);
}

void MeshManager::delete_mesh(uint32_t id) {
	meshes.remove(id);
}
