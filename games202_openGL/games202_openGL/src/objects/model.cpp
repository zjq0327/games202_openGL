#include "model.h"

#include <iostream>
#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace std;

unsigned int TextureFromFile(const char* path, const string& directory);

Model::Model(const string& path)
{
    loadModel(path);
    isTexture = true;
}

int Model::getMeshCount()
{
    return meshes.size();
}

int Model::getMaterialCount()
{
    return materials.size();
}

void Model::loadModel(const string& path)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
    // check for errors
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
    {
        cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
        return ;
    }
    // retrieve the directory path of the filepath
    directory = path.substr(0, path.find_last_of('/'));

    // process ASSIMP's root node recursively
    processNode(scene->mRootNode, scene);

    return;
}

void Model::processNode(aiNode* _node, const aiScene* _scene)
{
    // cout << "Ԥ�ƿ������" << _node->mNumMeshes << "��mesh" << endl;
    for (unsigned int i = 0; i < _node->mNumMeshes; i++)
    {
        // cout << "���һ��mesh" << endl;
        aiMesh* mesh = _scene->mMeshes[_node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, _scene));
        // processMaterial(mesh, _scene); ��һ���Ż����ڴ���Mesh���������
    }

    for (unsigned int i = 0; i < _node->mNumChildren; i++)
    {
        processNode(_node->mChildren[i], _scene);
    }
}

Mesh Model::processMesh(aiMesh* _mesh, const aiScene* _scene)
{
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    for (unsigned int i = 0; i < _mesh->mNumVertices; i++) {
        // ����mesh���µ����ж�������
        Vertex vertex;
        glm::vec3 vector;
        // λ��
        vector.x = _mesh->mVertices[i].x;
        vector.y = _mesh->mVertices[i].y;
        vector.z = _mesh->mVertices[i].z;
        vertex.Position = vector;
        // ������
        if (_mesh->HasNormals()) {
            vector.x = _mesh->mNormals[i].x;
            vector.y = _mesh->mNormals[i].y;
            vector.z = _mesh->mNormals[i].z;
            vertex.Normal = vector;
        }
        else {
            vertex.Normal = glm::vec3();
        }
        // ��������
        // mesh��Ӧ���ǿ��Ա��������������� ����Ŀǰ ����ֻ���Ǵ���һ�� ����һ�������������õ�����������
        // ��������������ٻ����޸ı���
        if (_mesh->mTextureCoords[0]) {
            glm::vec2 vec;
            vec.x = _mesh->mTextureCoords[0][i].x;
            vec.y = _mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;
        }
        else {
            vertex.TexCoords = glm::vec2();
        }

        // cout << "vertex_postion:" << vertex.Position.x << " " << vertex.Position.y << " " << vertex.Position.z  << endl;
        // cout << "vertex_normal:" << vertex.Normal.x << " " << vertex.Normal.y << " " << vertex.Normal.z  << endl;
        // cout << "vertex_texcoord:" << vertex.TexCoords.x << " " << vertex.TexCoords.y << " " <<  endl;

        vertices.push_back(vertex);

    }

    // ��������
    for (unsigned int i = 0; i < _mesh->mNumFaces; i++) {
        aiFace face = _mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    // cout << "indiecs_size:" << indices.size() << endl;

    // �ҵ���Mesh��Ӧ�Ĳ������ID
    unsigned int id = processMaterial(_mesh, _scene);

    return Mesh(vertices, indices, id);
}

unsigned int Model::processMaterial(aiMesh* _mesh, const aiScene* _scene)
{
    // CMaterial material(PHONG); // Ĭ�϶�ȡ��ģ�Ͳ��� ȫ������PHONG���������������� �����Ӹ�type����
    aiMaterial* aiMaterial = _scene->mMaterials[_mesh->mMaterialIndex];
    vector<Texture> textures;

    // ��ȡ��������
    glm::vec3 _ka, _kd, _ks;
    glm::vec3 color;
    aiMaterial->Get(AI_MATKEY_COLOR_AMBIENT, color);
    _ka = glm::vec3(color.r, color.g, color.b);
    aiMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, color);
    _kd = glm::vec3(color.r, color.g, color.b);
    aiMaterial->Get(AI_MATKEY_COLOR_SPECULAR, color);
    _ks = glm::vec3(color.r, color.g, color.b);

    Material tmaterial(PHONG, _ka, _kd, _ks); // Ĭ�϶�ȡ��ģ�Ͳ��� ȫ������PHONG���������������� �����Ӹ�type����

    // ��ȡ������ͼ���� Ŀǰֻ���Ƕ�ȡ��������ͼ
    // �����䲿��
    vector<Texture> diffuseMaps = loadMaterialTextures(aiMaterial, aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

    tmaterial.setTextures(textures);

    unsigned int tid = 0;

    // ���뵽��������֮ǰ����Ҫ�ж��Ƿ��ظ���Ӳ���
    if (materials.size() == 0) {
        materials.push_back(tmaterial);
        tid = 0;
    }
    else {
        for (int i = 0; i < materials.size(); i++) {
            if (!tmaterial.compare(materials[i])) {
                // ��ǰ������model�ڴ洢�������һ��
                materials.push_back(tmaterial);
                tid = materials.size() - 1;
            }
            else {
                // ��ǰ������model���Ѿ����ڣ����ü����ж�
                tid = i;
                break;
            }
        }
    }

    return tid;
}

vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName)
{
    // ��aasimp--mesh��Ĳ��ʲ��� ������ͼ��·������ȫ��תΪ����ṹ�巵��
    vector<Texture> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
        aiString str;
        mat->GetTexture(type, i, &str);
        bool skip = false;
        for (unsigned int j = 0; j < textures_loaded.size(); j++) {
            if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0) {
                textures.push_back(textures_loaded[j]);
                skip = true;
                break;
            }
        }
        if (!skip) {
            Texture texture;
            texture.id = TextureFromFile(str.C_Str(), this->directory);
            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
            textures_loaded.push_back(texture);
        }
    }
    return textures;
}

unsigned int TextureFromFile(const char* path, const string& directory)
{
    // ��ȡͼƬ ����assimp�õ�������·��
    string filename = string(path);
    filename = directory + '/' + filename;

    unsigned int textureID;
    glGenTextures(1, &textureID); // ���������Ķ��� �õ�ID

    int width, height, nrComponents;
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0); // ����ͼƬ��
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        // ������󶨵�ָ��״̬���� ����״̬�޸Ľӿ� ������״̬��ֵ ��ɴ����Ķ���ֵ���� 
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D); // ����mipmap ��������ӳ������

        // һЩ����״̬������st���������Լ���С����͹�������Ĵ����� ����ǳ���Ĭ�ϵ�����
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data); // �ͷ�ͼƬ��Դ ��Ϊ�Ѿ��󶨵�������������������
    }
    else
    {
        std::cout << "The texture was not imported correctly, the problem is in the loadMaterialTextures section" << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}


void Model::draw(const Shader& shader)
{
    for (int i = 0; i < meshes.size(); i++)
    {
        meshes[i].draw(shader);
    }
}
