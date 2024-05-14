#include "pipeline.h"

#include "objects/mesh.h"

Pipeline::Pipeline()
{
    // 默认构造
}

Pipeline::Pipeline(const Shader &_vertexShader, const Shader &_fragmentShader)
{
    unsigned int _id = glCreateProgram();
    id = _id;
    vertexShader = _vertexShader;
    fragmentShader = _fragmentShader;
    glAttachShader(_id, _vertexShader.id);
    glAttachShader(_id, _fragmentShader.id);
    glLinkProgram(_id);

    texture_id = 0;

    // 检测是否创建成功
    int success;
    char infoLog[1024];
    glGetProgramiv(_id, GL_LINK_STATUS, &success);
    if(!success){
        glGetProgramInfoLog(_id, 1024, NULL, infoLog);
        std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << "program" << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
    }

    glDeleteShader(_vertexShader.id);
    glDeleteShader(_fragmentShader.id);
}

bool Pipeline::judgeNameIn(const string &_name, const string &_type)
{
    // 这个函数是废案 原本是想增加安全度 然后发现逻辑错误了
    // 因为如果在meshRender加入传入name指定绑定数据的接口的话 这就和pipeline一样了
    // meshRender里可以一口气完成指定类型的数据绑定 没有时间传入名字
    // 如果要实现的话 需要给meshRender里传入一个姓名数组 
    // 比如mvp绑定函数距离，额外传入一个名称数组，然后依次调用pipeline的setMat4实现绑定
    // 更完整的话 要实现名称数组和类型数组的对应 相当于要传一个mat 要额外增加很多的数据存储
    
    
    return false;
}

void Pipeline::setInt(const std::string &name, int value)
{
    GLint location = glGetUniformLocation(id, name.c_str());
    if (location == -1) {
        std::cerr << "Error: Uniform variable '" << name.c_str() << "' not found in shader." << std::endl;
    }

    glUniform1i(location, value);
}

void Pipeline::setFloat(const std::string &name, float value)
{
    GLint location = glGetUniformLocation(id, name.c_str());
    if (location == -1) {
        std::cerr << "Error: Uniform variable '" << name.c_str() << "' not found in shader." << std::endl;
    }
    glUniform1f(location, value);
}

void Pipeline::setVec2(const std::string &name, const glm::vec2 &value)
{
    GLint location = glGetUniformLocation(id, name.c_str());
    if (location == -1) {
        std::cerr << "Error: Uniform variable '" << name.c_str() << "' not found in shader." << std::endl;
    }
    glUniform2fv(location, 1, &value[0]);
}

void Pipeline::setVec2(const std::string &name, float x, float y)
{
    GLint location = glGetUniformLocation(id, name.c_str());
    if (location == -1) {
        std::cerr << "Error: Uniform variable '" << name.c_str() << "' not found in shader." << std::endl;
    }
    glUniform2f(location, x, y);
}

void Pipeline::setVec3(const std::string &name, const glm::vec3 &value)
{
    GLint location = glGetUniformLocation(id, name.c_str());
    if (location == -1) {
        std::cerr << "Error: Uniform variable '" << name.c_str() << "' not found in shader." << std::endl;
    }
    glUniform3fv(location, 1, &value[0]);
}

void Pipeline::setVec3(const std::string &name, float x, float y, float z)
{
    GLint location = glGetUniformLocation(id, name.c_str());
    if (location == -1) {
        std::cerr << "Error: Uniform variable '" << name.c_str() << "' not found in shader." << std::endl;
    }
    glUniform3f(location, x, y, z);
}

void Pipeline::setVec4(const std::string &name, const glm::vec4 &value)
{
    GLint location = glGetUniformLocation(id, name.c_str());
    if (location == -1) {
        std::cerr << "Error: Uniform variable '" << name.c_str() << "' not found in shader." << std::endl;
    }
    glUniform4fv(location, 1, &value[0]);
}

