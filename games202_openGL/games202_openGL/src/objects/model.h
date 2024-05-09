#pragma once

#include "mesh.h"
#include "materials/material.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>


using namespace std;

class Model
{
public:
    // ����
    vector<Mesh> meshes;
    // ����
    vector<Material> materials;
    // ������õ���
    string directory;
    // ��ֹ����������������ظ�����
    vector<Texture> textures_loaded;;

    bool isTexture;
    // �����������model�任
    glm::mat4 modelMatirx;

    Model(const string& path); // ���캯�� ����ָ���ļ�path �����ڲ���������

    void draw(const Shader& shader); // ��Ⱦ�����ӿڣ���Ⱦ�ڲ����е�meshes����Ҫ����һ���������ȥ

    int getMeshCount(); // �õ�Mesh������

    int getMaterialCount(); // �õ����ʵ�����


    void setModelMatrix(const glm::mat4& _modelMatrix);

private:
    void loadModel(const string& path); // �ڲ��������������ģ�����ݵ��ڲ���Աת�� ����assimp��ʵ��,����boolȷ���Ƿ���ɹ�

    void processNode(aiNode* _node, const aiScene* _scene); // ����assimp�ڵ� �ݹ������״�ṹ
    Mesh processMesh(aiMesh* _mesh, const aiScene* _scene); // �������ڵ��ڵ�mesh����
    unsigned int processMaterial(aiMesh* _mesh, const aiScene* _scene); // �������ڵ�Ĳ��ʲ��֣����ʱ�����assimp--mesh���
    vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName); // ���ڼ���assimp--mat�����ͼ��Ϣ ����ȡ����·��strתΪ�������ﴴ��������id

};
