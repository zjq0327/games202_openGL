#ifndef MYWORK_MESHRENDER_H
#define MYWORK_MESHRENDER_H


#include"glframework/pipeline.h"
#include "objects/mesh.h"
#include "objects/light.h"
#include "camera/camera.h"
class MeshRender
{
    public:
    // 三个上下文里对象的id
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;

    unsigned int indices_size;

    // CMesh mesh;
    // CMaterial material; // 不想重复储存内容 浪费空间
    Pipeline pipeline;

    MeshRender(); // 默认构造 
    MeshRender(const Pipeline &_pipeline); // 赋值初始化

    void setBufferData(const Mesh &_mesh); // 数据加载到缓冲区 返回上下文缓冲区的id

    void bindAttributeData(); // 绑定顶点数据

    void bindUniform_mvp(const glm::mat4 &model, const glm::mat4 &view, const glm::mat4 &projection); // 绑定uniform mvp矩阵
    void bindUniform_mvpp(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection); // 分别绑定三个矩阵
    void bindUniform_lightMvp(const glm::mat4& mvp); // 绑定光源位置下的mvp矩阵 用于作业1 实现阴影

    void bindUniform_camera(const Camera &camera); // 绑定摄像机相关属性

    void bindUniform_light(const vector<Light> &lights); // 绑定光源类相关属性 存在多光源的情况
    void bindUnifrom_lightPos(const glm::vec3 &_position); // 单独更改着色器里光源位置变量 中间不断改变光源位置 不一定要时刻修改初始光源类信息

    void bindUniform_material(const Material &_material); // 绑定材质相关属性 默认是Phong材质的 后面也可以考虑加上PBR

    void bindTexture(const Material &_material); // 绑定贴图
    void bindShadowMap(unsigned int _id); // 绑定shadowmap 用于作业1 实现阴影


    void bindTextureSample(const int &_t);

    void use();

    void draw(); // 渲染启动接口

    void freeData(); // 释放资源 缓冲区资源 不然爆空间
};

#endif