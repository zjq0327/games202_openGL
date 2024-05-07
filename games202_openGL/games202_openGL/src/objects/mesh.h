#pragma once
#include"glframework/core.h"
#include "glframework/shader.h"
#include <vector>

using namespace std;

struct Vertex {
    // position
    glm::vec3 Position;
    // normal
    glm::vec3 Normal;
    // texCoords
    glm::vec2 TexCoords;
};


class Mesh
{
public:
    // ��������
    vector<Vertex> vertices;
    // ���ͼԪ�����
    vector<unsigned int> indices;
    // ��Ӧ�����ڵ����ֲ��� ��mesh
    unsigned int toMaterial_ID;

    Mesh(); // ��Ҫһ��Ĭ�Ϲ��죬��������һ��������

    Mesh(const vector<Vertex>& _vertices, const vector<unsigned int>& _indices, const unsigned int& ID); // ���캯�� ����������Ա������ֵ

    int getVertexCount(); // �õ��������

    unsigned int getMeshToMateria(); // �õ���Ӧ�����ֲ������

    void draw(const Shader& shader);

private:
    void genCube(); // ����һ��Ĭ��cube 
    void setupMesh();

    unsigned int VAO, VBO, EBO;

};



