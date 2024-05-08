#include "sceneRender.h"

SceneRender::SceneRender()
{
    // 默认构造
}

void SceneRender::addModel(const string &path)
{
    models.push_back(Model(path)); 
}

void SceneRender::addLight(float _intensity, glm::vec3 _position, glm::vec3 _color)
{
    lights.push_back(Light(_intensity, _position, _color));
}

void SceneRender::addCamera(const glm::vec3 &_position, const glm::vec3 &_target, const glm::vec3 &_lookup)
{
    camera = new Camera(_position, _target, _lookup);
}

void SceneRender::addCamera(Camera* _camera)
{
    camera = _camera;
}

unsigned int SceneRender::addPipeline(const Pipeline &_pipelien)
{
    pipelines.push_back(_pipelien);
    return pipelines.size()-1;
}

glm::mat4 SceneRender::getModelMatrix(const glm::vec3 &_translate, float _r, const glm::vec3 &_rotate, const glm::vec3 &_scale)
{
    glm::mat4 _result(1.0f);
    _result = glm::translate(_result, _translate);
    _result = glm::rotate(_result, _r, _rotate);
    _result = glm::scale(_result, _scale);
    return _result;
}

glm::mat4 SceneRender::getViewMatrix(unsigned int camera_id)
{
    glm::mat4 _result;
    camera->getViewMatrix(_result);
    return _result;

}

glm::mat4 SceneRender::getProjectionMatrix(float _r, float _ratio, float _near, float _far)
{
    return glm::perspective(glm::radians(_r), _ratio, _near, _far);
}

void SceneRender::setModelMatrixLight(const glm::mat4 &_m)
{
    modelMatrix_light = _m;
}

void SceneRender::setModelMatrixModel(const glm::mat4 &_m)
{
    modelMatrix_model = _m;
}

void SceneRender::setViewMatrix(const glm::mat4 &_m)
{
    viewMatrix = _m;
}

void SceneRender::setProjectionMatrix(const glm::mat4 &_m)
{
    projectionMatrix = _m;
}

void SceneRender::draw(unsigned int light_pipe_id, unsigned int Phong_pipe_id)
{
    // 启动深度缓冲
    glEnable(GL_DEPTH_TEST);
    // 遍历光源 渲染光源
    for (int i=0; i<lights.size(); i++){
        lightRendering(i, light_pipe_id);
    }
    // 遍历model 渲染model
    for (int i=0; i<models.size(); i++){
        modelRendering(i, Phong_pipe_id);
    }
}

void SceneRender::modelRendering(unsigned int model_id, unsigned int pipeline_id)
{
    // 整个model渲染过程在这里完成 调用其他需要的接口实现功能
    bool isTexture = models[model_id].isTexture;
    for (Mesh mesh : models[model_id].meshes){
        // 以mesh为基础渲染
        MeshRender meshRender(pipelines[pipeline_id]);
        Material material = models[model_id].materials[mesh.getMeshToMateria()];

        // 加载数据到缓冲区
        meshRender.setBufferData(mesh);
        // 设置当前program
        meshRender.use();
        // 绑定顶点数据到着色器
        meshRender.bindAttributeData();
        // 绑定各种unifrom数据
        meshRender.bindUniform_mvp(modelMatrix_model, viewMatrix, projectionMatrix); // MVP
        meshRender.bindUniform_camera(*camera); // 相机 默认首位相机
        meshRender.bindUniform_light(lights); // 目前 meshRender还只赋值一个光源属性
        meshRender.bindUniform_material(material); // 材质属性
        // 绑定贴图纹理数据
        if (isTexture){
            meshRender.bindTextureSample(1);
        }
        meshRender.bindTexture(material);

        // 启动绘制
        meshRender.draw();

        // 释放资源
        meshRender.freeData();

    }
}

void SceneRender::lightRendering(unsigned int light_id, unsigned int pipeline_id)
{
    // 具体过程可以参考modelRendering
    LightRender lightRender(pipelines[pipeline_id]);
    lightRender.setBufferData(lights[light_id].mesh);
    lightRender.use();
    lightRender.bindAttributeData();
    lightRender.bindUniform_mvp(modelMatrix_light, viewMatrix, projectionMatrix);
    lightRender.bindUniform_light(lights[light_id]);

    lightRender.draw();    

    lightRender.freeData();

}
