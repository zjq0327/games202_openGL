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
    // 网格
    vector<Mesh> meshes;
    // 材质
    vector<Material> materials;
    // 后面会用到的
    string directory;
    // 防止上下文中纹理对象重复加载
    vector<Texture> textures_loaded;;

    bool isTexture;
    // 该物体特殊的model变换
    glm::mat4 modelMatirx;

    Model(const string& path); // 构造函数 传入指定文件path 调动内部函数构造

    void draw(const Shader& shader); // 渲染启动接口，渲染内部所有的meshes，需要传递一个管线类进去

    int getMeshCount(); // 得到Mesh的数量

    int getMaterialCount(); // 得到材质的数量


    void setModelMatrix(const glm::mat4& _modelMatrix);

private:
    void loadModel(const string& path); // 内部函数，用于完成模型数据到内部成员转化 调用assimp库实现,返回bool确定是否导入成功

    void processNode(aiNode* _node, const aiScene* _scene); // 处理assimp节点 递归遍历树状结构
    Mesh processMesh(aiMesh* _mesh, const aiScene* _scene); // 处理单个节点内的mesh部分
    unsigned int processMaterial(aiMesh* _mesh, const aiScene* _scene); // 处理单个节点的材质部分，材质保存在assimp--mesh里的
    vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName); // 用于加载assimp--mat里的贴图信息 将获取到的路径str转为上下文里创建对象后的id

};
