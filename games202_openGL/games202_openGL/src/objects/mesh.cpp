#include "mesh.h"
#include <vector>


Mesh::Mesh()
{
    genCube();
    setupMesh();
}


Mesh::Mesh(const vector<Vertex>& _vertices, const vector<unsigned int>& _indices, const unsigned int& ID)
{
    vertices = _vertices;
    indices = _indices;
    toMaterial_ID = ID;
    setupMesh();
}

int Mesh::getVertexCount()
{
    return vertices.size();
}

unsigned int Mesh::getMeshToMateria()
{
    return toMaterial_ID;
}

void Mesh::genCube()
{
    const vector<glm::vec3> positions = {
         glm::vec3(-0.5, -0.5, 0.5),
    glm::vec3(0.5, -0.5, 0.5),
    glm::vec3(0.5, 0.5, 0.5),
    glm::vec3(-0.5, 0.5, 0.5),

    glm::vec3(-0.5, -0.5, -0.5),
    glm::vec3(-0.5, 0.5, -0.5),
    glm::vec3(0.5, 0.5, -0.5),
    glm::vec3(0.5, -0.5, -0.5),

    glm::vec3(-0.5, 0.5, -0.5),
    glm::vec3(-0.5, 0.5, 0.5),
    glm::vec3(0.5, 0.5, 0.5),
    glm::vec3(0.5, 0.5, -0.5),

    glm::vec3(-0.5, -0.5, -0.5),
    glm::vec3(0.5, -0.5, -0.5),
    glm::vec3(0.5, -0.5, 0.5),
    glm::vec3(-0.5, -0.5, 0.5),

    glm::vec3(0.5, -0.5, -0.5),
    glm::vec3(0.5, 0.5, -0.5),
    glm::vec3(0.5, 0.5, 0.5),
    glm::vec3(0.5, -0.5, 0.5),

    glm::vec3(-0.5, -0.5, -0.5),
    glm::vec3(-0.5, -0.5, 0.5),
    glm::vec3(-0.5, 0.5, 0.5),
    glm::vec3(-0.5, 0.5, -0.5),
    };

    const vector<unsigned int> _indices = {
        0, 1, 2, 0, 2, 3, // front
        4, 5, 6, 4, 6, 7, // back
        8, 9, 10, 8, 10, 11, // top
        12, 13, 14, 12, 14, 15, // bottom
        16, 17, 18, 16, 18, 19, // right
        20, 21, 22, 20, 22, 23, // left
    };

    for (auto vec : positions) {
        Vertex vertex;
        vertex.Position = vec;
        vertex.Normal = glm::vec3();
        vertex.TexCoords = glm::vec3();
        vertices.push_back(vertex);
    }

    indices = _indices;
}

void Mesh::setupMesh()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
        &indices[0], GL_STATIC_DRAW);

    // 顶点位置
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // 顶点法线
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    // 顶点纹理坐标
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

    glBindVertexArray(0);
}


void Mesh::draw(const Shader& shader)
{
    // 绘制网格
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}