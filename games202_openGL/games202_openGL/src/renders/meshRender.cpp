#include "meshRender.h"

MeshRender::MeshRender()
{
    // 默认构造
}

MeshRender::MeshRender(const Pipeline &_pipeline)
{
    pipeline = _pipeline;
}

void MeshRender::setBufferData(const Mesh &_mesh)
{
    // 完成两个缓冲区对象的创建加赋值
    unsigned int _VBO, _EBO;
    glGenBuffers(1, &_VBO);
    glGenBuffers(1, &_EBO);

    glBindBuffer(GL_ARRAY_BUFFER, _VBO); 
    glBufferData(GL_ARRAY_BUFFER, _mesh.vertices.size() * sizeof(Vertex), &_mesh.vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, _mesh.indices.size() * sizeof(unsigned int), &_mesh.indices[0], GL_STATIC_DRAW);

    VBO = _VBO;
    EBO = _EBO;
    indices_size = _mesh.indices.size();
}

void MeshRender::bindAttributeData()
{
    VAO = pipeline.setVAO(VBO, EBO);
}

void MeshRender::bindUniform_mvp(const glm::mat4 &model, const glm::mat4 &view, const glm::mat4 &projection)
{
    pipeline.setMat4("uModelViewMatrix", view * model); // 传入mv一起作用的矩阵
    pipeline.setMat4("uProjectionMatrix", projection);
}

void MeshRender::bindUniform_camera(const Camera &camera)
{
    pipeline.setVec3("uCameraPos", camera.position);
}

void MeshRender::bindUniform_light(const vector<Light> &lights)
{
    pipeline.setVec3("uLightPos", lights[0].position);
    pipeline.setFloat("uLightIntensity", lights[0].intensity);
}

void MeshRender::bindUnifrom_lightPos(const glm::vec3 &_position)
{
    pipeline.setVec3("uLightPos", _position);
}

void MeshRender::bindUniform_material(const Material &_material)
{
   pipeline.setVec3("uKd", _material.kd);
   pipeline.setVec3("uKs", _material.ks);
}

void MeshRender::bindTexture(const Material &_material)
{
    for (int i=0; i<_material.textures.size(); i++){
        pipeline.setTexture(_material.textures[i].type, _material.textures[i].id);
    }
}

void MeshRender::bindTextureSample(const int &_t)
{  
    pipeline.setInt("uTextureSample", _t);
}

void MeshRender::use()
{
    glUseProgram(pipeline.id);
}

void MeshRender::draw()
{  
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices_size), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glActiveTexture(GL_TEXTURE0);
}

void MeshRender::freeData()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}