void Pipeline::setVec4(const std::string &name, float x, float y, float z, float w)
{
    GLint location = glGetUniformLocation(id, name.c_str());
    if (location == -1) {
        std::cerr << "Error: Uniform variable '" << name.c_str() << "' not found in shader." << std::endl;
    }
    glUniform4f(location, x, y, z, w);
}

void Pipeline::setMat3(const std::string &name, const glm::mat3 &mat)
{
    GLint location = glGetUniformLocation(id, name.c_str());
    if (location == -1) {
        std::cerr << "Error: Uniform variable '" << name.c_str() << "' not found in shader." << std::endl;
    }
    glUniformMatrix3fv(location, 1, GL_FALSE, &mat[0][0]);
}

void Pipeline::setMat4(const std::string &name, const glm::mat4 &mat)
{
    GLint location = glGetUniformLocation(id, name.c_str());
    if (location == -1) {
        std::cerr << "Error: Uniform variable '" << name.c_str() << "' not found in shader." << std::endl;
    }
    glUniformMatrix4fv(location, 1, GL_FALSE, &mat[0][0]);
}

unsigned int Pipeline::setVAO(const unsigned int &VBO, const unsigned int &EBO)
{
    if (vertexShader.getAttributeName().size() == 0){
        cout << "顶点变量没有设置名称数组" << endl;
    }
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    // 设置顶点位置指针
    // 这里逻辑存在缺陷 我获取到了顶点shader里顶点数据名称 本来应该选择遍历名称 找到位置 进行绑定
    // 但是需要更改名称数组 为每一个名称加上一个类型 不然找不到
    // 所以这里改成特殊形式 确定第三个顶点数据是纹理坐标
    // 以后有机会可以修改下这里
    vector<string> attributeName = vertexShader.getAttributeName();
    glEnableVertexAttribArray(glGetAttribLocation(id, attributeName[0].c_str()));
    glVertexAttribPointer(glGetAttribLocation(id, attributeName[0].c_str()), 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0); 
    glEnableVertexAttribArray(glGetAttribLocation(id, attributeName[1].c_str()));
    glVertexAttribPointer(glGetAttribLocation(id, attributeName[1].c_str()), 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    glEnableVertexAttribArray(glGetAttribLocation(id, attributeName[2].c_str()));
    glVertexAttribPointer(glGetAttribLocation(id, attributeName[2].c_str()), 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

    // 最后把VAO的状态区域设置空
    glBindVertexArray(0);

    return VAO;
}

void Pipeline::setTexture(const string &_type, unsigned int _id)
{
    // 目前 我这里也只有漫反射贴图
    // 由于目前 只有一张图片 这里暂时改成0 以免后面debug半天
    // glActiveTexture(GL_TEXTURE0 + texture_id);

    if (fragmentShader.getTextureName().size() == 0){
        cout << "贴图纹理变量没有设置名称数组" << endl;
    }

    glActiveTexture(GL_TEXTURE0 + 0);
    vector<string> textureName = fragmentShader.getTextureName();
    if (_type == "texture_diffuse"){
        // 同样 名字这里不遍历，就直接默认第一个给的就是对应着色器贴图对象名字
        // 正常情况下 应该是要根据当前激活序号 类型 找到名称数组里指定的位置
        // 或者在shader里 直接保存不同类型贴图的集合 这样管理起来会非常方便
        // 同时当前贴图序号也根据此情况设置多个
        // glUniform1i(glGetUniformLocation(id, textureName[0].c_str()), texture_id);
        glBindTexture(GL_TEXTURE_2D, _id);
        glUniform1i(glGetUniformLocation(id, textureName[0].c_str()), 0);
    }

    texture_id++;
}

void Pipeline::setShadowMap(unsigned int _id)
{
    glActiveTexture(GL_TEXTURE0 + 0);
    glBindTexture(GL_TEXTURE_2D, _id); // 将当前纹理对象，绑定到纹理状态区域去
    string shadowName = "uShadowMap";
    glUniform1i(glGetUniformLocation(id, shadowName.c_str()), 0);
}
