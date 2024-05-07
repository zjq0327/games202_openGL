#pragma once

#include "core.h"
#include<string>
#include <assimp/scene.h>

class Shader {
public:
	Shader(const char* vertexPath, const char* fragmentPath);
	~Shader();
	
	void begin();//开始使用当前Shader

	void end();//结束使用当前Shader



	// uniform functions
	void setBool(const std::string& name, bool value);
	void setInt(const std::string& name, int value);
	void setFloat(const std::string& name, float value);
	void set3Float(const std::string& name, float v1, float v2, float v3);
	void set3Float(const std::string& name, glm::vec3 v);
	void set4Float(const std::string& name, float v1, float v2, float v3, float v4);
	void set4Float(const std::string& name, aiColor4D color);
	void set4Float(const std::string& name, glm::vec4 v);
	void setMat4(const std::string& name, glm::mat4 val);

private:

	void checkShaderErrors(GLuint target,std::string type);

private:
	GLuint mProgram{ 0 };
};
